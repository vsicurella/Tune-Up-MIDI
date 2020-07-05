/*
  ==============================================================================

    TuneUpMidiState.cpp
    Created: 18 Jun 2020 1:48:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiState.h"

TuneUpMidiState::TuneUpMidiState()
{
	originTuning.reset(new Tuning(originTuningDefinition.render()));
	tuning.reset(new Tuning(tuningDefinition.render()));

	midiProcessor.reset(new TuneUpMidiProcessor(originTuning.get(), tuning.get(), notesInOn));

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

TuneUpMidiProcessor* TuneUpMidiState::getMidiProcessor()
{
	return midiProcessor.get();
}

ValueTree TuneUpMidiState::getOriginTuningDefinition()
{
	return originTuningDefinition.getDefinition();
}

Tuning* TuneUpMidiState::getOriginTuning()
{
	return originTuning.get();
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

void TuneUpMidiState::setNewTuning(ValueTree tuningDefinitionIn, bool sendChangeSignal)
{
	DBG("PLUGINSTATE RECEIVED TUNING:\n" + tuningDefinitionIn.toXmlString());

	if (tuningDefinitionIn.isValid())
	{
		DBG("NEW TUNING SET");
		tuningDefinition.setDefinition(tuningDefinitionIn, false);
		renderTuning();

		if (sendChangeSignal)
			sendChangeMessage();
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