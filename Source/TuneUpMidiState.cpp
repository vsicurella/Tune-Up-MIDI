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
	defaultSessionNode = ValueTree(TuneUpIDs::tuneUpMidiSessionId);
	File defaultOptionsFile = File(defaultOptionsFilePath);
	ValueTree defaultOptionsLoad = ValueTree::fromXml(defaultOptionsFile.loadFileAsString());
	DBG("Found this default options node:\n" + defaultOptionsLoad.toXmlString());
	setDefaultSessionNode(defaultOptionsLoad);
	DBG("Loaded these default options:\n" + defaultSessionNode.toXmlString());

	sessionNode = ValueTree(TuneUpIDs::tuneUpMidiSessionId);

	tuningInDefinition.setDefinition(defaultSessionNode.getChild(0).getChild(0));
	tuningIn.reset(new Tuning(tuningInDefinition.render()));

	tuningOutDefinition.setDefinition(defaultSessionNode.getChild(0).getChild(1));
	tuningOut.reset(new Tuning(tuningOutDefinition.render()));

	midiProcessor.reset(new TuneUpMidiProcessor(tuningIn.get(), tuningOut.get(), notesInOn));
	midiProcessor->addControlListener(this);
}

TuneUpMidiState::~TuneUpMidiState()
{
	writeDefaultSessionToFile();
	tuningOut = nullptr;
	STD_TUNING = ValueTree();
}

ValueTree TuneUpMidiState::getPluginStateNode()
{ 
	return sessionNode;
}

ValueTree TuneUpMidiState::getDefaultSessionNode()
{
	return defaultSessionNode;
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
	return factoryDefaultSessionNode[propertyIn];
}

/*
	Reset to factory default
*/
void TuneUpMidiState::resetToFactoryDefault(bool callListeners)
{
	for (auto prop : tuneUpOptionIds)
	{
		var value = factoryDefaultSessionNode[prop];

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			defaultSessionNode.setProperty(prop, value, nullptr);
		}

		else if (prop == TuneUpIDs::tuningsListId)
		{
			setTunings(factoryDefaultSessionNode, callListeners);
		}
		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			setReferenceNoteIn(value);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			setReferenceFreqIn(value);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			setReferenceNoteOut(value);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			setReferenceFreqOut(value);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			setPitchbendRange(value);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			setVoiceLimit(value);
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			setChannelMode(FreeChannelMode((int)value));
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			setReuseChannels(value);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			setResetChannelPitchbend(value);
		}
	}

	if (callListeners)
		listeners.call(&TuneUpMidiState::Listener::optionsLoaded, sessionNode);
}

/*
	Sets the default options node of the instance (but doesn't initialize or pass data down)
*/
void TuneUpMidiState::setDefaultSessionNode(ValueTree defaultOptionsNodeIn)
{
	ValueTree nodeToCopy = defaultOptionsNodeIn;

	// Ensure it's the right type, reset if not
	if (!defaultSessionNode.hasType(TuneUpIDs::tuneUpMidiSessionId))
	{
		nodeToCopy = factoryDefaultSessionNode;
	}

	defaultSessionNode.copyPropertiesAndChildrenFrom(nodeToCopy, nullptr);

	// Ensure tuning path exists
	defaultTuningPath = defaultSessionNode[TuneUpIDs::defaultTuningFilePathId];
	if (defaultTuningPath.length() == 0)
	{
		defaultTuningPath = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
		defaultSessionNode.setProperty(TuneUpIDs::defaultTuningFilePathId, defaultTuningPath, nullptr);
	}

	// Ensure it has a default tuning definitions list
	ValueTree definitionsList = defaultSessionNode.getChildWithName(TuneUpIDs::tuningsListId);

	if (!definitionsList.isValid())
	{
		definitionsList = ValueTree(tuningsListId);
		definitionsList.addChild(STD_TUNING.createCopy(), 0, nullptr);
		defaultSessionNode.addChild(definitionsList, 0, nullptr);
	}

	if (definitionsList.getNumChildren() == 1)
		definitionsList.addChild(STD_TUNING.createCopy(), 1, nullptr);
}

/*
	Replaces data with user default options
*/
void TuneUpMidiState::resetToDefaultSession(bool callListeners)
{
	loadSessionNode(defaultSessionNode.createCopy());
}

/*
	Writes current defaultOptionsNode to application directory
*/
void TuneUpMidiState::writeDefaultSessionToFile()
{
	File f = File(defaultOptionsFilePath);
	DBG("Writing default options:\n" + defaultSessionNode.toXmlString());
	std::unique_ptr<XmlElement> xml = defaultSessionNode.createXml();
	xml->writeToFile(f, "");
}

