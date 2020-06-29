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

class TuneUpMidiState : public MidiCCListener
{
	MidiKeyboardState midiState;

	TuneUpMidiProcessor* midiProcessor;
	const Array<int> notesInOn;

	TuningDefinition tuningDefinition;
	std::unique_ptr<Tuning> tuning;

	// TODO: make a part of MidiCCListener interface
	// Midi Control number to function map
	//std::map<int, std::function<void(int)>> controlChangeMap;

public:

	TuneUpMidiState(TuneUpMidiProcessor* midiProcessorIn);
	~TuneUpMidiState();

	ValueTree getTuningDefinition();
	Tuning* getTuning();

	const MidiKeyboardState& getMidiKeyboardState();

	/*
		Sets and renders new tuning definition, without 
	*/
	void setNewTuning(ValueTree tuningDefinitionIn);

	void renderTuning();

	void setDynamicTuningPeriodController(int controlNumber);
	void setDynamicTuningGeneratorController(int controlNumber);

	// CC Listener Implementation
	void controlValueChanged(int controlNumber, int controlValue) override;
};