/*
  ==============================================================================

    CreateScaleWindow.cpp
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "CreateScaleWindow.h"

CreateTuningWindow::CreateTuningWindow(ValueTree tuningDefinitionIn)
	: grid(2, 6)
{
	tuningNameLabel.reset(new Label("scaleNameLabel", nameTrans + ':'));
	addAndMakeVisible(tuningNameLabel.get());

	tuningNameBox.reset(new Label("scaleNameBox"));
	addAndMakeVisible(tuningNameBox.get());
	tuningNameBox->setColour(Label::outlineColourId, Colours::black);
	tuningNameBox->setEditable(true);
	tuningNameBox->addListener(this);

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

	tuningDefinition = tuningDefinitionIn;
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
	grid.setSize(getWidth(), getHeight());

	descriptionBox->setBounds(grid.getX(1), 0, grid.getX(1), getHeight());

	tuningNameLabel->setBounds(0, 0, Font().getStringWidth(nameTrans) + 2 * stdGap, grid.getY(2) - stdGap);
	tuningNameBox->setBounds(tuningNameLabel->getRight(), tuningNameLabel->getY(), descriptionBox->getX() - tuningNameLabel->getRight() - stdGap, tuningNameLabel->getHeight());
	
	etNotesLabel->setBounds(0, grid.getY(2), Font().getStringWidth(sizeTrans) + 2 * stdGap, grid.getY(2) - stdGap);
	etNotesSlider->setBounds(etNotesLabel->getRight(), etNotesLabel->getY(), descriptionBox->getX() - etNotesLabel->getRight() - stdGap, etNotesLabel->getHeight());
	
	etPeriodLabel->setBounds(0, grid.getY(4), Font().getStringWidth(periodTrans) + 2 * stdGap, grid.getY(2) - stdGap);
	etPeriodBox->setBounds(etPeriodLabel->getRight(), etPeriodLabel->getY(), descriptionBox->getX() - etPeriodLabel->getRight() - stdGap, etPeriodLabel->getHeight());

	generatorTable->setBounds(0, grid.getY(2), grid.getX(1) - stdGap, getHeight() - grid.getY(2));
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

void CreateTuningWindow::setMode(TuningMode modeIn)
{
	mode = modeIn;
	
	generatorTable->setVisible(mode == RegularTemperament);
	etNotesLabel->setVisible(mode == EqualTemperament);
	etNotesSlider->setVisible(mode == EqualTemperament);
	etPeriodLabel->setVisible(mode == EqualTemperament);
	etPeriodBox->setVisible(mode == EqualTemperament);
}

void CreateTuningWindow::updateTuning()
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
			tuningDefinition.setProperty(TuningDefinition::tuningNameId, name, nullptr);

		String desc = descriptionBox->getText();
		if (desc.length() > 0)
			tuningDefinition.setProperty(TuningDefinition::tuningDescriptionId, desc, nullptr);
	}
	
	sendChangeMessage();
}

ValueTree CreateTuningWindow::getTuningDefinition() const
{
	return tuningDefinition;
}

