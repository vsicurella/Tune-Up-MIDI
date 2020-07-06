/*
  ==============================================================================

    GeneralOptionsWindow.cpp
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneralOptionsWindow.h"

GeneralOptionsWindow::GeneralOptionsWindow(ValueTree defaultOptions)
	: grid(12, 6)
{
	defaultTuningDirLabel.reset(new Label("defaultTuningDirLabel", "Default Tuning Directory:"));
	addAndMakeVisible(defaultTuningDirLabel.get());

	defaultTuningDirEditor.reset(new TextEditor("defaultTuningDirEditor"));
	addAndMakeVisible(defaultTuningDirEditor.get());
	defaultTuningDirEditor->setMultiLine(false);
	defaultTuningDirEditor->setReturnKeyStartsNewLine(false);

	defaultTuningDirButton.reset(new TextButton("defaultTuningDirButton", "Browse for default tuning file directory"));
	addAndMakeVisible(defaultTuningDirButton.get());
	defaultTuningDirButton->setButtonText("...");

	defaultTuningInLabel.reset(new Label("defaultTuningInLabel", "Default Tuning In:"));
	addAndMakeVisible(defaultTuningInLabel.get());

	defaultTuningInBox.reset(new ComboBox("defaultTuningInBox"));
	addAndMakeVisible(defaultTuningInBox.get());
	defaultTuningInBox->addItem("Standard Tuning (12EDO)", 1);
	defaultTuningInBox->addItem("Current Tuning...", 2);
	defaultTuningInBox->addItem("Browse For Tuning...", 3);

	defaultTuningOutLabel.reset(new Label("defaultTuningOutLabel", "Default Tuning Out:"));
	addAndMakeVisible(defaultTuningOutLabel.get());

	defaultTuningOutBox.reset(new ComboBox("defaultTuningOutBox"));
	addAndMakeVisible(defaultTuningOutBox.get());
	defaultTuningOutBox->addItem("Standard Tuning (12EDO)", 1);
	defaultTuningOutBox->addItem("Current Tuning...", 2);
	defaultTuningOutBox->addItem("Browse For Tuning...", 3);

	scaleInReferenceNoteLabel.reset(new Label("scaleInReferenceNoteLabel", "Tuning In Reference Note:"));
	addAndMakeVisible(scaleInReferenceNoteLabel.get());

	scaleInReferenceNoteSlider.reset(new Slider("scaleInReferenceNoteSlider"));
	addAndMakeVisible(scaleInReferenceNoteSlider.get());
	scaleInReferenceNoteSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceNoteSlider->setRange(0, 127, 1);

	scaleInReferenceFreqLabel.reset(new Label("scaleInReferenceFreqLabel", "Tuning In Reference Freq:"));
	addAndMakeVisible(scaleInReferenceFreqLabel.get());
	
	scaleInReferenceFreqSlider.reset(new Slider("scaleInReferenceFreqSlider"));
	addAndMakeVisible(scaleInReferenceFreqSlider.get());
	scaleInReferenceFreqSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceFreqSlider->setRange(20, 20000, 0.001);
	scaleInReferenceFreqSlider->setSkewFactor(2);

	scaleInReferenceFreqAutoButton.reset(new TextButton("scaleInReferenceFreqAutoButton", "Sets frequency based on standard tuning at Note 69=440Hz"));
	addAndMakeVisible(scaleInReferenceFreqAutoButton.get());
	scaleInReferenceFreqAutoButton->setButtonText("Auto");

	pitchbendRangeLabel.reset(new Label("pitchbendRangeLabel", "Pitchbend Range:"));
	addAndMakeVisible(pitchbendRangeLabel.get());

	pitchbendRangeSlider.reset(new Slider("pitchbendRangeSlider"));
	addAndMakeVisible(pitchbendRangeSlider.get());
	scaleInReferenceNoteSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceNoteSlider->setRange(1, 96, 1);

	channelControlLabel.reset(new Label("channelControlLabel", "Channels:"));
	addAndMakeVisible(channelControlLabel.get());

	channelController.reset(new Component("channelController"));
	//addAndMakeVisible(channelController.get());
	// TODO

	channelModeLabel.reset(new Label("channelModeLabel", "Free Channel Mode:"));
	addAndMakeVisible(channelModeLabel.get());

	channelModeBox.reset(new ComboBox("channelModeBox"));
	addAndMakeVisible(channelModeBox.get());
	channelModeBox->addItem("Round Robin (Cycle)", 1);
	channelModeBox->addItem("First Available", 2);

	voiceLimitLabel.reset(new Label("voiceLimitLabel", "Max Voices:"));
	addAndMakeVisible(voiceLimitLabel.get());

	voiceLimitSlider.reset(new Slider("voiceLimitSlider"));
	addAndMakeVisible(voiceLimitSlider.get());
	scaleInReferenceNoteSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceNoteSlider->setRange(1, 16, 1);

	reuseChannelsButton.reset(new ToggleButton("reuseChannelsButton"));
	addAndMakeVisible(reuseChannelsButton.get());

	resetChannelPitchbendButton.reset(new ToggleButton("resetChannelPitchbendButton"));
	addAndMakeVisible(resetChannelPitchbendButton.get());


	loadOptions(defaultOptions, true);
}

GeneralOptionsWindow::~GeneralOptionsWindow()
{
	 defaultTuningDirLabel = nullptr;
	 defaultTuningDirEditor = nullptr;
	 defaultTuningDirButton = nullptr;
	 defaultTuningInLabel = nullptr;
	 defaultTuningInBox = nullptr;
	 defaultTuningOutLabel = nullptr;
	 defaultTuningOutBox = nullptr;
	 scaleInReferenceNoteLabel = nullptr;
	 scaleInReferenceNoteSlider = nullptr;
	 scaleInReferenceFreqLabel = nullptr;
	 scaleInReferenceFreqSlider = nullptr;
	 scaleInReferenceFreqAutoButton = nullptr;
	 pitchbendRangeLabel = nullptr;
	 pitchbendRangeSlider = nullptr;
	 channelControlLabel = nullptr;
	 channelController = nullptr;
	 channelModeLabel = nullptr;
	 channelModeBox = nullptr;
	 voiceLimitLabel = nullptr;
	 voiceLimitSlider = nullptr;
	 reuseChannelsButton = nullptr;
	 resetChannelPitchbendButton = nullptr;
}

void GeneralOptionsWindow::paint(Graphics& g)
{

}

void GeneralOptionsWindow::resized()
{
	grid.setSize(getWidth(), getHeight());


}

void GeneralOptionsWindow::loadOptions(ValueTree optionsIn, bool factoryOptionIfEmpty)
{
	//defaultTuningDirEditor = nullptr;
	//defaultTuningInBox = nullptr;
	//defaultTuningOutBox = nullptr;
	//scaleInReferenceNoteSlider = nullptr;
	//scaleInReferenceFreqSlider = nullptr;
	//pitchbendRangeSlider = nullptr;
	//channelController = nullptr;
	//channelModeBox = nullptr;
	//voiceLimitSlider = nullptr;
	//reuseChannelsButton = nullptr;
	//resetChannelPitchbendButton = nullptr;
}


ValueTree GeneralOptionsWindow::getOptionsNode()
{
	return generalOptionsNode;
}
