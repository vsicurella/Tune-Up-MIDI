/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#include "MainWindow.h"


//==============================================================================
TuneUpWindow::TuneUpWindow ()
	: grid(6)
{
	tuningNameLabel.reset(new Label("tuningNameLabel", nameTrans + ":"));
	tuningNameLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningNameLabel.get());

	tuningNameBox.reset(new Label("tuningNameBox"));
	tuningNameBox->setJustificationType(Justification::centredLeft);
	addAndMakeVisible(tuningNameBox.get());

	tuningSizeLabel.reset(new Label("tuningSizeLabel", sizeTrans + ":"));
	tuningSizeLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningSizeLabel.get());

	tuningSizeBox.reset(new Label("tuningSizeBox"));
	tuningSizeBox->setJustificationType(Justification::centredLeft);
	addAndMakeVisible(tuningSizeBox.get());

	tuningPeriodLabel.reset(new Label("tuningPeriodLabel", periodTrans + ":"));
	tuningPeriodLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningPeriodLabel.get());

	tuningPeriodBox.reset(new Label("tuningPeriodBox"));
	tuningPeriodBox->setJustificationType(Justification::centredLeft);
	addAndMakeVisible(tuningPeriodBox.get());

    descriptionTextBox.reset (new TextEditor ("descriptionTextBox"));
    addAndMakeVisible (descriptionTextBox.get());
    descriptionTextBox->setMultiLine (true);
    descriptionTextBox->setReadOnly (true);
    descriptionTextBox->setScrollbarsShown (true);
    descriptionTextBox->setPopupMenuEnabled (true);
	descriptionTextBox->setTextToShowWhenEmpty(TRANS("No description"), Colours::darkgrey);

	for (auto str : { nameTrans, sizeTrans, periodTrans })
	{
		int width = font.getStringWidth(str + ":") + componentGap;
		if (width > valueLabelWidth)
			valueLabelWidth = width;
	}
}

TuneUpWindow::~TuneUpWindow()
{
	tuningNameLabel = nullptr;
	tuningNameBox = nullptr;
	tuningSizeLabel = nullptr;
	tuningSizeBox = nullptr;
	tuningPeriodLabel = nullptr;
	tuningPeriodBox = nullptr;
    descriptionTextBox = nullptr;
}

//==============================================================================
void TuneUpWindow::paint (Graphics& g)
{
	//g.setColour(Colours::blue);
	//g.drawRect(0, 0, getWidth(), getHeight());
}

void TuneUpWindow::resized()
{
	grid.setSize(getHeight());

	descriptionTextBox->setBounds(proportionOfWidth(0.5f), 0, proportionOfWidth(0.5f), getHeight());

	tuningNameLabel->setBounds(0, 0, valueLabelWidth, grid.getUnit(2) - componentGap);
	tuningNameBox->setBounds(valueLabelWidth, tuningNameLabel->getY(), descriptionTextBox->getX() - valueLabelWidth - componentGap, tuningNameLabel->getHeight());
	
	tuningSizeLabel->setBounds(0, grid.getUnit(2), valueLabelWidth, grid.getUnit(2) - componentGap);
	tuningSizeBox->setBounds(valueLabelWidth, tuningSizeLabel->getY(), descriptionTextBox->getX() - valueLabelWidth - componentGap, tuningSizeLabel->getHeight());

	tuningPeriodLabel->setBounds(0, grid.getUnit(4), valueLabelWidth, grid.getUnit(2) - componentGap);
	tuningPeriodBox->setBounds(valueLabelWidth, tuningPeriodLabel->getY(), descriptionTextBox->getX() - valueLabelWidth - componentGap, tuningPeriodLabel->getHeight());
}

void TuneUpWindow::loadOptionsNode(ValueTree optionsNodeIn)
{
	optionsNode = optionsNodeIn;
	updateTuningOutProperties();
}

void TuneUpWindow::updateTuningOutProperties()
{
	ValueTree tuningOutDefinition = optionsNode.getChildWithName(TuneUpIDs::tuningsListId).getChild(1);
	DBG("MAINWINDOW:\n" + optionsNode.toXmlString());
	setTuningNameLabel(tuningOutDefinition[TuneUpIDs::tuningNameId]);
	setTuningSizeLabel(tuningOutDefinition[TuneUpIDs::tuningSizeId]);
	setDescriptionText(tuningOutDefinition[TuneUpIDs::tuningDescriptionId]);

	// TODO: Add tuningPeriodId or abstract function
	double period = 0;
	if (tuningOutDefinition[TuneUpIDs::functionalId])
	{
		period = tuningOutDefinition.getChildWithName(TuneUpIDs::generatorListId).getChild(0)[TuneUpIDs::generatorValueId];
	}
	else
	{
		ValueTree intervalList = tuningOutDefinition.getChild(0);
		period = intervalList.getChild(intervalList.getNumChildren() - 1)[TuneUpIDs::intervalValueId];
	}
	
	setTuningPeriodLabel(period);
}

void TuneUpWindow::setTuningNameLabel(String nameIn)
{
	tuningNameBox->setText(nameIn, dontSendNotification);
}

void TuneUpWindow::setTuningSizeLabel(int numNotesIn)
{
	tuningSizeBox->setText(String(numNotesIn), dontSendNotification);
}

void TuneUpWindow::setTuningPeriodLabel(double periodIn)
{
	tuningPeriodBox->setText(String(periodIn), dontSendNotification);
}

void TuneUpWindow::setDescriptionText(String descIn)
{
	descriptionTextBox->setText(descIn);
}

