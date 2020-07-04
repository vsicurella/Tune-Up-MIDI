/*
  ==============================================================================

  MainWindow.cpp

  Created by Vincenzo Sicurella on 6/30/2020

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ScalaFileReader.h"
#include "../TuningDefinition.h"
#include "ButtonBar.h"
#include "UnitGrid.h"


class TuneUpWindow  : public Component,
                      public Button::Listener
{
public:
    //==============================================================================
    TuneUpWindow ();
    ~TuneUpWindow() override;

public:


	void onFileLoad();
	void loadTuning(Tuning* tuningIn);

	ValueTree getTuning();

	void setScaleNameLabel(String scaleNameIn);
	void setScaleSizeLabel(int numNotesIn);
	void setScalePeriodLabel(double periodIn);
	void setDescription(String descIn);

	//==============================================================================

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

	//==============================================================================

	class Listener
	{
	public:

		virtual void scaleLoaded(ValueTree tuningDefinition) = 0;
		virtual void newButtonClicked() = 0;
		virtual void optionsButtonClicked() = 0;
		virtual void dynamicOptionsClicked() = 0;
	};

	void addListener(Listener* listenerIn)
	{
		listeners.add(listenerIn);
	}

	void removeListener(Listener* listenerIn)
	{
		listeners.remove(listenerIn);
	}


protected:

	ListenerList<Listener> listeners;

	//==============================================================================


private:
	File loadedFile;

	ScalaFileReader scalaFileReader;

	ValueTree stagedTuning;

	// Helpers
	UnitGrid unitGrid;
	ButtonBar btnBar;
	int btnBarWidth;
	//int labelSectHeight;
	int stdGap = 8;

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
