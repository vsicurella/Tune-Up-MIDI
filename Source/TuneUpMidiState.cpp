/*
  ==============================================================================

    TuneUpMidiState.cpp
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiState.h"

TuneUpMidiState::TuneUpMidiState()
{
	DBG("PluginState for " + applicationName + " initalizing...");
	STD_TUNING = TuningDefinition::getStandardTuningDefinition();
	buildFactoryDefaultOptionsNode();

	// Initialize DefaultOptionsNode
	defaultOptionsNode = ValueTree(TuneUpIDs::optionsNodeId);
	File defaultOptionsFile = File(defaultOptionsFilePath);
	ValueTree defaultOptionsLoad = ValueTree::fromXml(defaultOptionsFile.loadFileAsString());
	DBG("Found this default options node:\n" + defaultOptionsLoad.toXmlString());
	setDefaultOptionsNode(defaultOptionsLoad);
	DBG("Loaded these default options:\n" + defaultOptionsNode.toXmlString());

	pluginStateNode = ValueTree(TuneUpIDs::tuneUpMidiStateId);
	pluginStateNode.appendChild(ValueTree(TuneUpIDs::optionsNodeId), nullptr);
	sessionOptionsNode = pluginStateNode.getChild(0);

	tuningInDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(0));
	tuningIn.reset(new Tuning(tuningInDefinition.render()));

	tuningOutDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(1));
	tuningOut.reset(new Tuning(tuningOutDefinition.render()));

	midiProcessor.reset(new TuneUpMidiProcessor(tuningIn.get(), tuningOut.get(), notesInOn));
	midiProcessor->addControlListener(this);
}

TuneUpMidiState::~TuneUpMidiState()
{
	writeDefaultOptionsToFile();
	tuningOut = nullptr;
	STD_TUNING = ValueTree();
}

ValueTree TuneUpMidiState::getPluginStateNode()
{ 
	return pluginStateNode;
}

ValueTree TuneUpMidiState::getDefaultOptionsNode()
{
	return defaultOptionsNode;
}

ValueTree TuneUpMidiState::getSessionOptionsNode()
{
	return sessionOptionsNode;
}

TuneUpMidiProcessor* TuneUpMidiState::getMidiProcessor()
{
	return midiProcessor.get();
}

ValueTree TuneUpMidiState::getTuningInDefinition()
{
	return tuningInDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getTuningIn()
{
	return tuningIn.get();
}

ValueTree TuneUpMidiState::getTuningOutDefinition()
{
	return tuningOutDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getTuningOut()
{
	return tuningOut.get();
}

const MidiKeyboardState& TuneUpMidiState::getMidiKeyboardState()
{
	return midiState;
}

/*
	Returns the factory default value for a given property
*/
var TuneUpMidiState::getFactoryDefaultValue(Identifier& propertyIn) const
{
	return factoryDefaultOptionsNode[propertyIn];
}

/*
	Loads a saved plugin state
*/
void TuneUpMidiState::setPluginStateNode(ValueTree pluginStateNodeIn)
{
	pluginStateNode = pluginStateNodeIn;
	if (!pluginStateNode.hasType(TuneUpIDs::tuneUpMidiStateId))
		pluginStateNode = ValueTree(TuneUpIDs::tuneUpMidiStateId);
	
	setSessionOptionsNode(pluginStateNode.getOrCreateChildWithName(TuneUpIDs::optionsNodeId, nullptr));
}

