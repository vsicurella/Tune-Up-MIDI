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

class GeneratorTable : public Component, public TableListBoxModel
{
public:

	GeneratorTable(ValueTree tuningDefinitionIn);
	~GeneratorTable();

	void resized() override;

	//=============================================================================

	int getNumRows() override;

	void paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected) override;

	Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

	//int getColumnAutoSizeWidth(int columnId) override;

	String getCellTooltip(int rowNumber, int columnId) override;

	void deleteKeyPressed(int lastRowSelected) override;

	void returnKeyPressed(int lastRowSelected) override;

	//=============================================================================

	void setDefinition(ValueTree definitionIn);

	void updateContent();

private:

	ValueTree tuningDefinition;

	TableListBox table = { {}, this };
	TableHeaderComponent* header;

	String generatorTrans = TRANS("Generator");
	String amountTrans = TRANS("Amount");
	String offsetTrans = TRANS("Offset");
};