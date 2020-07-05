/*
  ==============================================================================

    GeneratorBoxModel.cpp
    Created: 3 Jul 2020 2:57:14pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GeneratorBoxModel.h"

GeneratorTable::GeneratorTable(ValueTree tuningDefinitionIn)
{
	TuningDefinition::extractGeneratorProperties(
		tuningDefinitionIn,
		generatorValues,
		generatorAmounts,
		generatorOffsets
	);
	numGenerators = generatorValues.size();

	header = &table.getHeader();
	header->addColumn("#", GeneratorNumber, font.getStringWidth("10"), font.getStringWidth("0"));
	header->addColumn("Gen.", GeneratorValue, font.getStringWidth("1200.0"), font.getStringWidth("120"));
	header->addColumn("Amt.", GeneratorAmt, font.getStringWidth("128"), font.getStringWidth("10"));
	header->addColumn(offsetTrans, GeneratorOffset, font.getStringWidth(offsetTrans), font.getStringWidth("10"));
	header->addColumn("+/-", GeneratorToggle, font.getStringWidth("+-"));
	header->setStretchToFitActive(true);

	table.setColour(ListBox::outlineColourId, Colours::darkgrey);
	table.setOutlineThickness(1);

	addAndMakeVisible(table);
}

GeneratorTable::~GeneratorTable()
{
	removeAllChangeListeners();
}

void GeneratorTable::resized()
{
	table.setBoundsInset(BorderSize<int>(1));
	header->resizeAllColumnsToFit(table.getWidth() - table.getVerticalScrollBar().getWidth());
}

int GeneratorTable::getNumRows()
{
	// Force having at least one generator with an extra row
	return jmax(1, numGenerators) + 1;
}

void GeneratorTable::paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected)
{
	g.setColour(rowNumber % 2 == 0 ? Colours::grey : Colours::lightgrey);
	g.fillAll();
}

void GeneratorTable::paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected)
{
	if (columnId == 1 && rowNumber < numGenerators)
	{
		g.setColour(table.findColour(TableListBox::ColourIds::textColourId));
		g.setFont(font);
		g.drawText(String(rowNumber + 1), Rectangle<float>(0, 0, w, h), Justification::centred);
	}
}

Component* GeneratorTable::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId == GeneratorNumber)
		return nullptr;

	else if (columnId < GeneratorToggle)
	{
		if (rowNumber < numGenerators)
		{
			var value;

			switch (columnId)
			{
			case GeneratorValue:
				// Round for display
				value = roundf((double)generatorValues[rowNumber] * 1000) / 1000.0f;
				break;

			case GeneratorAmt:
				value = generatorAmounts[rowNumber];
				break;

			case GeneratorOffset:
				value = generatorOffsets[rowNumber];
				break;
			}

			if (value.isVoid())
				value = 0;

			auto* generatorLabel = static_cast<TableLabel*> (existingComponentToUpdate);
			if (generatorLabel == nullptr)
			{
				generatorLabel = new TableLabel();
				generatorLabel->setEditable(true);
				generatorLabel->addListener(this);
			}

			generatorLabel->setRowNumber(rowNumber);
			generatorLabel->setColumnId(columnId);
			generatorLabel->setText(value.toString(), dontSendNotification);

			return generatorLabel;
		}
		else
		{
			// don't understand why I have to do this
			delete existingComponentToUpdate;
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

		if (rowNumber < numGenerators)
			button->setButtonText("-");
		else
			button->setButtonText("+");

		return button;
	}

	return nullptr;
}

int GeneratorTable::getColumnAutoSizeWidth(int columnId)
{
	switch (columnId)
	{
	case GeneratorNumber:
	{
		return font.getStringWidth("10");
	}

	case GeneratorValue:
	{
		String genStr, maxGen = "";
		for (auto gen : generatorValues)
		{
			genStr = String(gen);
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}

	case GeneratorAmt:
	{
		String genStr, maxGen = "";
		for (auto gen : generatorAmounts)
		{
			genStr = String(gen);
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}

	case GeneratorOffset:
	{
		String genStr, maxGen = "";
		for (auto gen : generatorOffsets)
		{
			genStr = String(gen);
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}

	case GeneratorToggle:
	{
		return font.getStringWidth("+-");
	}
	}
}

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
	if (lastRowSelected > 1 && lastRowSelected < numGenerators)
		removeGenerator(lastRowSelected);
}

void GeneratorTable::returnKeyPressed(int lastRowSelected)
{
	addNewGenerator(lastRowSelected + 1);
}


void GeneratorTable::editorShown(Label* source, TextEditor& editor)
{
	// Replace generator value editing with full generator value

	TableLabel* tl = static_cast<TableLabel*>(source);

	int col = tl->getColumnId();

	if (col == GeneratorValue)
	{
		editor.setText(String(generatorValues[tl->getRowNumber()]), false);
		editor.selectAll();
	}
}

void GeneratorTable::labelTextChanged(Label* source)
{
	TableLabel* tl = static_cast<TableLabel*>(source);
	
	int col = tl->getColumnId();

	switch (col)
	{
	case GeneratorValue:
	{
		double genValue = source->getText().getDoubleValue();
		generatorValues.set(tl->getRowNumber(), genValue);
		source->setText(String(round(genValue * 1000) / 1000.0), dontSendNotification); // round for display
		break;
	}
	case GeneratorAmt:
		generatorAmounts.set(tl->getRowNumber(), source->getText().getIntValue());
		break;

	case GeneratorOffset:
		generatorOffsets.set(tl->getRowNumber(), source->getText().getIntValue());
		break;
	}

	table.updateContent();
	sendChangeMessage();
}

void GeneratorTable::buttonClicked(Button* button)
{
	TableButton* tb = static_cast<TableButton*>(button);

	if (tb->getRowNumber() == numGenerators)
	{
		addNewGenerator();
	}
	else
	{
		removeGenerator(tb->getRowNumber());
	}
}

void GeneratorTable::addNewGenerator(int rowNumber)
{	
	numGenerators++;

	if (rowNumber == -1)
		rowNumber = numGenerators;

	generatorValues.insert(rowNumber, 0);
	generatorAmounts.insert(rowNumber, 0);
	generatorOffsets.insert(rowNumber, 0);
	
	table.updateContent();
}

void GeneratorTable::removeGenerator(int rowNumber)
{
	numGenerators--;

	generatorValues.remove(rowNumber);
	generatorAmounts.remove(rowNumber);
	generatorOffsets.remove(rowNumber);

	table.updateContent();
	sendChangeMessage();
}

void GeneratorTable::updateDefinition(ValueTree tuningDefinitionIn)
{
	TuningDefinition::extractGeneratorProperties(
		tuningDefinitionIn,
		generatorValues,
		generatorAmounts,
		generatorOffsets
	);

	numGenerators = generatorValues.size();
	table.updateContent();
}

void GeneratorTable::updateContent()
{
	table.updateContent();
}

void GeneratorTable::setTableColour(int colourId, Colour colour)
{
	table.setColour(colourId, colour);
}

ValueTree GeneratorTable::getDefinition() const
{
	return TuningDefinition::createRegularTemperamentDefinition(
		generatorValues,
		generatorAmounts,
		generatorOffsets
	);
}