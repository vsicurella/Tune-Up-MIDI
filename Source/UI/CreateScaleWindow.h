/*
  ==============================================================================

    CreateScaleWindow.h
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "GeneratorBoxModel.h"
#include "UnitGrid.h"

class CreateTuningWindow :	public Component,
							public ChangeListener,
							public ChangeBroadcaster,
							private Label::Listener,
							private Slider::Listener
{
public:

	enum TuningMode
	{
		EqualTemperament = 1,
		RegularTemperament
	};

public:

	CreateTuningWindow(ValueTree tuningDefinitionIn /* TODO some recall */);
	~CreateTuningWindow();

	void paint(Graphics& g) override;
	void resized() override;

	void editorShown(Label* source, TextEditor& editor) override;
	void labelTextChanged(Label* labelThatChanged) override;
	void sliderValueChanged(Slider* sliderThatChanged) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==========================================================================

	void setMode(TuningMode modeIn);

	void updateTuning();

	ValueTree getTuningDefinition() const;

	//==============================================================================

private:

	TuningMode mode;
	ValueTree tuningDefinition;

	std::unique_ptr<Label> tuningNameLabel;
	std::unique_ptr<Label> tuningNameBox;
	std::unique_ptr<TextEditor> descriptionBox;

	Font font;

	// Equal Temperament Controls
	std::unique_ptr<Label> etNotesLabel;
	std::unique_ptr<Slider> etNotesSlider;
	std::unique_ptr<Label> etPeriodLabel;
	std::unique_ptr<Label> etPeriodBox;

	// Regular Temperament Controls
	std::unique_ptr<GeneratorTable> generatorTable;

	// Helpers
	UnitGrid grid;
	int stdGap = 8;
	int labelWidth = 32;

	String nameTrans = TRANS("Name");
	String sizeTrans = TRANS("Size");
	String periodTrans = TRANS("Period");
	String generatorTrans = TRANS("Generators");
};
