/*
  ==============================================================================

    ViewBase.cpp
    Created: 5 Aug 2020 11:18:33pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ViewBase.h"

TuneUpMidiView::TuneUpMidiView(const Tuning& tuningIn, MidiKeyboardState* inputStateIn)
	: tuning(tuningIn), midiInputState(inputStateIn)
{
	setColour(ColourIds::backgroundColourId, Colours::darkslategrey.darker(0.8f));
	setColour(ColourIds::textDefaultColourId, Colours::lightgoldenrodyellow);

	midiInputState->addListener(this);
	updateNotesCache();
}

TuneUpMidiView::~TuneUpMidiView()
{
	midiInputState->removeListener(this);
}

void TuneUpMidiView::updateNotesCache()
{
	notesOn.clear();

	for (int n = 0; n < 128; n++)
	{
		if (midiInputState->isNoteOnForChannels(0xFFFF, n))
		{
			notesOn.add(n);
			continue;
		}
	}
}

void TuneUpMidiView::setFont(Font fontIn)
{
	fontPrimary = fontIn;
}

void TuneUpMidiView::handleNoteOn(MidiKeyboardState* source, int midiChannel, int noteNumber, float velocity)
{
	notesOn.add(noteNumber);
}

void TuneUpMidiView::handleNoteOff(MidiKeyboardState* source, int midiChannel, int noteNumber, float velocity)
{
	notesOn.removeAllInstancesOf(noteNumber);
}