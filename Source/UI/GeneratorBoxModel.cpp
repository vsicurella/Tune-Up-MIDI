/*
  ==============================================================================

    GeneratorBoxModel.cpp
    Created: 3 Jul 2020 2:57:14pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneratorBoxModel.h"

GeneratorTable::GeneratorTable(ValueTree tuningDefinition)
{
	header = &table.getHeader();

	header->addColumn(generatorTrans, 1, Font().getStringWidth("1200.000") + 8, Font().getStringWidth("1200"));
	header->addColumn(amountTrans, 2, Font().getStringWidth("128") + 8, Font().getStringWidth("10"));
	header->addColumn(offsetTrans, 3, Font().getStringWidth(offsetTrans), Font().getStringWidth("10"));
	header->addColumn("+/-", 4, Font().getStringWidth("+/-"));
	header->setStretchToFitActive(true);

	table.setColour(ListBox::outlineColourId, Colours::darkgrey);
	table.setOutlineThickness(1);

	addAndMakeVisible(table);
}

GeneratorTable::~GeneratorTable()
{

}

void GeneratorTable::resized()
{
	table.setBoundsInset(BorderSize<int>(1));
	header->resizeAllColumnsToFit(table.getWidth() - table.getVerticalScrollBar().getWidth());
}

int GeneratorTable::getNumRows()
{
	// Force having a period
	return jmax(1, tuningDefinition.getChildWithName(TuningDefinition::generatorListId).getNumChildren() + 1);
}

void GeneratorTable::paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected)
{
	g.setColour(rowNumber % 2 == 0 ? Colours::grey : Colours::lightgrey);
	g.fillAll();
}

Component* GeneratorTable::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId < 4)
	{
		if (rowNumber < getNumRows() - 1)
		{
			Identifier id;

			switch (columnId)
			{
			case 1:
				id = TuningDefinition::generatorValueId;
				break;

			case 2:
				id = TuningDefinition::generatorAmountId;
				break;

			case 3:
				id = TuningDefinition::generatorOffsetId;
				break;
			}

			ValueTree generatorNode = tuningDefinition.getChildWithName(TuningDefinition::generatorListId).getChild(rowNumber);

			auto* generatorLabel = static_cast<Label*> (existingComponentToUpdate);
			if (generatorLabel == nullptr)
			{
				generatorLabel = new Label("Label" + String(rowNumber) + "," + String(columnId));
				generatorLabel->setEditable(true);
			}

			var value = generatorNode.getProperty(id);

			if (value.isVoid())
				value = 0;

			// Round generator to three decimal places for display
			if (columnId == 1)
				value = round((double)value * 1000) / 1000.0;

			generatorLabel->setText(value.toString(), dontSendNotification);
			return generatorLabel;
		}
	}

	// Add / Delete Column
	else if (rowNumber > 0)
	{
		auto* button = static_cast<TextButton*>(existingComponentToUpdate);

		if (existingComponentToUpdate == nullptr)
		{
			button = new TextButton();
		}

		ValueTree generatorNode = tuningDefinition.getChildWithName(TuningDefinition::generatorListId).getChild(rowNumber);

		if (generatorNode.isValid())
			button->setButtonText("-");
		else
			button->setButtonText("+");
		

		return button;
	}

	return nullptr;
}


//int GeneratorBoxModel::getColumnAutoSizeWidth(int columnId)
//{
//
//}

String GeneratorTable::getCellTooltip(int rowNumber, int columnId)
{
	switch (columnId)
	{
	case 2:
		return "The amount of times the generator is stacked.";

	case 3:
		return "How many generators you go down before you start stacking.";

	case 4:
		return "Add or delete a generator.";

	default:
		{
		if (rowNumber == 0)
			return "The period of the scale.";
		else
			return "A generator of the scale.";
		}
	}
}


void GeneratorTable::deleteKeyPressed(int lastRowSelected)
{

}

void GeneratorTable::returnKeyPressed(int lastRowSelected)
{

}

void GeneratorTable::setDefinition(ValueTree definitionIn)
{
	tuningDefinition = definitionIn;
}

void GeneratorTable::updateContent()
{
	table.updateContent();
}