/*
	Sets the session node of the instance (but doesn't initialize or pass data down)
*/
void TuneUpMidiState::setPluginStateNode(ValueTree pluginStateNodeIn)
{
	// Make sure it's valid
	ValueTree nodeToCopy = pluginStateNodeIn;
	if (!pluginStateNodeIn.hasType(TuneUpIDs::tuneUpMidiSessionId))
	{
		nodeToCopy = factoryDefaultSessionNode;
	}

	if (sessionNode != pluginStateNodeIn)
	{
		sessionNode.copyPropertiesAndChildrenFrom(nodeToCopy, nullptr);

		// Sessions don't need the default file path property
		sessionNode.removeProperty(TuneUpIDs::defaultTuningFilePathId, nullptr);
	}

	// Ensure it has as valid tuning list
	ValueTree tuningList = sessionNode.getOrCreateChildWithName(TuneUpIDs::tuningsListId, nullptr);
	bool validTuning = tuningList.getNumChildren() == 2;

	// Check if the tunings in the list are valid themselves
	if (validTuning)
	{
		for (auto child : tuningList)
		{
			validTuning *= TuningDefinition::isValid(child) > 0;
		}
	}

	// Check if it's a default node, which has a different ID for the tuning list
	else
	{
		ValueTree defaultTunings = tuningList.getChildWithName(TuneUpIDs::defaultTuningsListId);
		if (defaultTunings.isValid() && defaultTunings.getNumChildren() == 2)
		{
			sessionNode.getChildWithName(TuneUpIDs::defaultTuningsListId).copyPropertiesAndChildrenFrom(defaultTunings, nullptr);
			validTuning = true;
		}
	}

	if (!validTuning)
	{
		tuningList.removeAllChildren(nullptr);
		tuningList.addChild(defaultSessionNode.getChildWithName(tuningsListId).getChild(0).createCopy(), 0, nullptr);
		tuningList.addChild(defaultSessionNode.getChildWithName(tuningsListId).getChild(1).createCopy(), 1, nullptr);
	}

	DBG("SETSESSION:\n" + sessionNode.toXmlString());
}

/*
	Loads session options
*/
void TuneUpMidiState::resetToPluginState(bool sendMsg)
{	
	loadSessionNode(sessionNode);
}

/*
	Passes data from input node, and falls back to defaultOptions if necessary
*/
void TuneUpMidiState::loadSessionNode(ValueTree nodeOptionsIn, bool callListeners)
{
	for (auto prop : tuneUpOptionIds)
	{
		ValueTree nodeToUse = nodeOptionsIn;
		if (!nodeToUse.hasProperty(prop))
			nodeToUse = defaultSessionNode;
		
		var value = nodeToUse[prop];

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			defaultSessionNode.setProperty(prop, value, nullptr);
		}

		else if (prop == TuneUpIDs::tuningsListId)
		{
			// TODO: Improve with default nodes
			setTunings(nodeOptionsIn);
		}

		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			setReferenceNoteIn(value);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			setReferenceFreqIn(value);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			setReferenceNoteOut(value);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			setReferenceFreqOut(value);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			setPitchbendRange(value);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			setVoiceLimit(value);
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			setChannelMode(FreeChannelMode((int)value));
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			setReuseChannels(value);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			setResetChannelPitchbend(value);
		}
	}

	if (callListeners)
		listeners.call(&TuneUpMidiState::Listener::optionsLoaded, sessionNode.getChild(0));
}

/*
	Sets and renders new tuning in definition, leaving tuning out unchanged
*/
void TuneUpMidiState::setTuningIn(ValueTree& definitionIn, bool callListeners)
{
	ValueTree currentDefinition = sessionNode.getChild(0).getChild(0);

	if (definitionIn.hasType(TuneUpIDs::tuningDefinitionId) && currentDefinition != definitionIn)
	{
		currentDefinition.copyPropertiesAndChildrenFrom(definitionIn, nullptr);
		tuningInDefinition.setDefinition(currentDefinition);
		DBG("NEW TUNING IN WRITTEN:\n" + sessionNode.toXmlString());

		tuningIn.reset(new Tuning(tuningInDefinition.render()));
		midiProcessor->setTuningIn(tuningIn.get()); // TODO: move out of plugin state? 

		if (callListeners)
			listeners.call(&TuneUpMidiState::Listener::tuningInLoaded, tuningInDefinition.getDefinition(), tuningIn.get());
	}
}

