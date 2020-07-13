/*
  ==============================================================================

    DynamicTuningTable.cpp
    Created: 13 Jul 2020 12:08:18pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DynamicTuningTable.h"

DynamicTuningTable::DynamicTuningTable(ValueTree tuningDefinitionIn)
{
	definition = tuningDefinitionIn;
	generatorListNode = definition.getChildWithName(TuneUpIDs::generatorListId);
}

DynamicTuningTable::~DynamicTuningTable()
{
	removeAllChangeListeners();
}

void DynamicTuningTable::resized()
{

}

//=============================================================================

// TableListBoxModel implementation

int DynamicTuningTable::getNumRows()
{
	return generatorListNode.getNumChildren();
}

void DynamicTuningTable::paintRowBackground(Graphics& g, int rowNumber, int w, int h, bool rowIsSelected)
{
	Colour c = table.findColour(TableListBox::ColourIds::backgroundColourId);
	if (rowNumber % 2 == 0)
		c.brighter(0.25f);

	g.fillAll(c);
}

void DynamicTuningTable::paintCell(Graphics& g, int rowNumber, int columnId, int w, int h, bool isSelected)
{

}

Component* DynamicTuningTable::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId == ColumnType::CCNumber)
	{
		auto* numberSlider = static_cast<TableSlider*> (existingComponentToUpdate);
		if (numberSlider == nullptr)
		{
			numberSlider = new TableSlider();
			numberSlider->setName("CCNumberSlider-" + String(rowNumber));
			numberSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
			numberSlider->setRange(0, 127, 1);
			//numberSlider->onValueChange = [=]()
			//{
			//	TableSlider* thisSlider = (TableSlider*)this;
			//	if (thisSlider->getValue() == 0)
			//		thisSlider->se
			//}
			numberSlider->addListener(this);
		}

		numberSlider->setRowNumber(rowNumber);
		numberSlider->setColumnId(columnId);


	}

	if (columnId < GeneratorToggle)
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

int DynamicTuningTable::getColumnAutoSizeWidth(int columnId)
{

}

String DynamicTuningTable::getCellTooltip(int rowNumber, int columnId)
{

}

void DynamicTuningTable::deleteKeyPressed(int lastRowSelected)
{

}

void DynamicTuningTable::returnKeyPressed(int lastRowSelected)
{

}

//=============================================================================

// Label Listener 

void DynamicTuningTable::editorShown(Label* source, TextEditor& editor)
{

}

void DynamicTuningTable::labelTextChanged(Label* source)
{

}

// Button Listener 

void DynamicTuningTable::buttonClicked(Button* button)
{

}

// Slider Listener

void DynamicTuningTable::sliderValueChanged(Slider* slider)
{

}

//=============================================================================


void DynamicTuningTable::updateContent()
{
	table.updateContent();
}

void DynamicTuningTable::setTableColour(int colourId, Colour colour)
{
	table.setColour(colourId, colour);
}

