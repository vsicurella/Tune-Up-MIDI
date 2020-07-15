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

	File defaultOptionsFile = File(defaultOptionsFilePath);
	ValueTree defaultOptionsLoad = ValueTree::fromXml(defaultOptionsFile.loadFileAsString());
	DBG("Found this default options node:\n" + defaultOptionsLoad.toXmlString());
	setDefaultOptionsNode(defaultOptionsLoad);
	DBG("Loaded these default options:\n" + defaultOptionsNode.toXmlString());

	setPluginStateNode(ValueTree(TuneUpIDs::tuneUpMidiStateId));

	originTuningDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(0));
	originTuning.reset(new Tuning(originTuningDefinition.render()));

	tuningDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(1));
	tuning.reset(new Tuning(tuningDefinition.render()));

	midiProcessor.reset(new TuneUpMidiProcessor(originTuning.get(), tuning.get(), notesInOn));
	midiProcessor->addControlListener(this);
}

TuneUpMidiState::~TuneUpMidiState()
{
	writeDefaultOptionsToFile();
	tuning = nullptr;
	STD_TUNING = ValueTree();
}

ValueTree TuneUpMidiState::getPluginStateNode()
{ 
	if (!pluginStateNode.getChild(0).isEquivalentTo(sessionOptionsNode))
	{
		pluginStateNode.removeAllChildren(nullptr);
		pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
	}
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
	return originTuningDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getTuningIn()
{
	return originTuning.get();
}

ValueTree TuneUpMidiState::getTuningOutDefinition()
{
	return tuningDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getTuningOut()
{
	return tuning.get();
}

const MidiKeyboardState& TuneUpMidiState::getMidiKeyboardState()
{
	return midiState;
}

/*
	Loads a saved plugin state
*/
void TuneUpMidiState::setPluginStateNode(ValueTree pluginStateNodeIn)
{
	pluginStateNode = pluginStateNodeIn;
	if (!pluginStateNode.hasType(TuneUpIDs::tuneUpMidiStateId))
		pluginStateNode = ValueTree(TuneUpIDs::tuneUpMidiStateId);
	
	setSessionOptionsNode(pluginStateNode.getChildWithName(TuneUpIDs::sessionOptionsNodeId));
}

/*
	Reset to factory default
*/
void TuneUpMidiState::resetToFactoryDefault(bool sendChange)
{
	/* TODO */
}

/*
	Sets the default options node of the instance (but doesn't initialize or pass data down)
*/
void TuneUpMidiState::setDefaultOptionsNode(ValueTree defaultOptionsNodeIn)
{
	defaultOptionsNode = defaultOptionsNodeIn;

	// Ensure it's the right type, reset if not
	if (!defaultOptionsNode.hasType(TuneUpIDs::defaultOptionsNodeId))
	{
		defaultOptionsNode = ValueTree(TuneUpIDs::defaultOptionsNodeId);
	}

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
	setTuningIn(defaultOptionsNode.getChildWithName(TuneUpIDs::tuningsListId).getChild(0), saveToSession, sendChange);
	setTuningOut(defaultOptionsNode.getChildWithName(TuneUpIDs::tuningsListId).getChild(1), saveToSession, sendChange);

	if (saveToSession)
	{
		pluginStateNode.removeAllChildren(nullptr);
		pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
	}

	loadNodeOptions(defaultOptionsNode);
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
	sessionOptionsNode = sessionOptionsNodeIn;

	if (!sessionOptionsNode.hasType(TuneUpIDs::sessionOptionsNodeId))
	{
		sessionOptionsNode = ValueTree(TuneUpIDs::sessionOptionsNodeId);
		pluginStateNode.removeAllChildren(nullptr);
		pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
	}
	
	// Ensure it has as valid tuning list
	ValueTree tuningList = sessionOptionsNode.getChildWithName(TuneUpIDs::tuningsListId);
	bool validTuning = tuningList.isValid() && tuningList.getNumChildren() == 2;
	
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
		tuningList = ValueTree(TuneUpIDs::tuningsListId);
		tuningList.addChild(defaultOptionsNode.getChildWithName(tuningsListId).getChild(0).createCopy(), 0, nullptr);
		tuningList.addChild(defaultOptionsNode.getChildWithName(tuningsListId).getChild(1).createCopy(), 1, nullptr);
		sessionOptionsNode.removeAllChildren(nullptr);
		sessionOptionsNode.addChild(tuningList, 0, nullptr);
	}
}

/*
	Loads session options
*/
void TuneUpMidiState::resetToSessionOptions(bool sendMsg)
{	
	pluginStateNode.removeAllChildren(nullptr);
	pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);

	setTuningIn(sessionOptionsNode.getChildWithName(tuningsListId).getChild(0), sendMsg);
	setTuningOut(sessionOptionsNode.getChildWithName(tuningsListId).getChild(1), sendMsg);

	loadNodeOptions(sessionOptionsNode);
}

