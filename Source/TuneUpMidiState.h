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
						public ChangeBroadcaster
{

public:

	//====================================================================

	TuneUpMidiState();
	~TuneUpMidiState();

	ValueTree getPluginStateNode();

	ValueTree getDefaultSessionNode();

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
		Reset to factory default
	*/
	void resetToFactoryDefault(bool callListeners = true);

	/*
		Sets the default options node of the instance (but doesn't initialize or pass data down)
	*/
	void setDefaultSessionNode(ValueTree defaultOptionsNodeIn);

	/*
		Replaces data with user default options
	*/
	void resetToDefaultSession(bool callListeners = true);

	/*
		Writes current defaultOptionsNode to application directory
	*/
	void writeDefaultSessionToFile();

	/*
		Sets the parent data node of the instance (but doesn't initialize or pass data down)
	*/
	void setPluginStateNode(ValueTree pluginStateNodeIn);

	/*
		Sets the session options node and loads the data
	*/
	void resetToPluginState(bool callListeners = true);

	/*
		Passes data from input node, and falls back to defaultOptions if necessary
	*/
	void loadSessionNode(ValueTree nodeOptionsIn, bool callListeners = true);

	/*
		Sets and renders new tuning in definition, leaving tuning out unchanged
	*/
	void setTuningIn(ValueTree& definitionIn, bool callListeners = true);

	/*
		Sets and renders new tuning out definition, leaving tuning in unchanged
	*/
	void setTuningOut(ValueTree& definitionIn, bool callListeners = true);

	/*
		Sets and renders tunings in and out
	*/
	void setTunings(ValueTree parentOptionsNode, bool callListeners = true);

	/*
		Toggles Dynamic Tuning processing
	*/
	void setDynamicTuning(bool isDynamicTuning);

	// SESSION PARAMETER SETTERS

	void setReferenceNoteIn(int noteIn);

	void setReferenceFreqIn(double freqIn);

	void setReferenceNoteOut(int noteIn);

	void setReferenceFreqOut(double freqIn);

	void setPitchbendRange(int newPitchBendRange);

	void setVoiceLimit(int limitIn);

	void setChannelMode(FreeChannelMode channelModeIn);

	void setChannelConfiguration(ValueTree channelPropertiesNodeIn);

	void setReuseChannels(bool reuseChannels);

	void setResetChannelPitchbend(bool resetPitchbend);

	//=======================================================================================

	class Listener
	{
	public:

		virtual void optionsLoaded(ValueTree optionsNode) {};

		virtual void tuningInLoaded(ValueTree newTuningInDef, Tuning* tuningInPtr) {};

		virtual void tuningOutLoaded(ValueTree newTuningOutDef, Tuning* tuningOutPtr) {};

		virtual void dynamicTuningModeChanged(bool isDynamic) {};
	};

	void addListener(TuneUpMidiState::Listener* listenerIn) { listeners.add(listenerIn); }

	void removeListener(TuneUpMidiState::Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

	ListenerList<Listener> listeners;

private:

	ValueTree factoryDefaultSessionNode;
	ValueTree sessionNode;
	ValueTree defaultSessionNode;

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