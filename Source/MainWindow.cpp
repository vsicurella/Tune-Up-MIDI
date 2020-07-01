/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#include "MainWindow.h"


//==============================================================================
TuneUpWindow::TuneUpWindow ()
{

	newScaleButton.reset(new TextButton("newScaleButton"));
	addAndMakeVisible(newScaleButton.get());
	newScaleButton->setButtonText(TRANS("New"));
	newScaleButton->addListener(this);

	loadScaleButton.reset(new TextButton("loadScaleButton"));
	addAndMakeVisible(loadScaleButton.get());
	loadScaleButton->setButtonText(TRANS("Load"));
	loadScaleButton->addListener(this);

	viewButton.reset(new TextButton("viewButton"));
	addAndMakeVisible(viewButton.get());
	viewButton->setButtonText(TRANS("View"));
	viewButton->addListener(this);

	optionsButton.reset(new TextButton("optionsButton"));
	addAndMakeVisible(optionsButton.get());
	optionsButton->setButtonText(TRANS("Options"));
	optionsButton->addListener(this);

	loadScaleButton.reset(new TextButton("loadScaleButton"));
	addAndMakeVisible(loadScaleButton.get());
	loadScaleButton->setButtonText(TRANS("Load"));
	loadScaleButton->addListener(this);

	scaleNameLabel.reset(new Label(scaleTrans));
	addAndMakeVisible(scaleNameLabel.get());
	
	scaleSizeLabel.reset(new Label(sizeTrans));
	addAndMakeVisible(scaleSizeLabel.get());

	scalePeriodLabel.reset(new Label(periodTrans));
	addAndMakeVisible(scalePeriodLabel.get());

	dynamicToggleButton.reset(new TextButton("dynamicToggleButton"));
	addAndMakeVisible(dynamicToggleButton.get());
	dynamicToggleButton->setButtonText(TRANS("Dynamic"));
	dynamicToggleButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
	dynamicToggleButton->addListener(this);

	dynamicOptionsButton.reset(new TextButton("dynamicOptionsButton"));
	addAndMakeVisible(dynamicOptionsButton.get());
	dynamicOptionsButton->setButtonText("O");
	dynamicOptionsButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
	dynamicOptionsButton->addListener(this);
	
    descriptionTextBox.reset (new TextEditor ("descriptionTextBox"));
    addAndMakeVisible (descriptionTextBox.get());
    descriptionTextBox->setMultiLine (true);
    descriptionTextBox->setReturnKeyStartsNewLine (false);
    descriptionTextBox->setReadOnly (false);
    descriptionTextBox->setScrollbarsShown (true);
    descriptionTextBox->setCaretVisible (true);
    descriptionTextBox->setPopupMenuEnabled (true);  
    descriptionTextBox->setText (String());

	// TEMP
	dynamicToggleButton->setEnabled(false);
	dynamicOptionsButton->setEnabled(false);

	//scaleNameLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//scaleSizeLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);
	//scalePeriodLabel->setColour(Label::ColourIds::outlineColourId, Colours::red);

}

TuneUpWindow::~TuneUpWindow()
{
	newScaleButton = nullptr;
	loadScaleButton = nullptr;
	viewButton = nullptr;
	optionsButton = nullptr;
	dynamicToggleButton = nullptr;
	dynamicOptionsButton = nullptr;
	scaleNameLabel = nullptr;
	scaleSizeLabel = nullptr;
	scalePeriodLabel = nullptr;
    descriptionTextBox = nullptr;
}

//==============================================================================
void TuneUpWindow::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));

	// bottom bar background
	g.setColour(Colour(0xff323e44).darker(0.75f));
	g.fillRect(0, proportionOfHeight(5 / 6.0f), getWidth(), proportionOfHeight(1 / 6.0f));
}

void TuneUpWindow::resized()
{
	btnBarWidth = getWidth() - stdGap * 4 - stdBorder * 2;

	newScaleButton->setBounds(stdBorder, proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[0] / btnUnitDenom, proportionOfHeight(1 / 6.0f));
	loadScaleButton->setBounds(newScaleButton->getRight() + stdGap, proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[1] / btnUnitDenom, proportionOfHeight(1 / 6.0f));
	viewButton->setBounds(loadScaleButton->getRight() + stdGap, proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[2] / btnUnitDenom, proportionOfHeight(1 / 6.0f));
	optionsButton->setBounds(viewButton->getRight() + stdGap, proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[3] / btnUnitDenom, proportionOfHeight(1 / 6.0f));
	dynamicToggleButton->setBounds(optionsButton->getRight() + stdGap, proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[4] / btnUnitDenom, proportionOfHeight(1 / 6.0f));
	dynamicOptionsButton->setBounds(dynamicToggleButton->getRight(), proportionOfHeight(1 / 12.0f), btnBarWidth * btnUnit[5] / btnUnitDenom, proportionOfHeight(1 / 6.0f));

	labelSectHeight = proportionOfHeight(0.5f) - stdGap;

	scaleNameLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f), proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
	scaleSizeLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f) + labelSectHeight / 3.0f, proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
	scalePeriodLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f) + labelSectHeight * 2 / 3.0f, proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
	descriptionTextBox->setBounds(proportionOfWidth(0.5f), proportionOfHeight(1 / 3.0f), proportionOfWidth(0.5f) - 8, labelSectHeight);
}

void TuneUpWindow::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == loadScaleButton.get())
    {
		FileChooser chooser("Select your tuning file", File::getSpecialLocation(File::userDocumentsDirectory));
		chooser.browseForFileToOpen();
		loadedFile = chooser.getResult();

		onFileLoad();
    }
}

void TuneUpWindow::sliderValueChanged (Slider* sliderThatWasMoved)
{

}


ValueTree TuneUpWindow::getTuning()
{
	return stagedTuning;
}

Value* TuneUpWindow::getPitchbendRange()
{
	return &pitchbendRange;
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

void TuneUpWindow::onFileLoad()
{
	bool success = scalaFileReader.open(loadedFile);

	if (success)
	{
		ScalaFile file = scalaFileReader.getScalaFile();
		scaleName = file.name;
		scaleDescription = file.description;

		stagedTuning = TuningDefinition::createStaticTuningDefinition(file.cents, 60, file.description);
		DBG("Staged Tuning: \n" + stagedTuning.toXmlString());
		
		sendChangeMessage();
	}

}

void TuneUpWindow::loadTuning(Tuning* tuningIn)
{
	setScaleNameLabel(scaleName);
	setScaleSizeLabel(tuningIn->getTuningSize());
	setScalePeriodLabel(tuningIn->getPeriodCents());
	setDescription(scaleDescription);
}