/*
	Reset to factory default
*/
void TuneUpMidiState::resetToFactoryDefault(bool saveToSession, bool sendChange)
{
	for (auto prop : tuneUpOptionIds)
	{
		var value = getFactoryDefaultValue(prop);

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			defaultOptionsNode.setProperty(prop, value, nullptr);
		}

		else if (prop == TuneUpIDs::tuningsListId)
		{
			setTunings(factoryDefaultOptionsNode, saveToSession, sendChange);
		}
		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			setReferenceNoteIn(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			setReferenceFreqIn(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			setReferenceNoteOut(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			setReferenceFreqOut(value, saveToSession);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			setPitchbendRange(value, saveToSession);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			setVoiceLimit(value, saveToSession);
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			setChannelMode(FreeChannelMode((int)value), saveToSession);
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			setReuseChannels(value, saveToSession);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			setResetChannelPitchbend(value, saveToSession);
		}
	}

	if (sendChange)
		sendChangeMessage();
}

/*
	Sets the default options node of the instance (but doesn't initialize or pass data down)
*/
void TuneUpMidiState::setDefaultOptionsNode(ValueTree defaultOptionsNodeIn)
{
	ValueTree nodeToCopy = defaultOptionsNodeIn;

	// Ensure it's the right type, reset if not
	if (!defaultOptionsNode.hasType(TuneUpIDs::optionsNodeId))
	{
		defaultOptionsNodeIn = factoryDefaultOptionsNode;
	}

	defaultOptionsNode.copyPropertiesAndChildrenFrom(nodeToCopy, nullptr);

	// Ensure tuning path exists
	defaultTuningPath = defaultOptionsNode[TuneUpIDs::defaultTuningFilePathId];
	if (defaultTuningPath.length() == 0)
	{
		defaultTuningPath = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
		defaultOptionsNode.setProperty(TuneUpIDs::defaultTuningFilePathId, defaultTuningPath, nullptr);
	}

	// Ensure it has a default tuning definitions list
	ValueTree definitionsList = defaultOptionsNode.getChildWithName(TuneUpIDs::tuningsListId);

	if (!definitionsList.isValid())
	{
		definitionsList = ValueTree(tuningsListId);
		definitionsList.addChild(STD_TUNING.createCopy(), 0, nullptr);
		defaultOptionsNode.addChild(definitionsList, 0, nullptr);
	}

	if (definitionsList.getNumChildren() == 1)
		definitionsList.addChild(STD_TUNING.createCopy(), 1, nullptr);
}

/*
	Replaces data with user default options
*/
void TuneUpMidiState::resetToDefaultOptions(bool saveToSession, bool sendChange)
{
	if (saveToSession)
	{
		pluginStateNode.removeAllChildren(nullptr);
		pluginStateNode.appendChild(sessionOptionsNode, nullptr);
	}

	loadNodeOptions(defaultOptionsNode, saveToSession);
}

/*
	Writes current defaultOptionsNode to application directory
*/
void TuneUpMidiState::writeDefaultOptionsToFile()
{
	File f = File(defaultOptionsFilePath);
	DBG("Writing default options:\n" + defaultOptionsNode.toXmlString());
	std::unique_ptr<XmlElement> xml = defaultOptionsNode.createXml();
	xml->writeToFile(f, "");
}

/*
	Sets the session options node of the instance (but doesn't initialize or pass data down)
*/
void TuneUpMidiState::setSessionOptionsNode(ValueTree sessionOptionsNodeIn)
{
	// Make sure it's valid
	ValueTree nodeToCopy = sessionOptionsNodeIn;
	if (!sessionOptionsNodeIn.hasType(TuneUpIDs::optionsNodeId))
	{
		nodeToCopy = factoryDefaultOptionsNode;
	}
	
	sessionOptionsNode.copyPropertiesAndChildrenFrom(nodeToCopy, nullptr);

	// Ensure it has as valid tuning list
	ValueTree tuningList = sessionOptionsNode.getOrCreateChildWithName(TuneUpIDs::tuningsListId, nullptr);
	bool validTuning = tuningList.getNumChildren() == 2;
	
	// Check if the tunings in the list are valid themselves
	if (validTuning)
	{
		for (auto child : tuningList)
		{
			validTuning *= TuningDefinition::isValid(child);
		}
	}

	if (!validTuning)
	{
		tuningList.removeAllChildren(nullptr);
		tuningList.addChild(defaultOptionsNode.getChildWithName(tuningsListId).getChild(0).createCopy(), 0, nullptr);
		tuningList.addChild(defaultOptionsNode.getChildWithName(tuningsListId).getChild(1).createCopy(), 1, nullptr);
	}

	DBG("SETSESSION:\n" + sessionOptionsNode.toXmlString());
}

/*
	Loads session options
*/
void TuneUpMidiState::resetToSessionOptions(bool sendMsg)
{	
	loadNodeOptions(sessionOptionsNode);
}

/*
	Passes data from input node, and falls back to defaultOptions if necessary
*/
void TuneUpMidiState::loadNodeOptions(ValueTree nodeOptionsIn, bool saveToSession)
{
	for (auto prop : tuneUpOptionIds)
	{
		ValueTree nodeToUse = nodeOptionsIn;
		if (!nodeOptionsIn.hasProperty(prop))
			nodeToUse = defaultOptionsNode;
		
		var value = nodeToUse[prop];

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			defaultOptionsNode.setProperty(prop, value, nullptr);
		}

		else if (prop == TuneUpIDs::tuningsListId)
		{
			setTunings(nodeOptionsIn, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			setReferenceNoteIn(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			setReferenceFreqIn(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			setReferenceNoteOut(value, saveToSession);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			setReferenceFreqOut(value, saveToSession);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			setPitchbendRange(value, saveToSession);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			setVoiceLimit(value, saveToSession);
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			setChannelMode(FreeChannelMode((int)value), saveToSession);
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			setReuseChannels(value, saveToSession);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			setResetChannelPitchbend(value, saveToSession);
		}
	}
}

/*
	Sets and renders new tuning in definition, leaving tuning out unchanged
*/
void TuneUpMidiState::setTuningIn(ValueTree definitionIn, bool writeToSession, bool sendChangeSignal)
{
	bool success = false;

	if (definitionIn.hasType(TuneUpIDs::tuningDefinitionId))
	{
		if (writeToSession)
		{
			DBG("NEW TUNING IN WRITTEN:\n" + definitionIn.toXmlString());
			sessionOptionsNode.getChild(0).getChild(0).copyPropertiesAndChildrenFrom(definitionIn, nullptr);
			tuningInDefinition.setDefinition(sessionOptionsNode);
		}
		else
		{
			DBG("NEW TUNING IN SET:\n" + definitionIn.toXmlString());
			tuningInDefinition.setDefinition(definitionIn);
		}

		tuningIn.reset(new Tuning(tuningInDefinition.render()));
		midiProcessor->setTuningIn(tuningIn.get());
		success = true;
	}

	if (success && sendChangeSignal)
		sendChangeMessage();
}

/*
	Sets and renders new tuning out definition, leaving tuning in unchanged
*/
void TuneUpMidiState::setTuningOut(ValueTree definitionIn, bool writeToSession, bool sendChangeSignal)
{
	bool success = false;

	if (definitionIn.hasType(TuneUpIDs::tuningDefinitionId))
	{
		if (writeToSession)
		{
			DBG("NEW TUNING OUT WRITTEN:\n" + definitionIn.toXmlString());
			sessionOptionsNode.getChild(0).getChild(1).copyPropertiesAndChildrenFrom(definitionIn, nullptr);
			tuningOutDefinition.setDefinition(sessionOptionsNode);
		}
		else
		{
			DBG("NEW TUNING OUT SET:\n" + definitionIn.toXmlString());
			tuningOutDefinition.setDefinition(definitionIn);
		}

		tuningOut.reset(new Tuning(tuningOutDefinition.render()));
		midiProcessor->setTuningOut(tuningOut.get());
		success = true;
	}

	if (success && sendChangeSignal)
		sendChangeMessage();
}

void TuneUpMidiState::setTunings(ValueTree parentOptionsNode, bool writeToSession, bool sendChangeSignal)
{
	setTuningIn(parentOptionsNode.getChild(0).getChild(0), writeToSession, false);
	setTuningOut(parentOptionsNode.getChild(0).getChild(1), writeToSession, false);

	if (sendChangeSignal)
		sendChangeMessage();
}


void TuneUpMidiState::setReferenceNoteIn(int noteIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteInId, noteIn, nullptr);

	midiProcessor->setReferenceNoteIn(noteIn);
}

void TuneUpMidiState::setReferenceFreqIn(double freqIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqInId, freqIn, nullptr);

	midiProcessor->setReferenceFreqIn(freqIn);
}

void TuneUpMidiState::setReferenceNoteOut(int noteIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteOutId, noteIn, nullptr);

	midiProcessor->setReferenceNoteOut(noteIn);
}

