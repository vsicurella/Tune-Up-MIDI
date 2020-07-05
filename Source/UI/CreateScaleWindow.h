/*
  ==============================================================================

    CreateScaleWindow.h
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../TuningDefinition.h"
#include "GeneratorBoxModel.h"
#include "UnitGrid.h"

class CreateScaleWindow :	public Component,
							public ChangeListener,
							public ChangeBroadcaster,
							private Label::Listener,
							private Slider::Listener
{
public:

	enum ScaleMode
	{
		EqualTemperament = 1,
		RegularTemperament
	};

public:

	CreateScaleWindow(ValueTree tuningDefinitionIn /* TODO some recall */);
	~CreateScaleWindow();

	void paint(Graphics& g) override;
	void resized() override;

	void labelTextChanged(Label* labelThatChanged) override;
	void sliderValueChanged(Slider* sliderThatChanged) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==========================================================================

	void setMode(ScaleMode modeIn);

	void updateTuning();

	ValueTree getTuningDefinition() const;

	//==============================================================================

private:

	ScaleMode mode;
	ValueTree tuningDefinition;

	std::unique_ptr<Label> scaleNameLabel;
	std::unique_ptr<Label> scaleNameBox;
	std::unique_ptr<TextEditor> descriptionBox;

	// Equal Temperament Controls
	std::unique_ptr<Label> etNotesLabel;
	std::unique_ptr<Slider> etNotesSlider;
	std::unique_ptr<Label> etPeriodLabel;
	std::unique_ptr<Label> etPeriodBox;

	// Regular Temperament Controls
	std::unique_ptr<GeneratorTable> generatorTable;

	// Helpers
	int stdGap = 8;
	UnitPlane grid;

	String nameTrans = TRANS("Name");
	String sizeTrans = TRANS("Size");
	String periodTrans = TRANS("Period");
	String generatorTrans = TRANS("Generators");
};
