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
	scaleNameLabel.reset(new Label(scaleTrans));
	//scaleNameLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(scaleNameLabel.get());

	scaleSizeLabel.reset(new Label(sizeTrans));
	//scaleSizeLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(scaleSizeLabel.get());

	scalePeriodLabel.reset(new Label(periodTrans));
	//scalePeriodLabel->setJustificationType(Justification::centredRight);
	addAndMakeVisible(scalePeriodLabel.get());
	
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
	scaleNameLabel = nullptr;
	scaleSizeLabel = nullptr;
	scalePeriodLabel = nullptr;
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

	scaleNameLabel->setBounds(0, 0, grid.getX(1), grid.getY(2) - stdGap);
	scaleSizeLabel->setBounds(0, grid.getY(2), grid.getX(1) - stdGap, grid.getY(2) - stdGap);
	scalePeriodLabel->setBounds(0, grid.getY(4), grid.getX(1) - stdGap, grid.getY(2) - stdGap);
}

void TuneUpWindow::setScaleNameLabel(String scaleNameIn)
{
	scaleNameLabel->setText(scaleTrans + ": " + scaleNameIn, dontSendNotification);
}

void TuneUpWindow::setScaleSizeLabel(int numNotesIn)
{
	scaleSizeLabel->setText(sizeTrans + ": " + String(numNotesIn), dontSendNotification);
}

void TuneUpWindow::setScalePeriodLabel(double periodIn)
{
	scalePeriodLabel->setText(periodTrans + ": " + String(periodIn), dontSendNotification);
}

void TuneUpWindow::setDescription(String descIn)
{
	descriptionTextBox->setText(descIn);
}

void TuneUpWindow::loadTuning(Tuning* tuningIn)
{
	setScaleNameLabel(tuningIn->getName());
	setScaleSizeLabel(tuningIn->getTuningSize());
	setScalePeriodLabel(tuningIn->getPeriodCents());
	setDescription(tuningIn->getDescription());
}