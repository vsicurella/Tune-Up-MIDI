/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ScalaFileReader.h"
#include "TuningDefinition.h"


class TuneUpWindow  : public Component,
                      public ChangeBroadcaster,
                      public Button::Listener,
                      public Slider::Listener
{
public:
    //==============================================================================
    TuneUpWindow ();
    ~TuneUpWindow() override;

public:


	void onFileLoad();
	void loadTuning(Tuning* tuningIn);

	ValueTree getTuning();

	Value* getPitchbendRange();

	void setScaleNameLabel(String scaleNameIn);
	void setScaleSizeLabel(int numNotesIn);
	void setScalePeriodLabel(double periodIn);
	void setDescription(String descIn);

	//==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

private:
	File loadedFile;

	String scaleName;
	String scaleDescription;

	ScalaFileReader scalaFileReader;

	ValueTree stagedTuning;

	Value pitchbendRange = Value(var(48));

	// Helpers

	int btnBarWidth;
	int labelSectHeight;
	int stdGap = 8;
	int stdBorder = 8;
	Array<int> btnUnit = { 3, 4, 4, 5, 5, 2 };
	int btnUnitDenom = sumArray(btnUnit);

	const String scaleTrans = TRANS("Scale");
	const String sizeTrans = TRANS("Size");
	const String periodTrans = TRANS("Period");

    //==============================================================================
    
	std::unique_ptr<TextButton> newScaleButton;
	std::unique_ptr<TextButton> loadScaleButton;
	std::unique_ptr<TextButton> viewButton;
	std::unique_ptr<TextButton> optionsButton;
	std::unique_ptr<TextButton> dynamicToggleButton;
	std::unique_ptr<TextButton> dynamicOptionsButton;
	
	std::unique_ptr<Label> scaleNameLabel;
	std::unique_ptr<Label> scaleSizeLabel;
	std::unique_ptr<Label> scalePeriodLabel;

	std::unique_ptr<TextEditor> descriptionTextBox;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneUpWindow)
};
