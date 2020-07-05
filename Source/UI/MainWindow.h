/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../TuningDefinition.h"
#include "UnitGrid.h"


class TuneUpWindow  : public Component
{
public:
    //==============================================================================
    TuneUpWindow ();
    ~TuneUpWindow() override;

public:

	void loadTuning(Tuning* tuningIn);

	void setScaleNameLabel(String scaleNameIn);
	void setScaleSizeLabel(int numNotesIn);
	void setScalePeriodLabel(double periodIn);
	void setDescription(String descIn);

	//==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

	//==============================================================================

private:

	// Helpers
	UnitPlane grid;
	int stdGap = 8;

	String scaleTrans = TRANS("Scale");
	String sizeTrans = TRANS("Size");
	String periodTrans = TRANS("Period");

    //==============================================================================
	
	std::unique_ptr<Label> scaleNameLabel;
	std::unique_ptr<Label> scaleSizeLabel;
	std::unique_ptr<Label> scalePeriodLabel;

	std::unique_ptr<TextEditor> descriptionTextBox;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneUpWindow)
};
