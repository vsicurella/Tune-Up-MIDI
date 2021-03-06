/*
  ==============================================================================

    CreateScaleWindow.h
    Created: 2 Jul 2020 10:16:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "CommonUI.h"
#include "../IDs.h"
#include "../TuneUpModes.h"
#include "RegularTemperamentTable.h"

using namespace TuneUpMode;

class CreateTuningWindow :	public Component,
							public ChangeListener,
							public ChangeBroadcaster,
							private Label::Listener,
							private Slider::Listener
{
public:

	CreateTuningWindow(ValueTree sessionNodeIn);
	~CreateTuningWindow();

	void paint(Graphics& g) override;
	void resized() override;

	void editorShown(Label* source, TextEditor& editor) override;
	void labelTextChanged(Label* labelThatChanged) override;
	void sliderValueChanged(Slider* sliderThatChanged) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	//==========================================================================

	void setMode(CreateTuningMode modeIn);

	void loadOptionsNode(ValueTree optionsNodeIn);

	void updateTuning(bool sendChange = true);

	ValueTree getTuningDefinition() const;

	//==============================================================================

private:

	CreateTuningMode mode;
	ValueTree sessionOptionsNode;
	ValueTree newTuning;

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
	std::unique_ptr<RegularTemperamentTable> generatorTable;

	// Helpers
	UnitGrid grid;
	int labelWidth = 32;
};
