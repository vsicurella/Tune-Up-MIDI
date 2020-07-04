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
#include "ButtonBar.h"
#include "GeneratorBoxModel.h"
#include "UnitGrid.h"

class CreateScaleWindow :	public Component,
							public ChangeBroadcaster,
							private Button::Listener,
							private Label::Listener,
							private Slider::Listener,
							private ComboBox::Listener
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

	void visibilityChanged() override;

	void buttonClicked(Button* buttonClicked) override;
	void labelTextChanged(Label* labelThatChanged) override;
	void sliderValueChanged(Slider* sliderThatChanged) override;
	void comboBoxChanged(ComboBox* comboBoxThatChanged) override;

	//==========================================================================

	void setMode(ScaleMode modeIn);

	void updateModeComponents();

	void updateTuning();

	ValueTree getTuningDefinition() const;


	//==============================================================================

	class Listener
	{
	public:

		virtual void scaleUpdated(ValueTree tuningDefinition) = 0;
		virtual void saveButtonClicked() = 0;
		virtual void backButtonClicked() = 0;
	};

	void addListener(Listener* listenerIn)
	{
		listeners.add(listenerIn);
	}

	void removeListener(Listener* listenerIn)
	{
		listeners.remove(listenerIn);
	}


protected:

	ListenerList<Listener> listeners;

	//==============================================================================

private:

	ScaleMode mode;

	std::unique_ptr<TextButton> backButton;
	std::unique_ptr<TextButton> saveButton;
	std::unique_ptr<TextButton> viewButton;
	std::unique_ptr<ComboBox> modeBox;

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

	ValueTree tuningDefinition;

	// Helpers
	int btnBarWidth;
	ButtonBar btnBar;
	int stdGap = 8;
	//float labelSectHeight;
	UnitGrid unitGrid;

	String backTrans = TRANS("Back");
	String saveTrans = TRANS("Save");
	String viewTrans = TRANS("View");
	String nameTrans = TRANS("Name");
	String sizeTrans = TRANS("Size");
	String periodTrans = TRANS("Period");
	String generatorTrans = TRANS("Generators");
};
