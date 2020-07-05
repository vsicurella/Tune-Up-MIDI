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
#include "UI/CreateScaleWindow.h"
#include "UI/UnitGrid.h"

//==============================================================================
/**
*/
class TuneupMidiAudioProcessorEditor  :	public AudioProcessorEditor, 
										private TuneUpWindow::Listener,
										private CreateScaleWindow::Listener,
										private ChangeListener
{
public:
    TuneupMidiAudioProcessorEditor (TuneupMidiAudioProcessor&, TuneUpMidiProcessor&, TuneUpMidiState&);
    ~TuneupMidiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==============================================================================

	// TuneUpWindow Listener
	void scaleLoaded(ValueTree tuningDefinition) override;
	void newButtonClicked() override;
	void optionsButtonClicked() override;
	void dynamicOptionsClicked() override;

	// CreateNewScale Listener
	void scaleUpdated(ValueTree tuningDefinition) override;
	void saveButtonClicked() override;
	void backButtonClicked() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TuneupMidiAudioProcessor& processor;
	TuneUpMidiProcessor& midiProcessor;
	TuneUpMidiState& pluginState;

	std::unique_ptr<TuneUpWindow> mainWindow;
	std::unique_ptr<CreateScaleWindow> createScaleWindow;

	ValueTree lastTuningDefinition;

	UnitGrid unitGrid;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneupMidiAudioProcessorEditor)
};
