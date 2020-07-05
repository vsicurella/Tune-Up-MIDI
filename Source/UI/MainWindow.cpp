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

	//tuningNameBox->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//tuningSizeBox->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//tuningPeriodBox->setColour(Label::ColourIds::outlineColourId, Colours::red);

	for (auto str : { nameTrans, sizeTrans, periodTrans })
	{
		int width = font.getStringWidth(str + ":") + stdGap;
		if (width > labelWidth)
			labelWidth = width;
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

	tuningNameLabel->setBounds(0, 0, labelWidth, grid.getUnit(2) - stdGap);
	tuningNameBox->setBounds(labelWidth, tuningNameLabel->getY(), descriptionTextBox->getX() - labelWidth - stdGap, tuningNameLabel->getHeight());
	
	tuningSizeLabel->setBounds(0, grid.getUnit(2), labelWidth, grid.getUnit(2) - stdGap);
	tuningSizeBox->setBounds(labelWidth, tuningSizeLabel->getY(), descriptionTextBox->getX() - labelWidth - stdGap, tuningSizeLabel->getHeight());

	tuningPeriodLabel->setBounds(0, grid.getUnit(4), labelWidth, grid.getUnit(2) - stdGap);
	tuningPeriodBox->setBounds(labelWidth, tuningPeriodLabel->getY(), descriptionTextBox->getX() - labelWidth - stdGap, tuningPeriodLabel->getHeight());
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

void TuneUpWindow::setDescription(String descIn)
{
	descriptionTextBox->setText(descIn);
}

void TuneUpWindow::loadTuning(Tuning* tuningIn)
{
	setTuningNameLabel(tuningIn->getName());
	setTuningSizeLabel(tuningIn->getTuningSize());
	setTuningPeriodLabel(tuningIn->getPeriodCents());
	setDescription(tuningIn->getDescription());
}