void TuneUpMidiState::setReferenceFreqOut(double freqIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqOutId, freqIn, nullptr);

	midiProcessor->setReferenceFreqOut(freqIn);
}

void TuneUpMidiState::setPitchbendRange(int newPitchBendRange, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::pitchbendRangeId, newPitchBendRange, nullptr);

	midiProcessor->setPitchbendRange(newPitchBendRange);
}

void TuneUpMidiState::setVoiceLimit(int limitIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::voiceLimitId, limitIn, nullptr);
	midiProcessor->setVoiceLimit(limitIn);
}

void TuneUpMidiState::setChannelMode(FreeChannelMode channelModeIn, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::channelModeId, channelModeIn, nullptr);

	midiProcessor->setFreeChannelMode(channelModeIn);
}

void TuneUpMidiState::setReuseChannels(bool reuseChannels, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::reuseChannelsId, reuseChannels, nullptr);

	midiProcessor->setReuseChannels(reuseChannels);
}

void TuneUpMidiState::setResetChannelPitchbend(bool resetPitchbend, bool saveToSession)
{
	if (saveToSession)
		sessionOptionsNode.setProperty(TuneUpIDs::resetChannelPitchbendId, resetPitchbend, nullptr);

	midiProcessor->setResetChannelPitchbendWhenEmpty(resetPitchbend);
}

