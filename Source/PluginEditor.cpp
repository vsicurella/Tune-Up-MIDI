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
    : AudioProcessorEditor (&p), processor (p), midiProcessor(mp), pluginState(ps),
	unitGrid(2, 13)
{
    
	mainWindow.reset(new TuneUpWindow());
	addAndMakeVisible(mainWindow.get());
	mainWindow->addListener(this);

	createScaleWindow.reset(new CreateScaleWindow(pluginState.getTuningDefinition()));
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
	// bottom bar background
	g.setColour(Colour(0xff323e44).darker(0.75f));
	g.fillRect(0, unitGrid.getY(11), getWidth(), unitGrid.getY(2));
}

void TuneupMidiAudioProcessorEditor::resized()
{
	unitGrid.setSize(getWidth(), getHeight());

	mainWindow->setBoundsInset(BorderSize<int>(8));
	createScaleWindow->setBoundsInset(BorderSize<int>(8));
}

void TuneupMidiAudioProcessorEditor::scaleLoaded(ValueTree tuningDefinition)
{
	pluginState.setNewTuning(tuningDefinition);
	mainWindow->loadTuning(pluginState.getTuning());
}

// TuneUpWindow Listener
void TuneupMidiAudioProcessorEditor::newButtonClicked()
{
	midiProcessor.resetNotes();

	// backup current scale definition
	lastTuningDefinition = pluginState.getTuningDefinition().createCopy();

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
	scaleLoaded(tuningDefinition);
}

void TuneupMidiAudioProcessorEditor::saveButtonClicked()
{
	createScaleWindow->setVisible(false);
	mainWindow->setVisible(true);
}

void TuneupMidiAudioProcessorEditor::backButtonClicked()
{
	// Reset tuning
	scaleLoaded(lastTuningDefinition);

	createScaleWindow->setVisible(false);
	mainWindow->setVisible(true);
}

