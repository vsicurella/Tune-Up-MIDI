/*
  ==============================================================================

    GeneralOptionsWindow.cpp
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneralOptionsWindow.h"

GeneralOptionsWindow::GeneralOptionsWindow(ValueTree defaultOptions)
	: grid(24, 14)
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
	defaultTuningInBox->addItem(standardTrans, 1);
	defaultTuningInBox->addItem(currentTrans, 2);
	defaultTuningInBox->addItem(browseTrans, 3);

	defaultTuningOutLabel.reset(new Label("defaultTuningOutLabel", "Default Tuning Out:"));
	addAndMakeVisible(defaultTuningOutLabel.get());
	defaultTuningOutLabel->setJustificationType(Justification::centredRight);

	defaultTuningOutBox.reset(new ComboBox("defaultTuningOutBox"));
	addAndMakeVisible(defaultTuningOutBox.get());
	defaultTuningOutBox->addItem(standardTrans, 1);
	defaultTuningOutBox->addItem(currentTrans, 2);
	defaultTuningOutBox->addItem(browseTrans, 3);

	referenceNoteInLabel.reset(new Label("referenceNoteInLabel", "Reference Note In:"));
	addAndMakeVisible(referenceNoteInLabel.get());
	referenceNoteInLabel->setJustificationType(Justification::centredRight);

	referenceNoteInSlider.reset(new Slider("referenceNoteInSlider"));
	addAndMakeVisible(referenceNoteInSlider.get());
	referenceNoteInSlider->setSliderStyle(Slider::IncDecButtons);
	referenceNoteInSlider->setRange(0, 127, 1);

	referenceFreqInLabel.reset(new Label("referenceFreqInLabel", "Reference Freq In:"));
	addAndMakeVisible(referenceFreqInLabel.get());
	referenceFreqInLabel->setJustificationType(Justification::centredRight);
	
	referenceFreqInSlider.reset(new Slider("referenceFreqInSlider"));
	addAndMakeVisible(referenceFreqInSlider.get());
	referenceFreqInSlider->setSliderStyle(Slider::IncDecButtons);
	referenceFreqInSlider->setRange(20, 20000, 0.001);
	referenceFreqInSlider->setSkewFactor(2);

	referenceFreqInAutoBtn.reset(new TextButton("referenceFreqInAutoBtn", "Sets frequency based on standard tuning at Note 69=440Hz"));
	addAndMakeVisible(referenceFreqInAutoBtn.get());
	referenceFreqInAutoBtn->setButtonText("#");
	referenceFreqInAutoBtn->onClick = [=]() {
		referenceFreqInSlider->setValue(getStandardTuningFrequency(referenceNoteInSlider->getValue()));
	};

	referenceNoteOutLabel.reset(new Label("referenceNoteOutLabel", "Reference Note Out:"));
	addAndMakeVisible(referenceNoteOutLabel.get());
	referenceNoteOutLabel->setJustificationType(Justification::centredRight);

	referenceNoteOutSlider.reset(new Slider("referenceNoteOutSlider"));
	addAndMakeVisible(referenceNoteOutSlider.get());
	referenceNoteOutSlider->setSliderStyle(Slider::IncDecButtons);
	referenceNoteOutSlider->setRange(0, 127, 1);

	referenceFreqOutLabel.reset(new Label("referenceFreqOutLabel", "Reference Freq In:"));
	addAndMakeVisible(referenceFreqOutLabel.get());
	referenceFreqOutLabel->setJustificationType(Justification::centredRight);

	referenceFreqOutSlider.reset(new Slider("referenceFreqOutSlider"));
	addAndMakeVisible(referenceFreqOutSlider.get());
	referenceFreqOutSlider->setSliderStyle(Slider::IncDecButtons);
	referenceFreqOutSlider->setRange(20, 20000, 0.001);
	referenceFreqOutSlider->setSkewFactor(2);

	referenceFreqOutAutoBtn.reset(new TextButton("referenceFreqInAutoBtn", "Sets frequency based on standard tuning at Note 69=440Hz"));
	addAndMakeVisible(referenceFreqOutAutoBtn.get());
	referenceFreqOutAutoBtn->setButtonText("#");
	referenceFreqOutAutoBtn->onClick = [=]() {
		referenceFreqOutSlider->setValue(getStandardTuningFrequency(referenceNoteOutSlider->getValue()));
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

	voiceLimitLabel.reset(new Label("voiceLimitLabel", "Voice Limit:"));
	addAndMakeVisible(voiceLimitLabel.get());
	voiceLimitLabel->setJustificationType(Justification::centredRight);

	voiceLimitSlider.reset(new Slider("voiceLimitSlider"));
	addAndMakeVisible(voiceLimitSlider.get());
	voiceLimitSlider->setSliderStyle(Slider::IncDecButtons);
	voiceLimitSlider->setRange(1, 16, 1);

	reuseChannelsButton.reset(new ToggleButton(reuseTrans));
	addAndMakeVisible(reuseChannelsButton.get());
	

	resetChannelPitchbendButton.reset(new ToggleButton(resetTrans));
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
	 referenceNoteInLabel = nullptr;
	 referenceNoteInSlider = nullptr;
	 referenceFreqInLabel = nullptr;
	 referenceFreqInSlider = nullptr;
	 referenceFreqInAutoBtn = nullptr;
	 referenceNoteOutLabel = nullptr;
	 referenceNoteOutSlider = nullptr;
	 referenceFreqOutLabel = nullptr;
	 referenceFreqOutSlider = nullptr;
	 referenceFreqOutAutoBtn = nullptr;
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
	//g.setColour(Colours::green);
	//g.drawRect(0, 0, getWidth(), getHeight());
}

void GeneralOptionsWindow::resized()
{
	grid.setSize(getWidth(), getHeight());

	int leftHalfX = 0;
	int rightHalfX = grid.getX(12);
	int yStep = 2;

	int leftLabelWidth = grid.getX(6);
	int rightLabelWidth = grid.getX(5);
	int rightControlWidth = grid.getX(12);
	int leftControlWidth = rightControlWidth - stdGap / 2;
	int buttonWidth = grid.getX(1);

	int rowHeight = grid.getY(2) - 1;

	// Left half
	defaultTuningDirLabel->setBounds(leftHalfX, 0, leftLabelWidth, rowHeight);
	defaultTuningDirEditor->setBounds(defaultTuningDirLabel->getRight(), defaultTuningDirLabel->getY(), leftControlWidth - defaultTuningDirLabel->getWidth() - buttonWidth, rowHeight);
	defaultTuningDirButton->setBounds(defaultTuningDirEditor->getRight(), defaultTuningDirLabel->getY(), buttonWidth, rowHeight);
	defaultTuningInLabel->setBounds(leftHalfX, grid.getY(yStep * 1), leftLabelWidth, rowHeight);
	defaultTuningInBox->setBounds(defaultTuningInLabel->getRight(), defaultTuningInLabel->getY(), leftControlWidth - defaultTuningInLabel->getWidth(), rowHeight);
	defaultTuningOutLabel->setBounds(leftHalfX, grid.getY(yStep * 2), leftLabelWidth, rowHeight);
	defaultTuningOutBox->setBounds(defaultTuningOutLabel->getRight(), defaultTuningOutLabel->getY(), leftControlWidth - defaultTuningOutLabel->getWidth(), rowHeight);
	referenceNoteInLabel->setBounds(leftHalfX, grid.getY(yStep * 3), leftLabelWidth, rowHeight);
	referenceNoteInSlider->setBounds(referenceNoteInLabel->getRight(), referenceNoteInLabel->getY(), leftControlWidth - referenceNoteInLabel->getWidth(), referenceNoteInLabel->getHeight());
	referenceFreqInLabel->setBounds(leftHalfX, grid.getY(yStep * 4), leftLabelWidth, rowHeight);
	referenceFreqInSlider->setBounds(referenceFreqInLabel->getRight(), referenceFreqInLabel->getY(), leftControlWidth - referenceFreqInLabel->getWidth() - buttonWidth, rowHeight);
	referenceFreqInAutoBtn->setBounds(referenceFreqInSlider->getRight(), referenceFreqInLabel->getY(), buttonWidth, rowHeight);
	referenceNoteOutLabel->setBounds(leftHalfX, grid.getY(yStep * 5), leftLabelWidth, rowHeight);
	referenceNoteOutSlider->setBounds(referenceNoteOutLabel->getRight(), referenceNoteOutLabel->getY(), leftControlWidth - referenceNoteOutLabel->getWidth(), referenceNoteOutLabel->getHeight());
	referenceFreqOutLabel->setBounds(leftHalfX, grid.getY(yStep * 6), leftLabelWidth, rowHeight);
	referenceFreqOutSlider->setBounds(referenceFreqOutLabel->getRight(), referenceFreqOutLabel->getY(), leftControlWidth - referenceFreqOutLabel->getWidth() - buttonWidth, rowHeight);
	referenceFreqOutAutoBtn->setBounds(referenceFreqOutSlider->getRight(), referenceFreqOutLabel->getY(), buttonWidth, rowHeight);

	// Right half
	pitchbendRangeLabel->setBounds(rightHalfX, defaultTuningDirLabel->getY(), rightLabelWidth, rowHeight);
	pitchbendRangeSlider->setBounds(pitchbendRangeLabel->getRight(), 0, rightControlWidth - pitchbendRangeLabel->getWidth(), rowHeight);
	voiceLimitLabel->setBounds(rightHalfX, defaultTuningInLabel->getY(), rightLabelWidth, rowHeight);
	voiceLimitSlider->setBounds(voiceLimitLabel->getRight(), voiceLimitLabel->getY(), rightControlWidth - voiceLimitLabel->getWidth(), rowHeight);
	channelModeLabel->setBounds(rightHalfX, defaultTuningOutLabel->getY(), rightLabelWidth, rowHeight);
	channelModeBox->setBounds(channelModeLabel->getRight(), channelModeLabel->getY(), rightControlWidth - channelModeLabel->getWidth(), rowHeight);
	channelControlLabel->setBounds(rightHalfX, referenceNoteInLabel->getY(), rightLabelWidth, rowHeight);
	channelController->setBounds(channelControlLabel->getRight(), channelControlLabel->getY(), rightControlWidth - channelControlLabel->getWidth(), rowHeight);
	reuseChannelsButton->setBounds(rightHalfX, referenceNoteOutLabel->getY(), rightControlWidth, rowHeight);
	resetChannelPitchbendButton->setBounds(rightHalfX, referenceFreqOutLabel->getY(), rightControlWidth, rowHeight);
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
		referenceNoteInSlider->setValue(69);
	
	//scaleInReferenceFreqSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		referenceFreqInSlider->setValue(440.0);

	//scaleInReferenceNoteSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		referenceNoteOutSlider->setValue(60);

	//scaleInReferenceFreqSlider = nullptr;
	if (false /* TODO */)
		void;
	else
		referenceFreqOutSlider->setValue(261.626);


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
