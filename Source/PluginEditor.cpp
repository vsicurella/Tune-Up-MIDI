/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TuneupMidiAudioProcessorEditor::TuneupMidiAudioProcessorEditor (
	TuneupMidiAudioProcessor& p, 
	TuneUpMidiProcessor& mp, 
	TuneUpMidiState& ps
)
    : AudioProcessorEditor (&p), processor (p), midiProcessor(mp), pluginState(ps)
{
    
	gui.reset(new TuneUpWindow());
	addAndMakeVisible(gui.get());

    setSize (500, 200);

	gui->addChangeListener(this);

	pitchbendRange = gui->getPitchbendRange();
	if (pitchbendRange)
	{
		pitchbendRange->addListener(this);
	}

	//midiProcessor.addChangeListener(this);

	if (pluginState.getTuning())
	{
		midiProcessor.setTuning(pluginState.getTuning());
		gui->loadTuning(pluginState.getTuning());
	}
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
	// Tuning defined
	if (source == gui.get())
	{
		pluginState.setNewTuning(gui->getTuning());
		gui->loadTuning(pluginState.getTuning());
	}
}

void TuneupMidiAudioProcessorEditor::valueChanged(Value& value)
{
	if (value.refersToSameSourceAs(*pitchbendRange))
	{
		midiProcessor.setPitchbendRange(pitchbendRange->getValue());
	}
}
