/*
  ==============================================================================

    ViewBase.h
    Created: 5 Aug 2020 11:18:33pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../CommonUI.h"

class TuneUpMidiView : public juce::Component, protected MidiKeyboardStateListener
{
public:

	enum ColourIds
	{
		backgroundColourId = 0x00010100,
		textDefaultColourId = 0x00010200
	};

public:

	TuneUpMidiView(const Tuning& tuningIn, MidiKeyboardState* inputStateIn);
	virtual ~TuneUpMidiView();

	virtual void paint(Graphics&) = 0;
	virtual void resized() = 0;

	virtual void updateNotesCache();

	void setFont(Font fontIn);

	//============================================================================================

	void handleNoteOn(MidiKeyboardState* source, int midiChannel, int noteNumber, float velocity) override;

	void handleNoteOff(MidiKeyboardState* source, int midiChannel, int noteNumber, float velocity) override;

protected:

	Font fontPrimary;
	Array<int> notesOn;

	const Tuning& tuning;
	MidiKeyboardState* midiInputState;
};