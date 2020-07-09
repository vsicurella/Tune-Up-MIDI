/*
  ==============================================================================

    GeneralOptionsWindow.cpp
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneralOptionsWindow.h"

GeneralOptionsWindow::GeneralOptionsWindow()
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
	defaultTuningDirButton->addListener(this);

	defaultTuningInLabel.reset(new Label("defaultTuningInLabel", "Default Tuning In:"));
	addAndMakeVisible(defaultTuningInLabel.get());
	defaultTuningInLabel->setJustificationType(Justification::centredRight);

	defaultTuningInBox.reset(new ComboBox("defaultTuningInBox"));
	addAndMakeVisible(defaultTuningInBox.get());
	defaultTuningInBox->addItem(standardTrans, 1);
	defaultTuningInBox->addItem(currentTrans, 2);
	defaultTuningInBox->addItem(browseTrans, 3);
	defaultTuningInBox->addListener(this);

	defaultTuningOutLabel.reset(new Label("defaultTuningOutLabel", "Default Tuning Out:"));
	addAndMakeVisible(defaultTuningOutLabel.get());
	defaultTuningOutLabel->setJustificationType(Justification::centredRight);

	defaultTuningOutBox.reset(new ComboBox("defaultTuningOutBox"));
	addAndMakeVisible(defaultTuningOutBox.get());
	defaultTuningOutBox->addItem(standardTrans, 1);
	defaultTuningOutBox->addItem(currentTrans, 2);
	defaultTuningOutBox->addItem(browseTrans, 3);
	defaultTuningOutBox->addListener(this);

	referenceNoteInLabel.reset(new Label("referenceNoteInLabel", "Reference Note In:"));
	addAndMakeVisible(referenceNoteInLabel.get());
	referenceNoteInLabel->setJustificationType(Justification::centredRight);

	referenceNoteInSlider.reset(new Slider("referenceNoteInSlider"));
	addAndMakeVisible(referenceNoteInSlider.get());
	referenceNoteInSlider->setSliderStyle(Slider::IncDecButtons);
	referenceNoteInSlider->setRange(0, 127, 1);
	referenceNoteInSlider->addListener(this);

	referenceFreqInLabel.reset(new Label("referenceFreqInLabel", "Reference Freq In:"));
	addAndMakeVisible(referenceFreqInLabel.get());
	referenceFreqInLabel->setJustificationType(Justification::centredRight);
	
	referenceFreqInSlider.reset(new Slider("referenceFreqInSlider"));
	addAndMakeVisible(referenceFreqInSlider.get());
	referenceFreqInSlider->setSliderStyle(Slider::IncDecButtons);
	referenceFreqInSlider->setRange(20, 20000, 0.001);
	referenceFreqInSlider->setSkewFactor(2);
	referenceFreqInSlider->addListener(this);

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
	referenceNoteOutSlider->addListener(this);

	referenceFreqOutLabel.reset(new Label("referenceFreqOutLabel", "Reference Freq In:"));
	addAndMakeVisible(referenceFreqOutLabel.get());
	referenceFreqOutLabel->setJustificationType(Justification::centredRight);

	referenceFreqOutSlider.reset(new Slider("referenceFreqOutSlider"));
	addAndMakeVisible(referenceFreqOutSlider.get());
	referenceFreqOutSlider->setSliderStyle(Slider::IncDecButtons);
	referenceFreqOutSlider->setRange(20, 20000, 0.001);
	referenceFreqOutSlider->setSkewFactor(2);
	referenceFreqOutSlider->addListener(this);

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
	pitchbendRangeSlider->addListener(this);

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
	channelModeBox->addListener(this);

	voiceLimitLabel.reset(new Label("voiceLimitLabel", "Voice Limit:"));
	addAndMakeVisible(voiceLimitLabel.get());
	voiceLimitLabel->setJustificationType(Justification::centredRight);

	voiceLimitSlider.reset(new Slider("voiceLimitSlider"));
	addAndMakeVisible(voiceLimitSlider.get());
	voiceLimitSlider->setSliderStyle(Slider::IncDecButtons);
	voiceLimitSlider->setRange(1, 16, 1);
	voiceLimitSlider->addListener(this);

	reuseChannelsButton.reset(new ToggleButton(reuseTrans));
	addAndMakeVisible(reuseChannelsButton.get());
	reuseChannelsButton->addListener(this);
	
	resetChannelPitchbendButton.reset(new ToggleButton(resetTrans));
	addAndMakeVisible(resetChannelPitchbendButton.get());
	resetChannelPitchbendButton->addListener(this);

	sessionOptionsNode = ValueTree(sessionOptionsNodeId);
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

void GeneralOptionsWindow::buttonClicked(Button* btnClicked)
{
	if (btnClicked == defaultTuningDirButton.get())
	{
		// TODO : browse for directory
	}

	else if (btnClicked == reuseChannelsButton.get())
	{
		setReuseChannels(reuseChannelsButton->getToggleState(), false);
	}

	else if (btnClicked == resetChannelPitchbendButton.get())
	{
		setResetChannelPitchbend(resetChannelPitchbendButton->getToggleState(), false);
	}
}

void GeneralOptionsWindow::comboBoxChanged(ComboBox* comboBoxThatChanged)
{
	if (comboBoxThatChanged == defaultTuningInBox.get())
	{
		// TODO
	}

	else if (comboBoxThatChanged == defaultTuningOutBox.get())
	{
		// TODO
	}

	else if (comboBoxThatChanged == channelModeBox.get())
	{
		setChannelMode(FreeChannelMode(channelModeBox->getSelectedId()), false);
	}
}

void GeneralOptionsWindow::sliderValueChanged(Slider* sliderThatChanged)
{
	if (sliderThatChanged == referenceNoteInSlider.get())
	{
		setReferenceNoteIn(referenceNoteInSlider->getValue(), false);
	}

	else if (sliderThatChanged == referenceFreqInSlider.get())
	{
		setReferenceFreqIn(referenceFreqInSlider->getValue(), false);
	}

	else if (sliderThatChanged == referenceNoteOutSlider.get())
	{
		setReferenceNoteOut(referenceNoteOutSlider->getValue(), false);
	}

	else if (sliderThatChanged == referenceFreqOutSlider.get())
	{
		setReferenceFreqOut(referenceFreqOutSlider->getValue(), false);
	}

	else if (sliderThatChanged == pitchbendRangeSlider.get())
	{
		setPitchbendRange(pitchbendRangeSlider->getValue(), false);
	}

	else if (sliderThatChanged == voiceLimitSlider.get())
	{
		setVoiceLimit(voiceLimitSlider->getValue(), false);
	}
}

bool GeneralOptionsWindow::initializeOptions(ValueTree optionsIn)
{
	bool synced = true;

	if (optionsIn.hasType(TuneUpIDs::defaultOptionsNodeId))
		defaultOptionsNode = optionsIn;
	else
	{
		defaultOptionsNode = ValueTree(TuneUpIDs::defaultOptionsNodeId);
		synced = false;
	}

	ensureNodeHasTuningListNode(defaultOptionsNode);
	defaultTuningsListNode = defaultOptionsNode.getChildWithName(TuneUpIDs::tuningsListId);

	bool hasProperty;
	var value;

	for (auto prop : availableOptions)
	{
		hasProperty = defaultOptionsNode.hasProperty(prop);
		value = defaultOptionsNode[prop];

		// ONLY DEFAULT OPTIONS

		if (prop == TuneUpIDs::defaultTuningFilePathId)
		{
			if (!hasProperty)
				value = File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName();
			
			setDefaultTuningPath(value);
		}

		else if (prop == TuneUpIDs::tuningsListId)
		{
			if (defaultTuningsListNode.getNumChildren() > 0)
			{
				ValueTree dftTun = defaultTuningsListNode.getChild(0);

				if (dftTun.hasType(TuneUpIDs::tuningDefinitionId))
					setTuningIn(dftTun);
				else
					setTuningIn(STD_TUNING);

				dftTun = defaultTuningsListNode.getChild(1);
				
				if (dftTun.hasType(TuneUpIDs::tuningDefinitionId))
					setTuningOut(dftTun);
				else
					setTuningOut(STD_TUNING);
			}

			else
			{
				defaultTuningsListNode.addChild(STD_TUNING, 0, nullptr);
				defaultTuningsListNode.addChild(STD_TUNING, 1, nullptr);
			}

		}

		// OPTIONS THAT CAN BE BOTH DEFAULT OR SESSION

		else if (prop == TuneUpIDs::referenceNoteInId)
		{
			if (!hasProperty)
				value = 69;

			setReferenceNoteIn(value, true, true);
		}

		else if (prop == TuneUpIDs::referenceFreqInId)
		{
			if (!hasProperty)
				value = 440.0;

			setReferenceFreqIn(value, true, true);
		}

		else if (prop == TuneUpIDs::referenceNoteOutId)
		{
			if (!hasProperty)
				value = 60;

			setReferenceNoteOut(value, true, true);
		}

		else if (prop == TuneUpIDs::referenceFreqOutId)
		{
			if (!hasProperty)
				value = 261.626;

			setReferenceFreqOut(value, true, true);
		}

		else if (prop == TuneUpIDs::pitchbendRangeId)
		{
			if (!hasProperty)
				value = 2;

			setPitchbendRange(value, true, true);
		}

		else if (prop == TuneUpIDs::voiceLimitId)
		{
			if (!hasProperty)
				value = 16;

			setVoiceLimit(value, true, true);
		}

		else if (prop == TuneUpIDs::channelConfigurationId)
		{
			if (!hasProperty)
				value = 2;

			// TODO
		}

		else if (prop == TuneUpIDs::channelModeId)
		{
			if (!hasProperty)
				value = 1;

			setChannelMode(FreeChannelMode((int)value), true, true);
		}

		else if (prop == TuneUpIDs::reuseChannelsId)
		{
			if (!hasProperty)
				value = false;

			setReuseChannels(value, true, true);
		}

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
		{
			if (!hasProperty)
				value = true;

			setResetChannelPitchbend(value, true, true);
		}
	}

	return synced;
}

void GeneralOptionsWindow::loadSessionOptions(ValueTree sessionOptionsIn)
{
	for (auto prop : availableOptions)
	{
		if (!sessionOptionsIn.hasProperty(prop))
			continue;

		if (prop == TuneUpIDs::referenceNoteInId)
			setReferenceNoteIn(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::referenceFreqInId)
			setReferenceFreqIn(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::referenceNoteOutId)
			setReferenceNoteOut(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::referenceFreqOutId)
			setReferenceFreqOut(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::pitchbendRangeId)
			setPitchbendRange(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::voiceLimitId)
			setVoiceLimit(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::channelConfigurationId)
			setChannelConfiguration(/*TODO*/);

		else if (prop == TuneUpIDs::channelModeId)
			setChannelMode(FreeChannelMode((int)sessionOptionsIn[prop]));

		else if (prop == TuneUpIDs::reuseChannelsId)
			setReuseChannels(sessionOptionsIn[prop]);

		else if (prop == TuneUpIDs::resetChannelPitchbendId)
			setResetChannelPitchbend(sessionOptionsIn[prop]);
	}
}

