/*
  ==============================================================================

    GeneratorBoxModel.h
    Created: 3 Jul 2020 2:57:14pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../TuningDefinition.h"

class GeneratorTable :	public Component, 
						public TableListBoxModel,
						private Label::Listener,
						private TextButton::Listener
{
public:

	GeneratorTable(ValueTree generatorListNodeIn);
	~GeneratorTable();

	void resized() override;

	//=============================================================================

	// TableListBoxModel implementation

	int getNumRows() override;

	void paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected) override;

	void paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected) override;

	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

	//int getColumnAutoSizeWidth(int columnId) override;

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

	void addNewGenerator();

	void removeGenerator(int rowNumber);

	//=============================================================================

	void setGeneratorList(ValueTree generatorListNodeIn);

	void updateContent();

private:

	ValueTree generatorListNode;

	TableListBox table = { {}, this };
	TableHeaderComponent* header;

	String generatorTrans = TRANS("Generator");
	String amountTrans = TRANS("Amount");
	String offsetTrans = TRANS("Offset");
};


class TableComponent
{
	int rowNumber;
	int columnId;

public:

	void setRowNumber(int rowNumberIn)
	{
		rowNumber = rowNumberIn;
	}

	void setColumnId(int columnIdIn)
	{
		columnId = columnIdIn;
	}

	int getRowNumber() const
	{
		return rowNumber;
	}

	int getColumnId() const
	{
		return columnId;
	}
};

class TableLabel : public TableComponent, public Label {};
class TableButton : public TableComponent, public TextButton {};