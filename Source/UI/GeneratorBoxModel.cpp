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

	header->addColumn("#", GeneratorNumber, font.getStringWidth("10"), font.getStringWidth("0"));
	header->addColumn("Gen", GeneratorValue, font.getStringWidth("1200.0"), font.getStringWidth("120"));
	header->addColumn("Amt", GeneratorAmt, font.getStringWidth("128"), font.getStringWidth("10"));
	header->addColumn(offsetTrans, GeneratorOffset, font.getStringWidth(offsetTrans), font.getStringWidth("10"));
	header->addColumn("+/-", GeneratorToggle, font.getStringWidth("+/-"));
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
	if (columnId == 1)
	{
		g.setColour(table.findColour(TableListBox::ColourIds::textColourId));
		g.setFont(font);
		g.drawText(String(rowNumber), Rectangle<float>(0, 0, w, h), Justification::centred);
	}
}

Component* GeneratorTable::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId == GeneratorNumber)
		return nullptr;

	else if (columnId < GeneratorToggle)
	{
		if (rowNumber < getNumRows() - 1)
		{
			Identifier id;

			switch (columnId)
			{
			case GeneratorValue:
				id = TuningDefinition::generatorValueId;
				break;

			case GeneratorAmt:
				id = TuningDefinition::generatorAmountId;
				break;

			case GeneratorOffset:
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
			if (columnId == 2)
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

	if (col == GeneratorValue)
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
	case GeneratorValue:
	{
		double genValue = source->getText().getDoubleValue();
		node.setProperty(TuningDefinition::generatorValueId, genValue, nullptr);
		source->setText(String(round(genValue * 1000) / 1000.0), dontSendNotification); // round for display

		break;
	}
	case GeneratorAmt:
		node.setProperty(TuningDefinition::generatorAmountId, source->getText().getIntValue(), nullptr);
		break;

	case GeneratorOffset:
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

void GeneratorTable::setTableColour(int colourId, Colour colour)
{
	table.setColour(colourId, colour);
}