void TuneUpMidiState::setDynamicTuningPeriodController(int controlNumber)
{
	
}

void TuneUpMidiState::setDynamicTuningGeneratorController(int controlNumber)
{

}

void TuneUpMidiState::controlValueChanged(int controlNumber, int controlValue)
{

}

void TuneUpMidiState::valueTreeChildAdded(ValueTree& parentTree, ValueTree& childAdded)
{
	
}

void TuneUpMidiState::valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childRemoved, int childRemovedIndex)
{

}

void TuneUpMidiState::valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property)
{

}

void TuneUpMidiState::buildFactoryDefaultOptionsNode()
{
	factoryDefaultOptionsNode = ValueTree(TuneUpIDs::optionsNodeId);

	for (auto prop : tuneUpOptionIds)
	{
		var value;

		// DEFAULT OPTIONS

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			value = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
		}

		else if (prop == TuneUpIDs::tuningsListId) // TODO implement different defaut tuning in & out
		{
			ValueTree tuningListNode = ValueTree(TuneUpIDs::tuningsListId);
			tuningListNode.addChild(STD_TUNING.createCopy(), 0, nullptr);
			tuningListNode.addChild(STD_TUNING.createCopy(), 1, nullptr);
			factoryDefaultOptionsNode.addChild(tuningListNode, 0, nullptr);
			continue;
		}

		// DEFAULT OR SESSION

		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			value = 69;
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			value = 440.0;
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			value = 60;
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			value = 261.626;
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			value = 2;
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			value = 16;
		}

		else if (prop == TuneUpIDs::channelConfigurationId)
		{
			// TODO
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			value = 1;
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			value = false;
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			value = false;
		}

		factoryDefaultOptionsNode.setProperty(prop, value, nullptr);
	}
}