ValueTree GeneralOptionsWindow::getDefaultOptionsNode()
{
	return defaultOptionsNode;
}

ValueTree GeneralOptionsWindow::getSessionOptionsNode()
{
	return sessionOptionsNode;
}

void GeneralOptionsWindow::setDefaultTuningPath(String absolutePathIn, bool notifyListeners)
{
	defaultTuningDirEditor->setText(absolutePathIn);
	defaultOptionsNode.setProperty(TuneUpIDs::defaultTuningFilePathId, absolutePathIn, nullptr);

	if (notifyListeners)
		void; // TODO
}

void GeneralOptionsWindow::setTuningIn(ValueTree tuningInPath, bool notifyListeners)
{
	if (defaultTuningsListNode.getNumChildren() > 0)
		defaultTuningsListNode.getChild(0).copyPropertiesAndChildrenFrom(tuningInPath, nullptr);
	else
		defaultTuningsListNode.addChild(tuningInPath, 0, nullptr);

	if (notifyListeners)
		void; // TODO
}

void GeneralOptionsWindow::setTuningOut(ValueTree tuningOutPath, bool notifyListeners)
{
	if (defaultTuningsListNode.getNumChildren() == 0)
		defaultTuningsListNode.addChild(STD_TUNING, 0, nullptr);

	if (defaultTuningsListNode.getNumChildren() == 1)
		defaultTuningsListNode.addChild(tuningOutPath, 1, nullptr);
	else 
		defaultTuningsListNode.getChild(1).copyPropertiesAndChildrenFrom(tuningOutPath, nullptr);

	if (notifyListeners)
		void; // TODO
}

