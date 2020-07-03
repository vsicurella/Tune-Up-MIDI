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
    
	mainWindow.reset(new TuneUpWindow());
	addAndMakeVisible(mainWindow.get());
	mainWindow->addListener(this);

	createScaleWindow.reset(new CreateScaleWindow());
	addChildComponent(createScaleWindow.get());
	createScaleWindow->addListener(this);

    setSize (500, 200);

	//midiProcessor.addChangeListener(this);

	if (pluginState.getTuning())
	{
		midiProcessor.setTuning(pluginState.getTuning());
		mainWindow->loadTuning(pluginState.getTuning());
	}
}

TuneupMidiAudioProcessorEditor::~TuneupMidiAudioProcessorEditor()
{
	mainWindow = nullptr;
	createScaleWindow = nullptr;
}

//==============================================================================
void TuneupMidiAudioProcessorEditor::paint (Graphics& g)
{
	
}

void TuneupMidiAudioProcessorEditor::resized()
{
	mainWindow->setBounds(getBounds());
	createScaleWindow->setBounds(getBounds());
}

void TuneupMidiAudioProcessorEditor::scaleLoaded(ValueTree tuningDefinition)
{
	pluginState.setNewTuning(tuningDefinition);
	mainWindow->loadTuning(pluginState.getTuning());
}

void TuneupMidiAudioProcessorEditor::newButtonClicked()
{
	// backup current scale definition
	lastTuningDefinition = pluginState.getTuningDefinition();

	mainWindow->setVisible(false);
	createScaleWindow->setVisible(true);
}

void TuneupMidiAudioProcessorEditor::optionsButtonClicked()
{

}

void TuneupMidiAudioProcessorEditor::dynamicOptionsClicked()
{

}


// CreateNewScale Listener
void TuneupMidiAudioProcessorEditor::scaleUpdated(ValueTree tuningDefinition)
{
	pluginState.setNewTuning(tuningDefinition);
}

void TuneupMidiAudioProcessorEditor::saveButtonClicked()
{
	// Confirm tuning
	mainWindow->loadTuning(pluginState.getTuning());

	createScaleWindow->setVisible(false);
	mainWindow->setVisible(true);
}

void TuneupMidiAudioProcessorEditor::backButtonClicked()
{
	// Reset tuning
	pluginState.setNewTuning(lastTuningDefinition);

	createScaleWindow->setVisible(false);
	mainWindow->setVisible(true);
}

