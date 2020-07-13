/*
  ==============================================================================

    CreateScaleWindow.cpp
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "CreateScaleWindow.h"

CreateTuningWindow::CreateTuningWindow(ValueTree tuningDefinitionIn)
	: grid(6)
{
	tuningNameLabel.reset(new Label("scaleNameLabel", nameTrans + ':'));
	tuningNameLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningNameLabel.get());

	tuningNameBox.reset(new Label("scaleNameBox"));
	addAndMakeVisible(tuningNameBox.get());
	tuningNameBox->setJustificationType(Justification::centredLeft);
	tuningNameBox->setColour(Label::outlineColourId, Colours::black);
	tuningNameBox->setEditable(true);
	tuningNameBox->addListener(this);

	descriptionBox.reset(new TextEditor("descriptionBox"));
	addAndMakeVisible(descriptionBox.get());
	descriptionBox->setReturnKeyStartsNewLine(true);
	descriptionBox->setMultiLine(true);
	descriptionBox->setTextToShowWhenEmpty(TRANS("Tuning description"), Colours::darkgrey);

	etNotesLabel.reset(new Label("etNotesLabel", sizeTrans + ':'));
	etNotesLabel->setJustificationType(Justification::centredRight);
	addChildComponent(etNotesLabel.get());

	etNotesSlider.reset(new Slider("etNotesSlider"));
	addChildComponent(etNotesSlider.get());
	etNotesSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
	etNotesSlider->setRange(1, 128, 1);
	etNotesSlider->setValue(12, dontSendNotification);
	etNotesSlider->addListener(this);

	etPeriodLabel.reset(new Label("etPeriodLabel", periodTrans + ':'));
	etPeriodLabel->setJustificationType(Justification::centredRight);
	addChildComponent(etPeriodLabel.get());
	
	etPeriodBox.reset(new Label("etPeriodBox", "2/1"));
	addChildComponent(etPeriodBox.get());
	etPeriodBox->setJustificationType(Justification::centredLeft);
	etPeriodBox->setColour(Label::outlineColourId, Colours::black);
	etPeriodBox->setEditable(true);
	etPeriodBox->addListener(this);

	generatorTable.reset(new RegularTemperamentTable(tuningDefinition));
	addChildComponent(generatorTable.get());
	generatorTable->addChangeListener(this);

	tuningDefinition = tuningDefinitionIn;

	for (auto str : { nameTrans, sizeTrans, periodTrans })
	{
		int width = font.getStringWidth(str + ":") + componentGap;
		if (width > valueLabelWidth)
			valueLabelWidth = width;
	}
}

CreateTuningWindow::~CreateTuningWindow()
{
	tuningNameLabel = nullptr;
	tuningNameBox = nullptr;
	descriptionBox = nullptr;
	etNotesLabel = nullptr;
	etNotesSlider = nullptr;
	etPeriodLabel = nullptr;
	etPeriodBox = nullptr;
	generatorTable = nullptr;
}


void CreateTuningWindow::paint(Graphics& g)
{
	//g.setColour(Colours::red);
	//g.drawRect(0, 0, getWidth(), getHeight());
}

void CreateTuningWindow::resized() 
{
	grid.setSize(getHeight());

	descriptionBox->setBounds(proportionOfWidth(0.5f), 0, proportionOfWidth(0.5f), getHeight());

	tuningNameLabel->setBounds(0, 0, valueLabelWidth, grid.getUnit(2) - componentGap);
	tuningNameBox->setBounds(valueLabelWidth, tuningNameLabel->getY(), descriptionBox->getX() - valueLabelWidth - componentGap, tuningNameLabel->getHeight());
	
	etNotesLabel->setBounds(0, grid.getUnit(2), valueLabelWidth, grid.getUnit(2) - componentGap);
	etNotesSlider->setBounds(valueLabelWidth, etNotesLabel->getY(), descriptionBox->getX() - valueLabelWidth - componentGap, etNotesLabel->getHeight());
	
	etPeriodLabel->setBounds(0, grid.getUnit(4), valueLabelWidth, grid.getUnit(2) - componentGap);
	etPeriodBox->setBounds(valueLabelWidth, etPeriodLabel->getY(), descriptionBox->getX() - valueLabelWidth - componentGap, etPeriodLabel->getHeight());

	generatorTable->setBounds(0, grid.getUnit(2), proportionOfWidth(0.5f) - componentGap, getHeight() - grid.getUnit(2));
}

void CreateTuningWindow::editorShown(Label* source, TextEditor& editor)
{
	editor.setJustification(Justification::centredLeft);
}

void CreateTuningWindow::labelTextChanged(Label* labelThatChanged) 
{
	if (labelThatChanged == tuningNameBox.get())
	{
	}
	
	else if (labelThatChanged == etPeriodBox.get())
	{
	}

	updateTuning();
}

void CreateTuningWindow::sliderValueChanged(Slider* sliderThatChanged) 
{
	if (sliderThatChanged == etNotesSlider.get())
	{
	}

	updateTuning();
}

void CreateTuningWindow::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == generatorTable.get())
	{
		updateTuning();
	}
}

void CreateTuningWindow::setMode(CreateTuningMode modeIn)
{
	mode = modeIn;
	
	generatorTable->setVisible(mode == RegularTemperament);
	etNotesLabel->setVisible(mode == EqualTemperament);
	etNotesSlider->setVisible(mode == EqualTemperament);
	etPeriodLabel->setVisible(mode == EqualTemperament);
	etPeriodBox->setVisible(mode == EqualTemperament);
}

void CreateTuningWindow::setDefinition(ValueTree definition)
{
	tuningDefinition = definition;
	generatorTable->updateDefinition(tuningDefinition);
}

void CreateTuningWindow::updateTuning(bool sendChange)
{
	if (mode == EqualTemperament)
	{
		tuningDefinition = TuningDefinition::createEqualTemperamentDefinition(
			etNotesSlider->getValue(),
			etPeriodBox->getText(),
			60,
			tuningNameBox->getText(),
			descriptionBox->getText()
		);

		generatorTable->updateDefinition(tuningDefinition);
	}

	else if (mode == RegularTemperament)
	{
		tuningDefinition = generatorTable->getDefinition();

		String name = tuningNameBox->getText();
		if (name.length() > 0)
			tuningDefinition.setProperty(TuneUpIDs::tuningNameId, name, nullptr);

		String desc = descriptionBox->getText();
		if (desc.length() > 0)
			tuningDefinition.setProperty(TuneUpIDs::tuningDescriptionId, desc, nullptr);
	}
	
	if (sendChange)
		sendChangeMessage();
}

ValueTree CreateTuningWindow::getTuningDefinition() const
{
	return tuningDefinition;
}

