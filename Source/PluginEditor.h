/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MainWindow.h"

//==============================================================================
/**
*/
class TuneupMidiAudioProcessorEditor  : public AudioProcessorEditor, public ChangeListener
{
public:
    TuneupMidiAudioProcessorEditor (TuneupMidiAudioProcessor&, TuneUpMidiProcessor&);
    ~TuneupMidiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TuneupMidiAudioProcessor& processor;
	TuneUpMidiProcessor* midiProcessor;

	std::unique_ptr<TuneUpWindow> gui;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneupMidiAudioProcessorEditor)
};
