/*
  ==============================================================================

    TuneUpMidiState.h
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "Constants.h"
#include "TuneUpMidiProcessor.h"
#include "MidiCCListener.h"

class TuneUpMidiState : public MidiCCListener, 
						public ChangeBroadcaster,
						private ValueTree::Listener
{

public:

	//====================================================================

	TuneUpMidiState();
	~TuneUpMidiState();

	ValueTree getPluginStateNode();

	ValueTree getDefaultOptionsNode();

	ValueTree getSessionOptionsNode();

	TuneUpMidiProcessor* getMidiProcessor();

	ValueTree getTuningInDefinition();
	Tuning* getTuningIn();

	ValueTree getTuningOutDefinition();
	Tuning* getTuningOut();

	const MidiKeyboardState& getMidiKeyboardState();

	/*
		Returns the factory default value for a given property
	*/
	var getFactoryDefaultValue(Identifier& propertyIn) const;

	/*
		Sets the parent data node of the instance (but doesn't initialize or pass data down)
	*/
	void setPluginStateNode(ValueTree pluginStateNodeIn);

	/*
		Reset to factory default
	*/
	void resetToFactoryDefault(bool saveToSession = false, bool sendChange = true);

	/*
		Sets the default options node of the instance (but doesn't initialize or pass data down)
	*/
	void setDefaultOptionsNode(ValueTree defaultOptionsNodeIn);

	/*
		Replaces data with user default options
	*/
	void resetToDefaultOptions(bool saveToSession = false, bool sendChange = true);

	/*
		Writes current defaultOptionsNode to application directory
	*/
	void writeDefaultOptionsToFile();

	/*
		Sets the session options node of the instance (but doesn't initialize or pass data down)
	*/
	void setSessionOptionsNode(ValueTree sessionOptionsNodeIn);

	/*
		Sets the session options node and loads the data
	*/
	void resetToSessionOptions(bool sendChangeMessage = true);

	/*
		Passes data from input node, and falls back to defaultOptions if necessary
	*/
	void loadNodeOptions(ValueTree nodeOptionsIn, bool saveToSession = true);

	/*
		Sets and renders new tuning in definition, leaving tuning out unchanged
	*/
	void setTuningIn(ValueTree definitionIn, bool writeToSession = true, bool sendChangeSignal = false);

	/*
		Sets and renders new tuning out definition, leaving tuning in unchanged
	*/
	void setTuningOut(ValueTree definitionIn, bool writeToSession = true, bool sendChangeSignal = false);

	/*
		Sets and renders tunings in and out
	*/
	void setTunings(ValueTree parentOptionsNode, bool writeToSession, bool sendChangeSignal = false);

	// SESSION PARAMETER SETTERS

	void setReferenceNoteIn(int noteIn, bool saveToSession = true);

	void setReferenceFreqIn(double freqIn, bool saveToSession = true);

	void setReferenceNoteOut(int noteIn, bool saveToSession = true);

	void setReferenceFreqOut(double freqIn, bool saveToSession = true);

	void setPitchbendRange(int newPitchBendRange, bool saveToSession = true);

	void setVoiceLimit(int limitIn, bool saveToSession = true);

	void setChannelMode(FreeChannelMode channelModeIn, bool saveToSession = true);

	void setReuseChannels(bool reuseChannels, bool saveToSession = true);

	void setResetChannelPitchbend(bool resetPitchbend, bool saveToSession = true);

	//=======================================================================================

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

	ValueTree factoryDefaultOptionsNode;

	ValueTree pluginStateNode;
	ValueTree defaultOptionsNode;
	ValueTree sessionOptionsNode;

	MidiKeyboardState midiState;

	std::unique_ptr<TuneUpMidiProcessor> midiProcessor;
	Array<int> notesInOn;

	// Properties
	String defaultTuningPath;
	bool dynamicTuningOn = false;

	TuningDefinition tuningInDefinition;
	std::unique_ptr<Tuning> tuningIn;

	TuningDefinition tuningOutDefinition;
	std::unique_ptr<Tuning> tuningOut;

	// TODO: make a part of MidiCCListener interface
	// Midi Control number to function map
	//std::map<int, std::function<void(int)>> controlChangeMap;


private:

	void buildFactoryDefaultOptionsNode();

};