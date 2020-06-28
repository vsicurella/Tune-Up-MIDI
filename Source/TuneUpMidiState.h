/*
  ==============================================================================

    TuneUpMidiState.h
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "TuneUpMidiProcessor.h"
#include "DynamicTuning.h"

class TuneUpMidiState
{
	std::unique_ptr<Tuning> tuning;
	MidiKeyboardState midiState;

	TuneUpMidiProcessor* midiProcessor;
	const Array<int> notesInOn;

	ValueTree tuningDefinition;


public:

	TuneUpMidiState(TuneUpMidiProcessor* midiProcessorIn);
	~TuneUpMidiState();

	Tuning* getTuning();

	const MidiKeyboardState& getMidiKeyboardState();

	void setTuning(ValueTree tuningDefinitionIn);

	void refreshTuning();
};