void GeneralOptionsWindow::setReferenceNoteIn(int noteIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteInId, noteIn, nullptr);

	if (updateUI)
		referenceNoteInSlider->setValue(noteIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::referenceNoteInId, noteIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::referenceNoteInChanged, noteIn);
}

void GeneralOptionsWindow::setReferenceFreqIn(double freqIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqInId, freqIn, nullptr);

	if (updateUI)
		referenceFreqInSlider->setValue(freqIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::referenceFreqInId, freqIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::referenceFreqInChanged, freqIn);
}

void GeneralOptionsWindow::setReferenceNoteOut(int noteIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::referenceNoteOutId, noteIn, nullptr);

	if (updateUI)
		referenceNoteOutSlider->setValue(noteIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::referenceNoteOutId, noteIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::referenceNoteOutChanged, noteIn);
}

void GeneralOptionsWindow::setReferenceFreqOut(double freqIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::referenceFreqOutId, freqIn, nullptr);

	if (updateUI)
		referenceFreqOutSlider->setValue(freqIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::referenceFreqOutId, freqIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::referenceFreqOutChanged, freqIn);
}

void GeneralOptionsWindow::setPitchbendRange(int pitchbendRangeIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::pitchbendRangeId, pitchbendRangeIn, nullptr);

	if (updateUI)
		pitchbendRangeSlider->setValue(pitchbendRangeIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::pitchbendRangeId, pitchbendRangeIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::pitchbendRangeChanged, pitchbendRangeIn);
}

