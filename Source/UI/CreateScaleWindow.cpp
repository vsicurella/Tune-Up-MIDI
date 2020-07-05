/*
  ==============================================================================

    CreateScaleWindow.cpp
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "CreateScaleWindow.h"

CreateScaleWindow::CreateScaleWindow(ValueTree tuningDefinitionIn)
	: unitGrid(2, 13)
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
	descriptionBox->setReturnKeyStartsNewLine(true);
	descriptionBox->setMultiLine(true);
	descriptionBox->setTextToShowWhenEmpty(TRANS("Tuning description"), Colours::darkgrey);

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
	generatorTable->addChangeListener(this);

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

}

void CreateScaleWindow::resized() 
{
	unitGrid.setSize(getWidth(), getHeight());

	btnBarWidth = getWidth() - 3 * stdGap;
	btnBar.setBounds(0, 0, btnBarWidth, unitGrid.getY(2));

	backButton->setBounds(btnBar.getComponentBounds(backButton.get()).toNearestInt());
	saveButton->setBounds(btnBar.getComponentBounds(saveButton.get()).toNearestInt());
	viewButton->setBounds(btnBar.getComponentBounds(viewButton.get()).toNearestInt());
	modeBox->setBounds(btnBar.getComponentBounds(modeBox.get()).toNearestInt());

	//labelSectHeight = proportionOfHeight(0.5f) - stdGap;

	descriptionBox->setBounds(unitGrid.getX(1), unitGrid.getY(3), unitGrid.getX(1), unitGrid.getY(8));

	scaleNameLabel->setBounds(0, unitGrid.getY(3), Font().getStringWidth(nameTrans) + 2 * stdGap, unitGrid.getY(2));
	scaleNameBox->setBounds(scaleNameLabel->getRight(), scaleNameLabel->getY(), descriptionBox->getX() - scaleNameLabel->getRight() - stdGap, scaleNameLabel->getHeight());
	
	etNotesLabel->setBounds(0, unitGrid.getY(6), Font().getStringWidth(sizeTrans) + 2 * stdGap, unitGrid.getY(2));
	etNotesSlider->setBounds(etNotesLabel->getRight(), etNotesLabel->getY(), descriptionBox->getX() - etNotesLabel->getRight() - stdGap, etNotesLabel->getHeight());
	
	etPeriodLabel->setBounds(0, unitGrid.getY(9), Font().getStringWidth(periodTrans) + 2 * stdGap, unitGrid.getY(2));
	etPeriodBox->setBounds(etPeriodLabel->getRight(), etPeriodLabel->getY(), descriptionBox->getX() - etPeriodLabel->getRight() - stdGap, etPeriodLabel->getHeight());

	generatorTable->setBounds(0, unitGrid.getY(5), unitGrid.getX(1) - stdGap, unitGrid.getY(6));
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
		updateTuning();
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

void CreateScaleWindow::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == generatorTable.get())
	{
		updateTuning();
	}
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

		generatorTable->updateDefinition(tuningDefinition);
	}

	else if (mode == RegularTemperament)
	{
		tuningDefinition = generatorTable->getDefinition();

		String name = scaleNameBox->getText();
		if (name.length() > 0)
			tuningDefinition.setProperty(TuningDefinition::tuningNameId, name, nullptr);

		String desc = descriptionBox->getText();
		if (desc.length() > 0)
			tuningDefinition.setProperty(TuningDefinition::tuningDescriptionId, desc, nullptr);
	}
	
	listeners.call(&CreateScaleWindow::Listener::scaleUpdated, tuningDefinition);
}

ValueTree CreateScaleWindow::getTuningDefinition() const
{
	return tuningDefinition;
}

