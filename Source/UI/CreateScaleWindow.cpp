/*
  ==============================================================================

    CreateScaleWindow.cpp
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "CreateScaleWindow.h"

CreateScaleWindow::CreateScaleWindow(ValueTree tuningDefinitionIn)
{
	backButton.reset(new TextButton(backTrans));
	addAndMakeVisible(backButton.get());
	backButton->setButtonText(backTrans);
	backButton->addListener(this);

	saveButton.reset(new TextButton(saveTrans));
	addAndMakeVisible(saveButton.get());
	saveButton->setButtonText(saveTrans);
	saveButton->addListener(this);

	viewButton.reset(new TextButton(viewTrans));
	addAndMakeVisible(viewButton.get());
	viewButton->setButtonText(viewTrans);
	viewButton->addListener(this);

	modeBox.reset(new ComboBox("ModeBox"));
	addAndMakeVisible(modeBox.get());
	modeBox->addItem("Equal Temperament", EqualTemperament);
	modeBox->addItem("Regular Temperament", RegularTemperament);
	modeBox->addListener(this);

	scaleNameLabel.reset(new Label("scaleNameLabel", nameTrans + ':'));
	addAndMakeVisible(scaleNameLabel.get());

	scaleNameBox.reset(new Label("scaleNameBox"));
	addAndMakeVisible(scaleNameBox.get());
	scaleNameBox->setColour(Label::outlineColourId, Colours::black);
	scaleNameBox->setEditable(true);
	scaleNameBox->addListener(this);

	descriptionBox.reset(new TextEditor("descriptionBox"));
	addAndMakeVisible(descriptionBox.get());
	descriptionBox->setText(TRANS("Tuning description"));

	etNotesLabel.reset(new Label("etNotesLabel", sizeTrans + ':'));
	addChildComponent(etNotesLabel.get());

	etNotesSlider.reset(new Slider("etNotesSlider"));
	addChildComponent(etNotesSlider.get());
	etNotesSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
	etNotesSlider->setRange(1, 128, 1);
	etNotesSlider->setValue(12);
	etNotesSlider->addListener(this);

	etPeriodLabel.reset(new Label("etPeriodLabel", periodTrans + ':'));
	addChildComponent(etPeriodLabel.get());
	
	etPeriodBox.reset(new Label("etPeriodBox", "2/1"));
	addChildComponent(etPeriodBox.get());
	etPeriodBox->setColour(Label::outlineColourId, Colours::black);
	etPeriodBox->setEditable(true);
	etPeriodBox->addListener(this);

	generatorTable.reset(new GeneratorTable(tuningDefinition));
	addChildComponent(generatorTable.get());

	// TODO: generators

	btnBar.addComponent(backButton.get());
	btnBar.addComponent(saveButton.get());
	btnBar.addComponent(viewButton.get());
	btnBar.addComponent(modeBox.get(), 11);

	modeBox->setSelectedId(1);
	tuningDefinition = tuningDefinitionIn;
}

CreateScaleWindow::~CreateScaleWindow()
{
	backButton = nullptr;
	saveButton = nullptr;
	viewButton = nullptr;
	modeBox = nullptr;
	scaleNameLabel = nullptr;
	scaleNameBox = nullptr;
	descriptionBox = nullptr;
	etNotesLabel = nullptr;
	etNotesSlider = nullptr;
	etPeriodLabel = nullptr;
	etPeriodBox = nullptr;
	generatorTable = nullptr;
}


void CreateScaleWindow::paint(Graphics& g)
{
	g.fillAll(Colour(0xff323e44));

	// bottom bar background
	g.setColour(Colour(0xff323e44).darker(0.75f));
	g.fillRect(0, proportionOfHeight(5 / 6.0f), getWidth(), proportionOfHeight(1 / 6.0f));
}

void CreateScaleWindow::resized() 
{
	btnBarWidth = getWidth() - 2 * borderGap - 3 * stdGap;
	btnBar.setBounds(borderGap, proportionOfHeight(1 / 12.0f), btnBarWidth, proportionOfHeight(1 / 6.0f));

	backButton->setBounds(btnBar.getComponentBounds(backButton.get()).toNearestInt());
	saveButton->setBounds(btnBar.getComponentBounds(saveButton.get()).toNearestInt());
	viewButton->setBounds(btnBar.getComponentBounds(viewButton.get()).toNearestInt());
	modeBox->setBounds(btnBar.getComponentBounds(modeBox.get()).toNearestInt());

	labelSectHeight = proportionOfHeight(0.5f) - stdGap;

	descriptionBox->setBounds(proportionOfWidth(0.5f), proportionOfHeight(1 / 3.0f), proportionOfWidth(0.5f) - 8, labelSectHeight);

	scaleNameLabel->setBounds(borderGap, proportionOfHeight(1 / 3.0f), Font().getStringWidth(nameTrans) + 2 * stdGap, labelSectHeight / 3.0f);
	scaleNameBox->setBounds(scaleNameLabel->getRight(), scaleNameLabel->getY(), descriptionBox->getX() - scaleNameLabel->getRight() - stdGap, labelSectHeight / 3.0f);
	
	etNotesLabel->setBounds(borderGap, proportionOfHeight(1 / 3.0f) + labelSectHeight / 3, Font().getStringWidth(sizeTrans) + 2 * stdGap, labelSectHeight / 3.0f);
	etNotesSlider->setBounds(etNotesLabel->getRight(), etNotesLabel->getY(), descriptionBox->getX() - etNotesLabel->getRight() - stdGap, labelSectHeight / 3.0f);
	
	etPeriodLabel->setBounds(borderGap, proportionOfHeight(1 / 3.0f) + labelSectHeight * 2 / 3, Font().getStringWidth(periodTrans) + 2 * stdGap, labelSectHeight / 3.0f);
	etPeriodBox->setBounds(etPeriodLabel->getRight(), etPeriodLabel->getY(), descriptionBox->getX() - etPeriodLabel->getRight() - stdGap, labelSectHeight / 3.0f);

	generatorTable->setBounds(borderGap, proportionOfHeight(1 / 3.0f) + labelSectHeight / 3, proportionOfWidth(0.5f) - borderGap - stdGap, proportionOfHeight(1 / 3.0f));
}

void CreateScaleWindow::visibilityChanged()
{
	if (isVisible())
		updateTuning();
}

void CreateScaleWindow::buttonClicked(Button* buttonClicked)
{
	if (buttonClicked == backButton.get())
	{
		listeners.call(&CreateScaleWindow::Listener::backButtonClicked);
	}

	else if (buttonClicked == saveButton.get())
	{
		listeners.call(&CreateScaleWindow::Listener::saveButtonClicked);
	}

	else if (buttonClicked == viewButton.get())
	{
		/* pop up menu */
	}
}

