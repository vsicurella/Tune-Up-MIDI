/*
  ==============================================================================

    GeneratorBoxModel.cpp
    Created: 3 Jul 2020 2:57:14pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneratorBoxModel.h"

GeneratorTable::GeneratorTable(ValueTree generatorListNodeIn)
{
	generatorListNode = generatorListNodeIn;

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
	// Force having at least one generator with an extra row
	return jmax(1, generatorListNode.getNumChildren()) + 1;
}

void GeneratorTable::paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected)
{
	g.setColour(rowNumber % 2 == 0 ? Colours::grey : Colours::lightgrey);
	g.fillAll();
}

void GeneratorTable::paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected)
{

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

			ValueTree generatorNode = generatorListNode.getChild(rowNumber);

			auto* generatorLabel = static_cast<TableLabel*> (existingComponentToUpdate);
			if (generatorLabel == nullptr)
			{
				generatorLabel = new TableLabel();
				generatorLabel->setEditable(true);
				generatorLabel->addListener(this);
			}

			generatorLabel->setRowNumber(rowNumber);
			generatorLabel->setColumnId(columnId);

			var value = generatorNode.getProperty(id);

			if (value.isVoid())
				value = 0;

			// Round generator to three decimal places for display
			if (columnId == 1)
				value = roundf((double)value * 1000) / 1000.0;

			generatorLabel->setText(value.toString(), dontSendNotification);

			return generatorLabel;
		}
	}

	// Add / Delete Column
	else if (rowNumber > 0)
	{
		auto* button = static_cast<TableButton*>(existingComponentToUpdate);

		if (existingComponentToUpdate == nullptr)
		{
			button = new TableButton();
			button->addListener(this);
		}

		button->setRowNumber(rowNumber);
		button->setColumnId(columnId);

		ValueTree generatorNode = generatorListNode.getChild(rowNumber);

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
	if (lastRowSelected != 0 && lastRowSelected < generatorListNode.getNumChildren())
	{
		removeGenerator(lastRowSelected);
	}
}

void GeneratorTable::returnKeyPressed(int lastRowSelected)
{
	addNewGenerator();
}


void GeneratorTable::editorShown(Label* source, TextEditor& editor)
{
	// Replace generator value editing with full generator value

	TableLabel* tl = static_cast<TableLabel*>(source);

	ValueTree node = generatorListNode.getChild(tl->getRowNumber());
	int col = tl->getColumnId();

	if (col == 1)
	{
		editor.setText(node[TuningDefinition::generatorValueId].toString(), false);
	}
}

void GeneratorTable::labelTextChanged(Label* source)
{
	TableLabel* tl = static_cast<TableLabel*>(source);
	
	ValueTree node = generatorListNode.getChild(tl->getRowNumber());
	int col = tl->getColumnId();

	switch (col)
	{
	case 1:
	{
		double genValue = source->getText().getDoubleValue();
		node.setProperty(TuningDefinition::generatorValueId, genValue, nullptr);
		source->setText(String(round(genValue * 1000) / 1000.0), dontSendNotification); // round for display

		break;
	}
	case 2:
		node.setProperty(TuningDefinition::generatorAmountId, source->getText().getIntValue(), nullptr);
		break;

	case 3:
		node.setProperty(TuningDefinition::generatorOffsetId, source->getText().getIntValue(), nullptr);
		break;

	}

	DBG("LABEL CHANGED, LIST IS:\n" + generatorListNode.toXmlString());

	table.updateContent();
}

void GeneratorTable::buttonClicked(Button* button)
{
	TableButton* tb = static_cast<TableButton*>(button);

	DBG("TABLEBUTTON " + String(tb->getRowNumber()) + " CLICKED");

	if (tb->getRowNumber() == generatorListNode.getNumChildren())
	{
		addNewGenerator();
	}
	else
	{
		removeGenerator(tb->getRowNumber());
	}
}

void GeneratorTable::addNewGenerator()
{	
	ValueTree newNode = ValueTree(TuningDefinition::generatorNodeId);
	newNode.setProperty(TuningDefinition::generatorValueId, 0, nullptr);
	newNode.setProperty(TuningDefinition::generatorAmountId, 0, nullptr);
	newNode.setProperty(TuningDefinition::generatorOffsetId, 0, nullptr);

	generatorListNode.addChild(newNode, generatorListNode.getNumChildren(), nullptr);
	table.updateContent();
}

void GeneratorTable::removeGenerator(int rowNumber)
{
	generatorListNode.removeChild(rowNumber, nullptr);
	table.updateContent();
}

void GeneratorTable::setGeneratorList(ValueTree generatorListNodeIn)
{
	generatorListNode = generatorListNodeIn;
}

void GeneratorTable::updateContent()
{
	table.updateContent();
}

