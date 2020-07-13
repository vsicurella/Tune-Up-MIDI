/*
  ==============================================================================

    CommonUI.h
    Created: 13 Jul 2020 18:20:57pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "UnitGrid.h"
#include "../TuningDefinition.h"
#include "../TuneUpModes.h"

// Translated Text

static String nameTrans = TRANS("Name");
static String sizeTrans = TRANS("Size");
static String periodTrans = TRANS("Period");
static String generatorTrans = TRANS("Generator");
static String amountTrans = TRANS("Amount");
static String offsetTrans = TRANS("Offset");
static String rangeTrans = TRANS("Range");
static String centerTrans = TRANS("Center");
static String standardTrans = TRANS("Standard") + " (12EDO)";
static String currentTrans = TRANS("Current Tuning");
static String browseTrans = TRANS("Browse") + " ...";
static String reuseTrans = TRANS("Reuse channels when possible");
static String resetTrans = TRANS("Reset empty channel pitchbend");

// UI Helpers

static int borderGap = 8;
static int componentGap = 8;
static int valueLabelWidth = 32;

// TableListBox Helpers

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
class TableSlider : public TableComponent, public Slider {};