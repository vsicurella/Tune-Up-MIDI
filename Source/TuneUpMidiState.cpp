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
	renderTuning();
	tuningDefinition.addListener(midiProcessor);

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

void TuneUpMidiState::setNewTuning(ValueTree tuningDefinitionIn)
{
	if (tuningDefinitionIn.isValid())
	{
		tuningDefinition.setDefinition(tuningDefinitionIn, false);
		renderTuning();
	}
}

void TuneUpMidiState::renderTuning()
{
	tuning.reset(new Tuning(tuningDefinition.render()));
	midiProcessor->setTuning(tuning.get());
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