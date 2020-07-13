/*
  ==============================================================================

    DynamicTuningTable.h
    Created: 13 Jul 2020 18:08:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "CommonUI.h"

class RegularTemperamentTable : public Component,
								public TableListBoxModel,
								public ChangeBroadcaster,
								private Label::Listener,
								private TextButton::Listener
{
public:

	enum ColumnType
	{
		CCNumber = 1,
		CCRange,
		CCCenter,
		GeneratorValue,
		MinValue,
		MaxValue,
		Skew
	};

public:

	RegularTemperamentTable(ValueTree tuningDefinitionIn);
	~RegularTemperamentTable();

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

	void returnKeyPressed(int lastRowSelected) override;

	//=============================================================================

	// Label::Listener implementation

	void editorShown(Label* source, TextEditor& editor) override;

	void labelTextChanged(Label* source) override;

	// Button Listener implementation

	void buttonClicked(Button* button) override;

	//=============================================================================

	void addNewGenerator(int rowNumber = -1);

	void removeGenerator(int rowNumber);

	//=============================================================================

	void updateContent();

	void setTableColour(int colourId, Colour colour);


private:

	int numGenerators = 1;
	Array<double> generatorValues;
	Array<int> generatorAmounts;
	Array<int> generatorOffsets;

	Font font = Font(11, Font::plain);
	TableListBox table = { {}, this };
	TableHeaderComponent* header;
};