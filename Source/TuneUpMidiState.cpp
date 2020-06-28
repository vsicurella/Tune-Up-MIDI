/*
  ==============================================================================

    TuneUpMidiState.cpp
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiState.h"

TuneUpMidiState::TuneUpMidiState(TuneUpMidiProcessor* midiProcessorIn)
	: midiProcessor(midiProcessorIn), notesInOn(midiProcessorIn->getTuningNotesOn())
{
	// TODO: Move this to MIDICCListener interface
	// fill control change map with empty functions
	for (int cc = 0; cc < 128; cc++)
	{
		controlChangeMap.emplace(cc, std::function<void(void)>());
	}

	midiProcessor->addControlListener(this);
}

TuneUpMidiState::~TuneUpMidiState()
{
	tuning = nullptr;
}

Tuning* TuneUpMidiState::getTuning()
{
	return tuning.get();
}

const MidiKeyboardState& TuneUpMidiState::getMidiKeyboardState()
{
	return midiState;
}

void TuneUpMidiState::setTuning(ValueTree tuningDefinitionIn)
{
	if (tuningDefinitionIn.isValid())
	{
		tuningDefinition = tuningDefinitionIn;
		refreshTuning();
	}
}

void TuneUpMidiState::refreshTuning()
{
	if (tuningDefinition.isValid())
	{
		if (tuningDefinition.getChildWithName(Tuning::centsTableID).isValid())
		{
			tuning.reset(new Tuning(tuningDefinition));
		}
		else
		{
			tuning.reset(new DynamicTuning(tuningDefinition));
		}
	}
	else
	{
		tuning.reset(new Tuning());
	}
}

void TuneUpMidiState::setDynamicTuningPeriodController(int controlNumber)
{
	
}

void TuneUpMidiState::setDynamicTuningGeneratorController(int controlNumber)
{

}