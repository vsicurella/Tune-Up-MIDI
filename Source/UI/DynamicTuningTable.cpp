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

	header = &table.getHeader();
	header->addColumn("CC #", CCNumber, font.getStringWidth("000"), font.getStringWidth("0"));
	header->addColumn("CC " + rangeTrans, CCRange, font.getStringWidth("[000,000]"), font.getStringWidth("[0,0]"));
	header->addColumn("CC " + centerTrans, CCCenter, font.getStringWidth("000"), font.getStringWidth("0"));
	header->addColumn("Gen.", GeneratorValue, font.getStringWidth("000.000"), font.getStringWidth("000"));
	header->addColumn("Min", MinValue, font.getStringWidth("000.000"), font.getStringWidth("000"));
	header->addColumn("Max", MaxValue, font.getStringWidth("000.000"), font.getStringWidth("000"));
	header->setStretchToFitActive(true);

	table.setColour(ListBox::outlineColourId, Colours::darkgrey);
	table.setOutlineThickness(1);

	addAndMakeVisible(table);
}

DynamicTuningTable::~DynamicTuningTable()
{
	removeAllChangeListeners();
}

void DynamicTuningTable::resized()
{
	table.setBoundsInset(BorderSize<int>(1));
}

//=============================================================================

// TableListBoxModel implementation

int DynamicTuningTable::getNumRows()
{
	return definition.getChildWithName(TuneUpIDs::generatorListId).getNumChildren();
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
	ValueTree genNode = definition.getChildWithName(TuneUpIDs::generatorListId).getChild(rowNumber);
	bool isDynamic = genNode[dynamicTuningId];
	var value, value2;

	switch (columnId)
	{
	case ColumnType::CCNumber:
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

		if (isDynamic)
			value = genNode[dynamicGenCCNumId];
		else
			value = -1;

		numberSlider->setValue(value, dontSendNotification);

		return numberSlider;
	}

	case ColumnType::CCRange:
	{
		// TODO: NumberSelector Component

		auto* dualSlider = static_cast<TableSlider*> (existingComponentToUpdate);
		if (dualSlider == nullptr)
		{
			dualSlider = new TableSlider();
			dualSlider->setName("CCRangeSlider-" + String(rowNumber));
			dualSlider->setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
			dualSlider->setRange(0, 127, 1);
			dualSlider->addListener(this);
		}

		dualSlider->setRowNumber(rowNumber);
		dualSlider->setColumnId(columnId);

		if (isDynamic)
		{
			value = genNode[dynamicGenMinId];
			value2 = genNode[dynamicGenMaxId];
		}
		else
		{
			value = 0;
			value2 = 127;
		}

		dualSlider->setMinAndMaxValues(value, value2, dontSendNotification);

		return dualSlider;
	}

	case ColumnType::CCCenter:
	{
		// TODO: LEARN COMPONENT

		auto* numberSlider = static_cast<TableSlider*> (existingComponentToUpdate);
		if (numberSlider == nullptr)
		{
			numberSlider = new TableSlider();
			numberSlider->setName("CCCenterSlider-" + String(rowNumber));
			numberSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
			numberSlider->setRange(0, 127, 1);
			numberSlider->addListener(this);
		}

		numberSlider->setRowNumber(rowNumber);
		numberSlider->setColumnId(columnId);

		if (isDynamic)
		{
			value = genNode[dynamicGenCenterId];
		}
		else
		{
			value = 64;
		}

		numberSlider->setValue(value, dontSendNotification);

		return numberSlider;
	}


	case ColumnType::GeneratorValue:
	{
		if (rowNumber < definition.getChildWithName(TuneUpIDs::generatorListId).getNumChildren())
		{
			ValueTree genNode = definition.getChildWithName(TuneUpIDs::generatorListId).getChild(rowNumber);

			auto* generatorLabel = static_cast<TableLabel*> (existingComponentToUpdate);
			if (generatorLabel == nullptr)
			{
				generatorLabel = new TableLabel();
			}

			generatorLabel->setRowNumber(rowNumber);
			generatorLabel->setColumnId(columnId);

			// Round for display
			value = roundf((double)genNode[generatorValueId] * 1000) / 1000.0f;
			generatorLabel->setText(value.toString(), dontSendNotification);

			return generatorLabel;
		}
	}


	case ColumnType::MinValue:
	{
		auto* minLabel = static_cast<TableLabel*> (existingComponentToUpdate);
		if (minLabel == nullptr)
		{
			minLabel = new TableLabel();
			minLabel->setEditable(true);
			minLabel->addListener(this);
		}

		minLabel->setRowNumber(rowNumber);
		minLabel->setColumnId(columnId);

		if (isDynamic)
			value = genNode[dynamicGenMinId];
		else
			value = roundf((double)genNode[generatorValueId] * 1000) / 1000.0f;

		minLabel->setText(value.toString(), dontSendNotification);

		return minLabel;
	}

	case ColumnType::MaxValue:
	{
		auto* maxLabel = static_cast<TableLabel*> (existingComponentToUpdate);
		if (maxLabel == nullptr)
		{
			maxLabel = new TableLabel();
			maxLabel->setEditable(true);
			maxLabel->addListener(this);
		}

		maxLabel->setRowNumber(rowNumber);
		maxLabel->setColumnId(columnId);

		if (isDynamic)
			value = genNode[dynamicGenMaxId];
		else
			value = roundf((double)genNode[generatorValueId] * 1000) / 1000.0f;

		maxLabel->setText(value.toString(), dontSendNotification);

		return maxLabel;
	}

	//case ColumnType::Skew:
	//{
	//	auto* numberSlider = static_cast<TableSlider*> (existingComponentToUpdate);
	//	if (numberSlider == nullptr)
	//	{
	//		numberSlider = new TableSlider();
	//		numberSlider->setName("SkewSlider-" + String(rowNumber));
	//		numberSlider->setSliderStyle(Slider::SliderStyle::IncDecButtons);
	//		numberSlider->setRange(0, 127, 1);
	//		numberSlider->addListener(this);
	//	}

	//	numberSlider->setRowNumber(rowNumber);
	//	numberSlider->setColumnId(columnId);

	//	return numberSlider;
	//}
	}

	return nullptr;
}