void TuneUpMidiState::writeSessionDataToState()
{
	ValueTree tuningList = sessionOptionsNode.getChildWithName(TuneUpIDs::tuningsListId);
	tuningList.removeAllChildren(nullptr);
	tuningList.addChild(originTuningDefinition.getDefinition().createCopy(), 0, nullptr);
	tuningList.addChild(tuningDefinition.getDefinition().createCopy(), 1, nullptr);
	
	pluginStateNode.removeAllChildren(nullptr);
	pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
}

/*
	Passes data from input node, and falls back to defaultOptions if necessary
*/
void TuneUpMidiState::loadNodeOptions(ValueTree nodeOptionsIn)
{
	//ValueTree defaultTuningList = defaultOptionsNode.getChildWithName(TuneUpIDs::tuningsListId);
	//ValueTree inputTuningList = nodeOptionsIn.getChildWithName(TuneUpIDs::tuningsListId);

	for (auto prop : availableOptions)
	{
		ValueTree nodeToUse = nodeOptionsIn;
		if (!nodeOptionsIn.hasProperty(prop))
			nodeToUse = defaultOptionsNode;
		
		var value = nodeToUse[prop];

		if (prop == TuneUpIDs::referenceNoteInId)
		{
			midiProcessor->setReferenceNoteIn(value);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			midiProcessor->setReferenceFreqIn(value);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			midiProcessor->setReferenceNoteOut(value);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			midiProcessor->setReferenceFreqOut(value);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			midiProcessor->setPitchbendRange(value);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			midiProcessor->setVoiceLimit(value);
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			midiProcessor->setFreeChannelMode(FreeChannelMode((int)value));
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			midiProcessor->setReuseChannels(value);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			midiProcessor->setResetChannelPitchbendWhenEmpty(value);
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
		DBG("NEW TUNING IN SET:\n" + definitionIn.toXmlString());

		originTuningDefinition.setDefinition(definitionIn);
		originTuning.reset(new Tuning(originTuningDefinition.render()));

		midiProcessor->setTuningIn(originTuning.get());
		success = true;

		if (writeToSession)
		{
			sessionOptionsNode.getChild(0).getChild(0).copyPropertiesAndChildrenFrom(definitionIn, nullptr);
		}
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
		DBG("NEW TUNING OUT SET:\n" + definitionIn.toXmlString());

		tuningDefinition.setDefinition(definitionIn);
		tuning.reset(new Tuning(tuningDefinition.render()));

		midiProcessor->setTuningOut(tuning.get());
		success = true;

		if (writeToSession)
		{
			sessionOptionsNode.getChild(0).getChild(1).copyPropertiesAndChildrenFrom(definitionIn, nullptr);
		}
	}

	if (success && sendChangeSignal)
		sendChangeMessage();
}

void TuneUpMidiState::setReferenceNoteIn(int noteIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteInId, noteIn, nullptr);
	midiProcessor->setReferenceNoteIn(noteIn);
}

void TuneUpMidiState::setReferenceFreqIn(double freqIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqInId, freqIn, nullptr);
	midiProcessor->setReferenceFreqIn(freqIn);
}

void TuneUpMidiState::setReferenceNoteOut(int noteIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteOutId, noteIn, nullptr);
	midiProcessor->setReferenceNoteOut(noteIn);
}

void TuneUpMidiState::setReferenceFreqOut(double freqIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqOutId, freqIn, nullptr);
	midiProcessor->setReferenceFreqOut(freqIn);
}

void TuneUpMidiState::setPitchbendRange(int newPitchBendRange)
{
	sessionOptionsNode.setProperty(TuneUpIDs::pitchbendRangeId, newPitchBendRange, nullptr);
	midiProcessor->setPitchbendRange(newPitchBendRange);
}

void TuneUpMidiState::setVoiceLimit(int limitIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::voiceLimitId, limitIn, nullptr);
	midiProcessor->setVoiceLimit(limitIn);
}

void TuneUpMidiState::setChannelMode(FreeChannelMode channelModeIn)
{
	sessionOptionsNode.setProperty(TuneUpIDs::channelModeId, channelModeIn, nullptr);
	midiProcessor->setFreeChannelMode(channelModeIn);
}

void TuneUpMidiState::setReuseChannels(bool reuseChannels)
{
	sessionOptionsNode.setProperty(TuneUpIDs::reuseChannelsId, reuseChannels, nullptr);
	midiProcessor->setReuseChannels(reuseChannels);
}

void TuneUpMidiState::setResetChannelPitchbend(bool resetPitchbend)
{
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