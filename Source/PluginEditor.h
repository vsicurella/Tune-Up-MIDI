/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "UI/MainWindow.h"

//==============================================================================
/**
*/
class TuneupMidiAudioProcessorEditor  : public AudioProcessorEditor, public ChangeListener, public Value::Listener
{
public:
    TuneupMidiAudioProcessorEditor (TuneupMidiAudioProcessor&, TuneUpMidiProcessor&, TuneUpMidiState&);
    ~TuneupMidiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void valueChanged(Value& value) override;

	//==============================================================================

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TuneupMidiAudioProcessor& processor;
	TuneUpMidiProcessor& midiProcessor;
	TuneUpMidiState& pluginState;

	std::unique_ptr<TuneUpWindow> gui;

	Value* pitchbendRange = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneupMidiAudioProcessorEditor)
};