int DynamicTuningTable::getColumnAutoSizeWidth(int columnId)
{
	switch (columnId)
	{
	case CCNumber:
	{
		return font.getStringWidth("00");
	}

	case CCRange:
	{
		return font.getStringWidth("[0, 0]");
	}

	case CCCenter:
	{
		return font.getStringWidth("00");
	}

	case GeneratorValue:
	{
		String genStr, maxGen = "";
		for (auto gen : definition.getChildWithName(TuneUpIDs::generatorListId))
		{
			genStr = gen[TuneUpIDs::generatorValueId].toString();
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}


	case MinValue:
	{
		String genStr, maxGen = "";
		for (auto gen : definition.getChildWithName(TuneUpIDs::generatorListId))
		{
			genStr = gen[TuneUpIDs::dynamicGenMinId].toString();
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}

	case MaxValue:
	{
		String genStr, maxGen = "";
		for (auto gen : definition.getChildWithName(TuneUpIDs::generatorListId))
		{
			genStr = gen[TuneUpIDs::dynamicGenMaxId].toString();
			if (genStr.length() > maxGen.length())
				maxGen = genStr;
		}

		return font.getStringWidth(maxGen);
	}
	}
}

String DynamicTuningTable::getCellTooltip(int rowNumber, int columnId)
{
	switch (columnId)
	{
	case CCNumber:
		return "The MIDI CC Number to control this generator value.";

	case CCRange:
		return "The MIDI CC value range map the generator value modification to.";

	case CCCenter:
		return "The MIDI CC value that corresponds to the generator's original value.";

	case GeneratorValue:
		return "The center generator value in cents.";

	case MinValue:
		return "The mininum value the generator can be.";

	case MaxValue:
		return "The maximum value the generator can be.";
	}

	return "";
}

void DynamicTuningTable::deleteKeyPressed(int lastRowSelected)
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