void GeneralOptionsWindow::setChannelConfiguration(/* TODO */ bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	/* TODO */
	//generalOptionsNode.setProperty(TuneUpIDs::channelConfigurationId, , nullptr);
}

void GeneralOptionsWindow::setChannelMode(TuneUpMode::FreeChannelMode modeIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::channelModeId, modeIn, nullptr);

	if (updateUI)
		channelModeBox->setSelectedId(modeIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::channelModeId, modeIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::channelModeChanged, modeIn);
}

void GeneralOptionsWindow::setVoiceLimit(int limitIn, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::voiceLimitId, limitIn, nullptr);

	if (updateUI)
		voiceLimitSlider->setValue(limitIn, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::voiceLimitId, limitIn, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::voiceLimitChanged, limitIn);
}

void GeneralOptionsWindow::setResetChannelPitchbend(bool resetChannels, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::resetChannelPitchbendId, resetChannels, nullptr);

	if (updateUI)
		resetChannelPitchbendButton->setToggleState(resetChannels, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::resetChannelPitchbendId, resetChannels, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::resetChannelPitchbendChanged, resetChannels);
}

void GeneralOptionsWindow::setReuseChannels(bool reuseChannels, bool updateUI, bool saveAsDefault, bool notifyListeners)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::reuseChannelsId, reuseChannels, nullptr);

	if (updateUI)
		reuseChannelsButton->setToggleState(reuseChannels, dontSendNotification);

	if (saveAsDefault)
		defaultOptionsNode.setProperty(TuneUpIDs::reuseChannelsId, reuseChannels, nullptr);

	if (notifyListeners)
		listeners.call(&GeneralOptionsWindow::Listener::reuseChannelsChanged, reuseChannels);
}

void GeneralOptionsWindow::setDynamicTuningMode(bool isDynamicTuning)
{
	//sessionOptionsNode.setProperty(TuneUpIDs::dynamicTuningModeId, isDynamicTuning, nullptr);
	listeners.call(&GeneralOptionsWindow::Listener::dynamicTuningModeChanged, isDynamicTuning);
}

void GeneralOptionsWindow::ensureNodeHasTuningListNode(ValueTree nodeIn)
{
	if (!nodeIn.getChildWithName(TuneUpIDs::tuningsListId).isValid())
		nodeIn.addChild(ValueTree(TuneUpIDs::tuningsListId), -1, nullptr);
}