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
#include "DynamicTuning.h"

class TuneUpMidiState : public MidiCCListener
{
	std::unique_ptr<Tuning> tuning;
	MidiKeyboardState midiState;

	TuneUpMidiProcessor* midiProcessor;
	const Array<int> notesInOn;

	ValueTree tuningDefinition;

	// TODO: make a part of MidiCCListener interface
	// Midi Control number to function map
	std::map<int, std::function<void(int)>> controlChangeMap;

public:

	TuneUpMidiState(TuneUpMidiProcessor* midiProcessorIn);
	~TuneUpMidiState();

	Tuning* getTuning();

	const MidiKeyboardState& getMidiKeyboardState();

	void setTuning(ValueTree tuningDefinitionIn);

	void refreshTuning();

	void setDynamicTuningPeriodController(int controlNumber);
	void setDynamicTuningGeneratorController(int controlNumber);

	// CC Listener Implementation
	void controlValueChanged(int controlNumber, int controlValue) override;
};