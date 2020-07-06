/*
  ==============================================================================

    GeneralOptionsWindow.cpp
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneralOptionsWindow.h"

GeneralOptionsWindow::GeneralOptionsWindow(ValueTree defaultOptions)
	: grid(24, 6)
{
	defaultTuningDirLabel.reset(new Label("defaultTuningDirLabel", "Default Load Path:"));
	addAndMakeVisible(defaultTuningDirLabel.get());
	defaultTuningDirLabel->setJustificationType(Justification::centredRight);

	defaultTuningDirEditor.reset(new TextEditor("defaultTuningDirEditor"));
	addAndMakeVisible(defaultTuningDirEditor.get());
	defaultTuningDirEditor->setMultiLine(false);
	defaultTuningDirEditor->setReturnKeyStartsNewLine(false);

	defaultTuningDirButton.reset(new TextButton("defaultTuningDirButton", "Browse for default tuning file directory"));
	addAndMakeVisible(defaultTuningDirButton.get());
	defaultTuningDirButton->setButtonText("...");

	defaultTuningInLabel.reset(new Label("defaultTuningInLabel", "Default Tuning In:"));
	addAndMakeVisible(defaultTuningInLabel.get());
	defaultTuningInLabel->setJustificationType(Justification::centredRight);

	defaultTuningInBox.reset(new ComboBox("defaultTuningInBox"));
	addAndMakeVisible(defaultTuningInBox.get());
	defaultTuningInBox->addItem("Standard Tuning (12EDO)", 1);
	defaultTuningInBox->addItem("Current Tuning...", 2);
	defaultTuningInBox->addItem("Browse For Tuning...", 3);

	defaultTuningOutLabel.reset(new Label("defaultTuningOutLabel", "Default Tuning Out:"));
	addAndMakeVisible(defaultTuningOutLabel.get());
	defaultTuningOutLabel->setJustificationType(Justification::centredRight);

	defaultTuningOutBox.reset(new ComboBox("defaultTuningOutBox"));
	addAndMakeVisible(defaultTuningOutBox.get());
	defaultTuningOutBox->addItem("Standard Tuning (12EDO)", 1);
	defaultTuningOutBox->addItem("Current Tuning...", 2);
	defaultTuningOutBox->addItem("Browse For Tuning...", 3);

	scaleInReferenceNoteLabel.reset(new Label("scaleInReferenceNoteLabel", "Reference Note In:"));
	addAndMakeVisible(scaleInReferenceNoteLabel.get());
	scaleInReferenceNoteLabel->setJustificationType(Justification::centredRight);

	scaleInReferenceNoteSlider.reset(new Slider("scaleInReferenceNoteSlider"));
	addAndMakeVisible(scaleInReferenceNoteSlider.get());
	scaleInReferenceNoteSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceNoteSlider->setRange(0, 127, 1);

	scaleInReferenceFreqLabel.reset(new Label("scaleInReferenceFreqLabel", "Reference Freq In:"));
	addAndMakeVisible(scaleInReferenceFreqLabel.get());
	scaleInReferenceFreqLabel->setJustificationType(Justification::centredRight);
	
	scaleInReferenceFreqSlider.reset(new Slider("scaleInReferenceFreqSlider"));
	addAndMakeVisible(scaleInReferenceFreqSlider.get());
	scaleInReferenceFreqSlider->setSliderStyle(Slider::IncDecButtons);
	scaleInReferenceFreqSlider->setRange(20, 20000, 0.001);
	scaleInReferenceFreqSlider->setSkewFactor(2);

	scaleInReferenceFreqAutoButton.reset(new TextButton("scaleInReferenceFreqAutoButton", "Sets frequency based on standard tuning at Note 69=440Hz"));
	addAndMakeVisible(scaleInReferenceFreqAutoButton.get());
	scaleInReferenceFreqAutoButton->setButtonText("#");
	scaleInReferenceFreqAutoButton->onClick = [=]() {
		scaleInReferenceFreqSlider->setValue(pow(2, (scaleInReferenceNoteSlider->getValue() - 69) / 12.0) * 440);
	};

	pitchbendRangeLabel.reset(new Label("pitchbendRangeLabel", "Pitchbend Range:"));
	addAndMakeVisible(pitchbendRangeLabel.get());
	pitchbendRangeLabel->setJustificationType(Justification::centredRight);

	pitchbendRangeSlider.reset(new Slider("pitchbendRangeSlider"));
	addAndMakeVisible(pitchbendRangeSlider.get());
	pitchbendRangeSlider->setSliderStyle(Slider::IncDecButtons);
	pitchbendRangeSlider->setRange(1, 96, 1);

	channelControlLabel.reset(new Label("channelControlLabel", "Channels:"));
	addAndMakeVisible(channelControlLabel.get());
	channelControlLabel->setJustificationType(Justification::centredRight);

	channelController.reset(new Component("channelController"));
	//addAndMakeVisible(channelController.get());
	// TODO

	channelModeLabel.reset(new Label("channelModeLabel", "Free Channel Mode:"));
	addAndMakeVisible(channelModeLabel.get());
	channelModeLabel->setJustificationType(Justification::centredRight);

	channelModeBox.reset(new ComboBox("channelModeBox"));
	addAndMakeVisible(channelModeBox.get());
	channelModeBox->addItem("Round Robin (Cycle)", 1);
	channelModeBox->addItem("First Available", 2);

	voiceLimitLabel.reset(new Label("voiceLimitLabel", "Max Voices:"));
	addAndMakeVisible(voiceLimitLabel.get());
	voiceLimitLabel->setJustificationType(Justification::centredRight);

	voiceLimitSlider.reset(new Slider("voiceLimitSlider"));
	addAndMakeVisible(voiceLimitSlider.get());
	voiceLimitSlider->setSliderStyle(Slider::IncDecButtons);
	voiceLimitSlider->setRange(1, 16, 1);

	reuseChannelsButton.reset(new ToggleButton("Reuse Channels When Possible"));
	addAndMakeVisible(reuseChannelsButton.get());
	

	resetChannelPitchbendButton.reset(new ToggleButton("Reset Empty Channel Pitchbend"));
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
	g.setColour(Colours::green);
	g.drawRect(0, 0, getWidth(), getHeight());
}

void GeneralOptionsWindow::resized()
{
	grid.setSize(getWidth(), getHeight());

	int rightHalfX = grid.getX(12);
	int yStep = 1;

	int leftLabelWidth = grid.getX(6);
	int rightLabelWidth = grid.getX(5);
	int rightControlWidth = grid.getX(12);
	int leftControlWidth = rightControlWidth - stdGap;
	int buttonWidth = grid.getX(1);

	int rowHeight = grid.getY(1) - 1;


	// Left half
	defaultTuningDirLabel->setBounds(0, 0, leftLabelWidth, rowHeight);
	defaultTuningDirEditor->setBounds(defaultTuningDirLabel->getRight(), defaultTuningDirLabel->getY(), leftControlWidth - defaultTuningDirLabel->getWidth() - buttonWidth, rowHeight);
	defaultTuningDirButton->setBounds(defaultTuningDirEditor->getRight(), defaultTuningDirLabel->getY(), buttonWidth, rowHeight);
	defaultTuningInLabel->setBounds(0, grid.getY(yStep * 1), leftLabelWidth, rowHeight);
	defaultTuningInBox->setBounds(defaultTuningInLabel->getRight(), defaultTuningInLabel->getY(), leftControlWidth - defaultTuningInLabel->getWidth(), rowHeight);
	defaultTuningOutLabel->setBounds(0, grid.getY(yStep * 2), leftLabelWidth, rowHeight);
	defaultTuningOutBox->setBounds(defaultTuningOutLabel->getRight(), defaultTuningOutLabel->getY(), leftControlWidth - defaultTuningOutLabel->getWidth(), rowHeight);
	scaleInReferenceNoteLabel->setBounds(0, grid.getY(yStep * 3), leftLabelWidth, rowHeight);
	scaleInReferenceNoteSlider->setBounds(scaleInReferenceNoteLabel->getRight(), scaleInReferenceNoteLabel->getY(), leftControlWidth - scaleInReferenceNoteLabel->getWidth(), scaleInReferenceNoteLabel->getHeight());
	scaleInReferenceFreqLabel->setBounds(0, grid.getY(yStep * 4), leftLabelWidth, rowHeight);
	scaleInReferenceFreqSlider->setBounds(scaleInReferenceFreqLabel->getRight(), scaleInReferenceFreqLabel->getY(), leftControlWidth - scaleInReferenceFreqLabel->getWidth() - buttonWidth, rowHeight);
	scaleInReferenceFreqAutoButton->setBounds(scaleInReferenceFreqSlider->getRight(), scaleInReferenceFreqLabel->getY(), buttonWidth, rowHeight);

	// Right half
	pitchbendRangeLabel->setBounds(rightHalfX, defaultTuningDirLabel->getY(), rightLabelWidth, rowHeight);
	pitchbendRangeSlider->setBounds(pitchbendRangeLabel->getRight(), 0, rightControlWidth - pitchbendRangeLabel->getWidth(), rowHeight);
	channelControlLabel->setBounds(rightHalfX, defaultTuningInLabel->getY(), rightLabelWidth, rowHeight);
	channelController->setBounds(channelControlLabel->getRight(), channelControlLabel->getY(), rightControlWidth - channelControlLabel->getWidth(), rowHeight);
	channelModeLabel->setBounds(rightHalfX, defaultTuningOutLabel->getY(), rightLabelWidth, rowHeight);
	channelModeBox->setBounds(channelModeLabel->getRight(), channelModeLabel->getY(), rightControlWidth - channelModeLabel->getWidth(), rowHeight);
	voiceLimitLabel->setBounds(rightHalfX, scaleInReferenceNoteLabel->getY(), rightLabelWidth, rowHeight);
	voiceLimitSlider->setBounds(voiceLimitLabel->getRight(), voiceLimitLabel->getY(), rightControlWidth - voiceLimitLabel->getWidth(), rowHeight);
	reuseChannelsButton->setBounds(rightHalfX, scaleInReferenceFreqLabel->getY(), rightControlWidth, rowHeight);
	resetChannelPitchbendButton->setBounds(rightHalfX, grid.getY(yStep * 5), rightControlWidth, rowHeight);
}

void GeneralOptionsWindow::loadOptions(ValueTree optionsIn, bool factoryOptionIfEmpty)
{
	//defaultTuningDirEditor = nullptr;
	if (false /* TODO */)
		void;
	else
		void;

	//defaultTuningInBox = nullptr;
	if (false /* TODO */)
		void; 
	else
		defaultTuningInBox->setSelectedId(1);

	//defaultTuningOutBox = nullptr;
	if (false /* TODO */)
		void;
	else
		defaultTuningOutBox->setSelectedId(1);

	//scaleInReferenceNoteSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		scaleInReferenceNoteSlider->setValue(69);
	
	//scaleInReferenceFreqSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		scaleInReferenceFreqSlider->setValue(440.0);

	//pitchbendRangeSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		pitchbendRangeSlider->setValue(2);

	//channelController = nullptr;
	if (false /* TODO */)
		void;
	else
		void;
	
	//channelModeBox = nullptr;
	if (false /* TODO */)
		void;
	else
		channelModeBox->setSelectedId(1);

	//voiceLimitSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		voiceLimitSlider->setValue(16);
	
	//reuseChannelsButton = nullptr;
	if (false /* TODO */)
		void;
	else
		reuseChannelsButton->setToggleState(false, dontSendNotification);

	//resetChannelPitchbendButton = nullptr;
	if (false /* TODO */)
		void;
	else
		resetChannelPitchbendButton->setToggleState(true, dontSendNotification);
}


ValueTree GeneralOptionsWindow::getOptionsNode()
{
	return generalOptionsNode;
}
