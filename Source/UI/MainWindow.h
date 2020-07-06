/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#pragma once
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

	void setTuningNameLabel(String nameIn);
	void setTuningSizeLabel(int numNotesIn);
	void setTuningPeriodLabel(double periodIn);
	void setDescription(String descIn);

	//==============================================================================

    void paint (Graphics& g) override;
    void resized() override;

	//==============================================================================

private:

	Font font;

	// Helpers
	UnitGrid grid;
	int stdGap = 8;
	int labelWidth = 32;

	String nameTrans = TRANS("Name");
	String sizeTrans = TRANS("Size");
	String periodTrans = TRANS("Period");

    //==============================================================================
	
	std::unique_ptr<Label> tuningNameLabel;
	std::unique_ptr<Label> tuningNameBox;
	std::unique_ptr<Label> tuningSizeLabel;
	std::unique_ptr<Label> tuningSizeBox;
	std::unique_ptr<Label> tuningPeriodLabel;
	std::unique_ptr<Label> tuningPeriodBox;

	std::unique_ptr<TextEditor> descriptionTextBox;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneUpWindow)
};
