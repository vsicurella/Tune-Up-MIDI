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
	File defaultOptionsFile = File(defaultOptionsPath);
	setDefaultOptionsNode(ValueTree::fromXml(defaultOptionsFile.loadFileAsString()));

	setPluginStateNode(ValueTree(TuneUpIDs::tuneUpMidiStateId));

	originTuningDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(0));
	originTuning.reset(new Tuning(originTuningDefinition.render()));

	tuningDefinition.setDefinition(defaultOptionsNode.getChild(0).getChild(1));
	tuning.reset(new Tuning(tuningDefinition.render()));

	midiProcessor.reset(new TuneUpMidiProcessor(originTuning.get(), tuning.get(), notesInOn));

	// TODO: Move this to MIDICCListener interface
	// fill control change map with empty functions
	//for (int cc = 0; cc < 128; cc++)
	//{
	//	controlChangeMap.emplace(cc, std::function<void(void)>());
	//}

	midiProcessor->addControlListener(this);
}

TuneUpMidiState::~TuneUpMidiState()
{
	tuning = nullptr;
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

ValueTree TuneUpMidiState::getOriginTuningDefinition()
{
	return originTuningDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getOriginTuning()
{
	return originTuning.get();
}

ValueTree TuneUpMidiState::getTuningDefinition()
{
	return tuningDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getTuning()
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
	ValueTree definitionsList = defaultOptionsNode.getChildWithName(TuneUpIDs::defaultTuningsListId);

	if (!definitionsList.isValid())
	{
		definitionsList = ValueTree(defaultTuningsListId);
		definitionsList.addChild(TuningDefinition::getStandardTuningDefinition(), 0, nullptr);
		defaultOptionsNode.addChild(definitionsList, 0, nullptr);
	}

	if (definitionsList.getNumChildren() == 1)
		definitionsList.addChild(TuningDefinition::getStandardTuningDefinition(), 1, nullptr);
}

/*
	Replaces data with user default options
*/
void TuneUpMidiState::resetToDefaultOptions(bool saveToSession, bool sendChange)
{
	setTuningIn(defaultOptionsNode.getChildWithName(TuneUpIDs::defaultTuningsListId).getChild(0), saveToSession, sendChange);
	setTuningOut(defaultOptionsNode.getChildWithName(TuneUpIDs::defaultTuningsListId).getChild(1), saveToSession, sendChange);

	if (saveToSession)
	{
		pluginStateNode.removeAllChildren(nullptr);
		pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
	}
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
	
	// Ensure it has as tuning list
	ValueTree tuningList = sessionOptionsNode.getChildWithName(TuneUpIDs::tuningsInAndOutNodeId);
	if (!tuningList.isValid() || tuningList.getNumChildren() < 2)
	{
		tuningList = ValueTree(TuneUpIDs::tuningsInAndOutNodeId);
		tuningList.addChild(defaultOptionsNode.getChildWithName(defaultTuningsListId).getChild(0).createCopy(), 0, nullptr);
		tuningList.addChild(defaultOptionsNode.getChildWithName(defaultTuningsListId).getChild(1).createCopy(), 1, nullptr);
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

	setTuningIn(sessionOptionsNode.getChildWithName(tuningsInAndOutNodeId).getChild(0), sendMsg);
	setTuningOut(sessionOptionsNode.getChildWithName(tuningsInAndOutNodeId).getChild(1), sendMsg);

	// TODO: SEND OTHER APPROPRIATE DATA TO MIDI PROCESSOR
}

void TuneUpMidiState::writeSessionDataToState()
{
	ValueTree tuningList = sessionOptionsNode.getChildWithName(TuneUpIDs::tuningsInAndOutNodeId);
	tuningList.removeAllChildren(nullptr);
	tuningList.addChild(originTuningDefinition.getDefinition().createCopy(), 0, nullptr);
	tuningList.addChild(tuningDefinition.getDefinition().createCopy(), 1, nullptr);
	
	pluginStateNode.removeAllChildren(nullptr);
	pluginStateNode.addChild(sessionOptionsNode, 0, nullptr);
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