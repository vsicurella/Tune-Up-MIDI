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

	newScaleButton.reset(new TextButton("New"));
	addAndMakeVisible(newScaleButton.get());
	newScaleButton->setButtonText(TRANS("New"));
	newScaleButton->addListener(this);

	loadScaleButton.reset(new TextButton("Load"));
	addAndMakeVisible(loadScaleButton.get());
	loadScaleButton->setButtonText(TRANS("Load"));
	loadScaleButton->addListener(this);

	viewButton.reset(new TextButton("View"));
	addAndMakeVisible(viewButton.get());
	viewButton->setButtonText(TRANS("View"));
	viewButton->addListener(this);

	optionsButton.reset(new TextButton("Options"));
	addAndMakeVisible(optionsButton.get());
	optionsButton->setButtonText(TRANS("Options"));
	optionsButton->addListener(this);

	dynamicToggleButton.reset(new TextButton("Dynamic"));
	addAndMakeVisible(dynamicToggleButton.get());
	dynamicToggleButton->setButtonText(TRANS("Dynamic"));
	dynamicToggleButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
	dynamicToggleButton->addListener(this);

	dynamicOptionsButton.reset(new TextButton("dynamicOptionsButton"));
	addAndMakeVisible(dynamicOptionsButton.get());
	dynamicOptionsButton->setButtonText("O");
	dynamicOptionsButton->setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
	dynamicOptionsButton->addListener(this);

	scaleNameLabel.reset(new Label(scaleTrans));
	addAndMakeVisible(scaleNameLabel.get());

	scaleSizeLabel.reset(new Label(sizeTrans));
	addAndMakeVisible(scaleSizeLabel.get());

	scalePeriodLabel.reset(new Label(periodTrans));
	addAndMakeVisible(scalePeriodLabel.get());
	
    descriptionTextBox.reset (new TextEditor ("descriptionTextBox"));
    addAndMakeVisible (descriptionTextBox.get());
    descriptionTextBox->setMultiLine (true);
    descriptionTextBox->setReturnKeyStartsNewLine (false);
    descriptionTextBox->setReadOnly (false);
    descriptionTextBox->setScrollbarsShown (true);
    descriptionTextBox->setCaretVisible (true);
    descriptionTextBox->setPopupMenuEnabled (true);  
    descriptionTextBox->setText (String());

	btnBar.setGapSize(stdGap);
	btnBar.addComponent(newScaleButton.get());
	btnBar.addComponent(loadScaleButton.get());
	btnBar.addComponent(viewButton.get());
	btnBar.addComponent(optionsButton.get(), 5);
	btnBar.addComponent(dynamicToggleButton.get(), 5);
	btnBar.addComponent(dynamicOptionsButton.get(), 2);

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
	btnBar.setBounds(8, proportionOfHeight(1 / 12.0f), btnBarWidth, proportionOfHeight(1 / 6.0f));

	newScaleButton->setBounds(btnBar.getComponentBounds(newScaleButton.get()).toNearestInt());
	loadScaleButton->setBounds(btnBar.getComponentBounds(loadScaleButton.get()).toNearestInt());
	viewButton->setBounds(btnBar.getComponentBounds(viewButton.get()).toNearestInt());
	optionsButton->setBounds(btnBar.getComponentBounds(optionsButton.get()).toNearestInt());
	dynamicToggleButton->setBounds(btnBar.getComponentBounds(dynamicToggleButton.get()).toNearestInt());
	dynamicOptionsButton->setBounds(btnBar.getComponentBounds(dynamicOptionsButton.get(), false).toNearestInt());

	labelSectHeight = proportionOfHeight(0.5f) - stdGap;

	descriptionTextBox->setBounds(proportionOfWidth(0.5f), proportionOfHeight(1 / 3.0f), proportionOfWidth(0.5f) - 8, labelSectHeight);

	scaleNameLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f), proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
	scaleSizeLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f) + labelSectHeight / 3.0f, proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
	scalePeriodLabel->setBounds(stdBorder, proportionOfHeight(1 / 3.0f) + labelSectHeight * 2 / 3.0f, proportionOfWidth(0.5f) - 8, labelSectHeight / 3.0f);
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

	else if (buttonThatWasClicked == newScaleButton.get())
	{
		listeners.call(&TuneUpWindow::Listener::newButtonClicked);
	}
}

ValueTree TuneUpWindow::getTuning()
{
	return stagedTuning;
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
		ValueTree definition = TuningDefinition::createStaticTuningDefinition(file.cents, 60, file.name, file.description);
		
		DBG("Staged Tuning: \n" + definition.toXmlString());
		listeners.call(&TuneUpWindow::Listener::scaleLoaded, definition);
	}
	else
	{
		//DialogWindow::LaunchOptions errorDialog;
		//errorDialog.content = OptionalScopedPointer<Component>(Label("msg", "Error loading scale file"));
		//errorDialog.launchAsync();
	}

}

void TuneUpWindow::loadTuning(Tuning* tuningIn)
{
	setScaleNameLabel(tuningIn->getName());
	setScaleSizeLabel(tuningIn->getTuningSize());
	setScalePeriodLabel(tuningIn->getPeriodCents());
	setDescription(tuningIn->getDescription());
}