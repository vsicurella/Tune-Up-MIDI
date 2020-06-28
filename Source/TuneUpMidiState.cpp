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
