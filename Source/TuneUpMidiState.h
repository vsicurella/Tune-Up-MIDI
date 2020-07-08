/*
  ==============================================================================

    TuneUpMidiState.h
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "TuneUpMidiProcessor.h"
#include "MidiCCListener.h"
#include "TuningDefinition.h"

// TODO: Move this elsewhere

//static ValueTree jucerFile = ValueTree::fromXml(File("/Tune-Up MIDI!/Tune-Up MIDI!.jucer").getRelativePathFrom(File::getCurrentWorkingDirectory()));
static String applicationName = "Tune-Up MIDI!";
static String defaultOptionsPath = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(applicationName).getFullPathName();

class TuneUpMidiState : public MidiCCListener, 
						public ChangeBroadcaster,
						private ValueTree::Listener
{

public:

	//====================================================================

	TuneUpMidiState();
	~TuneUpMidiState();

	TuneUpMidiProcessor* getMidiProcessor();

	ValueTree getOriginTuningDefinition();
	Tuning* getOriginTuning();

	ValueTree getTuningDefinition();
	Tuning* getTuning();

	const MidiKeyboardState& getMidiKeyboardState();

	/*
		Loads a saved plugin state
	*/
	void loadPluginStateNode(ValueTree pluginStateNodeIn);

	/*
		Loads default options and passes data down
	*/
	void loadDefaultOptions();

	/*
		Loads session options and passes data down
	*/
	void loadSessionOptions(ValueTree sessionOptionsNodeIn);

	/*
		Sets and renders tuning in and out definitions
	*/
	void setTunings(ValueTree tuningInDefinition, ValueTree tuningOutDefinition, bool sendChangeSignal = false);

	/*
		Sets and renders new tuning in definition, leaving tuning out unchanged
	*/
	void setTuningIn(ValueTree definitionIn, bool sendChangeSignal = false);

	/*
		Sets and renders new tuning out definition, leaving tuning in unchanged
	*/
	void setTuningOut(ValueTree definitionIn, bool sendChangeSignal = false);

	void setDynamicTuningPeriodController(int controlNumber);
	void setDynamicTuningGeneratorController(int controlNumber);

	// CC Listener Implementation
	void controlValueChanged(int controlNumber, int controlValue) override;

private:

	void valueTreeChildAdded(ValueTree& parentTree, ValueTree& childAdded) override;

	void valueTreeChildRemoved(ValueTree& parentTree, ValueTree& childRemoved, int childRemovedIndex) override;

	void valueTreePropertyChanged(ValueTree& parentTree, const Identifier& property) override;

protected:

	ListenerList<Listener> listeners;


private:

	ValueTree pluginStateNode;
	ValueTree defaultOptionsNode;

	MidiKeyboardState midiState;

	std::unique_ptr<TuneUpMidiProcessor> midiProcessor;
	Array<int> notesInOn;

	// Properties
	String defaultTuningPath;
	bool dynamicTuningOn = false;

	TuningDefinition originTuningDefinition;
	std::unique_ptr<Tuning> originTuning;

	TuningDefinition tuningDefinition;
	std::unique_ptr<Tuning> tuning;

	// TODO: make a part of MidiCCListener interface
	// Midi Control number to function map
	//std::map<int, std::function<void(int)>> controlChangeMap;

};