void CreateScaleWindow::labelTextChanged(Label* labelThatChanged) 
{
	if (labelThatChanged == scaleNameBox.get())
	{
	}
	
	else if (labelThatChanged == etPeriodBox.get())
	{
	}

	updateTuning();
}

void CreateScaleWindow::sliderValueChanged(Slider* sliderThatChanged) 
{
	if (sliderThatChanged == etNotesSlider.get())
	{
	}

	updateTuning();
}

void CreateScaleWindow::comboBoxChanged(ComboBox* comboBoxThatChanged) 
{
	setMode(ScaleMode(modeBox->getSelectedId()));
}

void CreateScaleWindow::setMode(ScaleMode modeIn)
{
	mode = modeIn;
	updateModeComponents();
}

void CreateScaleWindow::updateModeComponents()
{
	generatorTable->setVisible(mode == RegularTemperament);

	etNotesLabel->setVisible(mode == EqualTemperament);
	etNotesSlider->setVisible(mode == EqualTemperament);
	etPeriodLabel->setVisible(mode == EqualTemperament);
	etPeriodBox->setVisible(mode == EqualTemperament);
}

void CreateScaleWindow::updateTuning()
{
	if (mode == EqualTemperament)
	{
		tuningDefinition = TuningDefinition::createEqualTemperamentDefinition(
			etNotesSlider->getValue(),
			etPeriodBox->getText(),
			60,
			scaleNameBox->getText(),
			descriptionBox->getText()
		);
	}

	else if (mode == RegularTemperament)
	{
		tuningDefinition.setProperty(TuningDefinition::tuningNameId, scaleNameBox->getText(), nullptr);
		tuningDefinition.setProperty(TuningDefinition::tuningDescriptionId, descriptionBox->getText(), nullptr);
	}

	DBG("Current Scale:\n" + tuningDefinition.toXmlString());
	
	generatorTable->setDefinition(tuningDefinition);
	generatorTable->updateContent();
	
	listeners.call(&CreateScaleWindow::Listener::scaleUpdated, tuningDefinition);
}

ValueTree CreateScaleWindow::getTuningDefinition() const
{
	return tuningDefinition;
}

