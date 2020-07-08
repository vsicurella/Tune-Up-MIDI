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
	defaultOptionsNode = ValueTree::fromXml(defaultOptionsFile.loadFileAsString());

	loadDefaultOptions();

	originTuning.reset(new Tuning(originTuningDefinition.render()));
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
void TuneUpMidiState::loadPluginStateNode(ValueTree pluginStateNodeIn)
{
	pluginStateNode = pluginStateNodeIn;

	ValueTree tuningsNode = pluginStateNode.getChildWithName(TuneUpIDs::tuningsInAndOutNodeId);
	setTunings(tuningsNode.getChild(0), tuningsNode.getChild(1), true);

}

/*
	Loads default options and passes data down
*/
void TuneUpMidiState::loadDefaultOptions()
{
	ValueTree definitionsList = defaultOptionsNode.getChildWithName(TuneUpIDs::defaultTuningsListId);
	setTunings(definitionsList.getChild(0), definitionsList.getChild(1));

	defaultTuningPath = defaultOptionsNode[TuneUpIDs::defaultTuningFilePathId];
	if (defaultTuningPath.length() == 0)
	{
		defaultTuningPath = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
	}
}

/*
	Loads session options and passes data down
*/
void TuneUpMidiState::loadSessionOptions(ValueTree sessionOptionsNodeIn)
{

}

/*
	Sets and renders tuning in and out definitions
*/
void TuneUpMidiState::setTunings(ValueTree tuningInDefinition, ValueTree tuningOutDefinition, bool sendChangeSignal)
{
	bool success = false;

	if (tuningInDefinition.hasType(TuneUpIDs::tuningDefinitionId))
	{
		DBG("NEW TUNING IN SET");
		
		originTuningDefinition.setDefinition(tuningInDefinition);
		originTuning.reset(new Tuning(originTuningDefinition.render()));

		midiProcessor->setTuningIn(originTuning.get());
		success = true;
	}

	if (tuningOutDefinition.hasType(TuneUpIDs::tuningDefinitionId))
	{
		DBG("NEW TUNING OUT SET");

		tuningDefinition.setDefinition(tuningInDefinition);
		tuning.reset(new Tuning(tuningDefinition.render()));

		midiProcessor->setTuningOut(tuning.get());
		success = true;
	}

	if (success && sendChangeSignal)
		sendChangeMessage();
}

/*
	Sets and renders new tuning in definition, leaving tuning out unchanged
*/
void TuneUpMidiState::setTuningIn(ValueTree definitionIn, bool sendChangeSignal)
{
	setTunings(definitionIn, ValueTree(), sendChangeSignal);
}

/*
	Sets and renders new tuning out definition, leaving tuning in unchanged
*/
void TuneUpMidiState::setTuningOut(ValueTree definitionIn, bool sendChangeSignal)
{
	setTunings(ValueTree(), definitionIn, sendChangeSignal);
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