/*
	Sets and renders new tuning out definition, leaving tuning in unchanged
*/
void TuneUpMidiState::setTuningOut(ValueTree& definitionIn, bool callListeners)
{
	ValueTree currentDefinition = sessionNode.getChild(0).getChild(1);

	if (definitionIn.hasType(TuneUpIDs::tuningDefinitionId) && currentDefinition != definitionIn)
	{
		currentDefinition.copyPropertiesAndChildrenFrom(definitionIn, nullptr);
		tuningOutDefinition.setDefinition(currentDefinition);
		DBG("NEW TUNING OUT WRITTEN:\n" + sessionNode.toXmlString());

		tuningOut.reset(new Tuning(tuningOutDefinition.render()));
		midiProcessor->setTuningOut(tuningOut.get());

		if (callListeners)
			listeners.call(&TuneUpMidiState::Listener::tuningOutLoaded, tuningOutDefinition.getDefinition(), tuningOut.get());
	}
}

void TuneUpMidiState::setTunings(ValueTree parentOptionsNode, bool callListeners)
{
	setTuningIn(parentOptionsNode.getChild(0).getChild(0), callListeners);
	setTuningOut(parentOptionsNode.getChild(0).getChild(1), callListeners);
}

/*
	Toggles Dynamic Tuning processing
*/
void TuneUpMidiState::setDynamicTuning(bool isDynamicTuning)
{
	dynamicTuningOn = isDynamicTuning;

	// TODO

	listeners.call(&TuneUpMidiState::Listener::dynamicTuningModeChanged, dynamicTuningOn);
}

void TuneUpMidiState::setReferenceNoteIn(int noteIn)
{
	sessionNode.setProperty(TuneUpIDs::referenceNoteInId, noteIn, nullptr);
	midiProcessor->setReferenceNoteIn(noteIn);
}

void TuneUpMidiState::setReferenceFreqIn(double freqIn)
{
	sessionNode.setProperty(TuneUpIDs::referenceFreqInId, freqIn, nullptr);
	midiProcessor->setReferenceFreqIn(freqIn);
}

void TuneUpMidiState::setReferenceNoteOut(int noteIn)
{
	sessionNode.setProperty(TuneUpIDs::referenceNoteOutId, noteIn, nullptr);
	midiProcessor->setReferenceNoteOut(noteIn);
}

void TuneUpMidiState::setReferenceFreqOut(double freqIn)
{
	sessionNode.setProperty(TuneUpIDs::referenceFreqOutId, freqIn, nullptr);
	midiProcessor->setReferenceFreqOut(freqIn);
}

void TuneUpMidiState::setPitchbendRange(int newPitchBendRange)
{
	sessionNode.setProperty(TuneUpIDs::pitchbendRangeId, newPitchBendRange, nullptr);
	midiProcessor->setPitchbendRange(newPitchBendRange);
}

void TuneUpMidiState::setVoiceLimit(int limitIn)
{
	sessionNode.setProperty(TuneUpIDs::voiceLimitId, limitIn, nullptr);
	midiProcessor->setVoiceLimit(limitIn);
}

void TuneUpMidiState::setChannelMode(FreeChannelMode channelModeIn)
{
	sessionNode.setProperty(TuneUpIDs::channelModeId, channelModeIn, nullptr);
	midiProcessor->setFreeChannelMode(channelModeIn);
}

void TuneUpMidiState::setReuseChannels(bool reuseChannels)
{
	sessionNode.setProperty(TuneUpIDs::reuseChannelsId, reuseChannels, nullptr);
	midiProcessor->setReuseChannels(reuseChannels);
}

void TuneUpMidiState::setResetChannelPitchbend(bool resetPitchbend)
{
	sessionNode.setProperty(TuneUpIDs::resetChannelPitchbendId, resetPitchbend, nullptr);
	midiProcessor->setResetChannelPitchbendWhenEmpty(resetPitchbend);
}

void TuneUpMidiState::buildFactoryDefaultOptionsNode()
{
	factoryDefaultSessionNode = ValueTree(TuneUpIDs::tuneUpMidiSessionId);

	for (auto prop : tuneUpOptionIds)
	{
		var value;

		// DEFAULT OPTIONS

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			value = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
		}

		else if (prop == TuneUpIDs::defaultTuningsListId)
		{
			ValueTree tuningListNode = ValueTree(TuneUpIDs::tuningsListId);
			tuningListNode.addChild(STD_TUNING.createCopy(), 0, nullptr);
			tuningListNode.addChild(STD_TUNING.createCopy(), 1, nullptr);
			factoryDefaultSessionNode.addChild(tuningListNode, 0, nullptr);
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

		factoryDefaultSessionNode.setProperty(prop, value, nullptr);
	}
}