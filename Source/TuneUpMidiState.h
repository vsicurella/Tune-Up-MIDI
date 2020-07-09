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

	ValueTree getPluginStateNode();

	ValueTree getDefaultOptionsNode();

	ValueTree getSessionOptionsNode();

	TuneUpMidiProcessor* getMidiProcessor();

	ValueTree getOriginTuningDefinition();
	Tuning* getOriginTuning();

	ValueTree getTuningDefinition();
	Tuning* getTuning();

	const MidiKeyboardState& getMidiKeyboardState();

	/*
		Sets the parent data node of the instance (but doesn't initialize or pass data down)
	*/
	void setPluginStateNode(ValueTree pluginStateNodeIn);

	/*
		Reset to factory default
	*/
	void resetToFactoryDefault(bool sendChange = true);

	/*
		Sets the default options node of the instance (but doesn't initialize or pass data down)
	*/
	void setDefaultOptionsNode(ValueTree defaultOptionsNodeIn);

	/*
		Replaces data with user default options
	*/
	void resetToDefaultOptions(bool saveToSession, bool sendChange = true);

	/*
		Sets the session options node of the instance (but doesn't initialize or pass data down)
	*/
	void setSessionOptionsNode(ValueTree sessionOptionsNodeIn);

	/*
		Sets the session options node and loads the data
	*/
	void resetToSessionOptions(bool sendChangeMessage = true);

	/*
		Writes session data to plugin state
	*/
	void writeSessionDataToState();

	/*
		Passes data from input node, and falls back to defaultOptions if necessary
	*/
	void loadNodeOptions(ValueTree nodeOptionsIn);

	/*
		Sets and renders new tuning in definition, leaving tuning out unchanged
	*/
	void setTuningIn(ValueTree definitionIn, bool writeToSession = true, bool sendChangeSignal = false);

	/*
		Sets and renders new tuning out definition, leaving tuning in unchanged
	*/
	void setTuningOut(ValueTree definitionIn, bool writeToSession = true, bool sendChangeSignal = false);

	// SESSION PARAMETER SETTERS

	void setReferenceNoteIn(int noteIn);

	void setReferenceFreqIn(double freqIn);

	void setReferenceNoteOut(int noteIn);

	void setReferenceFreqOut(double freqIn);

	void setPitchbendRange(int newPitchBendRange);

	void setVoiceLimit(int limitIn);

	void setChannelMode(FreeChannelMode channelModeIn);

	void setReuseChannels(bool reuseChannels);

	void setResetChannelPitchbend(bool resetPitchbend);

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

	ValueTree pluginStateNode;
	ValueTree defaultOptionsNode;
	ValueTree sessionOptionsNode;

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

	// Session Options
	// TODO: generalize this and make it Common
	Array<Identifier> availableOptions =
	{
//		TuneUpIDs::defaultTuningFilePathId,
//		TuneUpIDs::tuningsListId,
		TuneUpIDs::referenceNoteInId,
		TuneUpIDs::referenceFreqInId,
		TuneUpIDs::referenceNoteOutId,
		TuneUpIDs::referenceFreqOutId,
		TuneUpIDs::pitchbendRangeId,
//		TuneUpIDs::channelConfigurationId,
		TuneUpIDs::channelModeId,
		TuneUpIDs::voiceLimitId,
		TuneUpIDs::reuseChannelsId,
		TuneUpIDs::resetChannelPitchbendId
	};


	// TODO: make a part of MidiCCListener interface
	// Midi Control number to function map
	//std::map<int, std::function<void(int)>> controlChangeMap;

};