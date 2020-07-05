/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#include "MainWindow.h"


//==============================================================================
TuneUpWindow::TuneUpWindow ()
	: grid(2, 6)
{
	tuningNameLabel.reset(new Label(nameTrans));
	//scaleNameLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningNameLabel.get());

	tuningSizeLabel.reset(new Label(sizeTrans));
	//scaleSizeLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningSizeLabel.get());

	tuningPeriodLabel.reset(new Label(periodTrans));
	//scalePeriodLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(tuningPeriodLabel.get());
	
    descriptionTextBox.reset (new TextEditor ("descriptionTextBox"));
    addAndMakeVisible (descriptionTextBox.get());
    descriptionTextBox->setMultiLine (true);
    descriptionTextBox->setReturnKeyStartsNewLine (false);
    descriptionTextBox->setReadOnly (true);
    descriptionTextBox->setScrollbarsShown (true);
    descriptionTextBox->setCaretVisible (true);
    descriptionTextBox->setPopupMenuEnabled (true);  
	descriptionTextBox->setTextToShowWhenEmpty(TRANS("No description"), Colours::darkgrey);

	//scaleNameLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//scaleSizeLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//scalePeriodLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);
}

TuneUpWindow::~TuneUpWindow()
{
	tuningNameLabel = nullptr;
	tuningSizeLabel = nullptr;
	tuningPeriodLabel = nullptr;
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
	grid.setSize(getWidth(), getHeight());

	descriptionTextBox->setBounds(grid.getX(1), 0, grid.getX(1), getHeight());

	tuningNameLabel->setBounds(0, 0, grid.getX(1), grid.getY(2) - stdGap);
	tuningSizeLabel->setBounds(0, grid.getY(2), grid.getX(1) - stdGap, grid.getY(2) - stdGap);
	tuningPeriodLabel->setBounds(0, grid.getY(4), grid.getX(1) - stdGap, grid.getY(2) - stdGap);
}

void TuneUpWindow::setTuningNameLabel(String scaleNameIn)
{
	tuningNameLabel->setText(nameTrans + ": " + scaleNameIn, dontSendNotification);
}

void TuneUpWindow::setTuningSizeLabel(int numNotesIn)
{
	tuningSizeLabel->setText(sizeTrans + ": " + String(numNotesIn), dontSendNotification);
}

void TuneUpWindow::setTuningPeriodLabel(double periodIn)
{
	tuningPeriodLabel->setText(periodTrans + ": " + String(periodIn), dontSendNotification);
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