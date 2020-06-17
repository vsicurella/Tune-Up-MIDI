/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TuneupMidiAudioProcessorEditor::TuneupMidiAudioProcessorEditor (TuneupMidiAudioProcessor& p, TuneUpMidiProcessor& mp)
    : AudioProcessorEditor (&p), processor (p), midiProcessor(&mp)
{
    
	gui.reset(new TuneUpWindow());
	addAndMakeVisible(gui.get());

    setSize (600, 400);

	gui->setLogs(midiProcessor->getMidiInLog(), midiProcessor->getMidiOutLog(), midiProcessor->getRetunerLog());
	gui->addChangeListener(this);

	midiProcessor->addChangeListener(this);
}

TuneupMidiAudioProcessorEditor::~TuneupMidiAudioProcessorEditor()
{
}

//==============================================================================
void TuneupMidiAudioProcessorEditor::paint (Graphics& g)
{
	
}

void TuneupMidiAudioProcessorEditor::resized()
{
	gui->setBounds(getBounds());
}

void TuneupMidiAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == gui.get())
	{
		midiProcessor->setTuning(gui->getTuning());
	}
	else if (source == midiProcessor)
	{
		gui->updateText();
	}
}