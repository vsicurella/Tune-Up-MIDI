/*
  ==============================================================================

    DynamicTuningTable.h
    Created: 13 Jul 2020 18:08:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "CommonUI.h"

class DynamicTuningTable :	public Component,
							public TableListBoxModel,
							public ChangeBroadcaster,
							private Label::Listener,
							private TextButton::Listener,
							private Slider::Listener
{
public:

	enum ColumnType
	{
		CCNumber = 1,
		CCRange,
		CCCenter,
		GeneratorValue,
		MinValue,
		MaxValue
		//Skew
	};

public:

	DynamicTuningTable(ValueTree tuningDefinitionIn);
	~DynamicTuningTable();

	void resized() override;

	//=============================================================================

	// TableListBoxModel implementation

	int getNumRows() override;

	void paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected) override;

	void paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected) override;

	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

	int getColumnAutoSizeWidth(int columnId) override;

	String getCellTooltip(int rowNumber, int columnId) override;

	void deleteKeyPressed(int lastRowSelected) override;

	//=============================================================================

	// Label Listener 

	void editorShown(Label* source, TextEditor& editor) override;

	void labelTextChanged(Label* source) override;

	// Button Listener 

	void buttonClicked(Button* button) override;

	// Slider Listener

	void sliderValueChanged(Slider* slider) override;

	//=============================================================================


	void updateContent();

	void setTableColour(int colourId, Colour colour);


private:

	ValueTree definition;

	Font font = Font(11, Font::plain);
	TableListBox table = { {}, this };
	TableHeaderComponent* header;
};