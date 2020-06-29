/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "ScalaFileReader.h"
#include "TuningDefinition.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TuneUpWindow  : public Component,
                      public ChangeBroadcaster,
                      public ComboBox::Listener,
                      public Button::Listener,
                      public Slider::Listener
{
public:
    //==============================================================================
    TuneUpWindow ();
    ~TuneUpWindow() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

public:

	enum ViewMode {
		ScaleFile = 0,
		MidiIn = 1,
		MidiOut = 2,
		Retuner = 3,
	};

public:


	void onFileLoad();
	void loadTuning(Tuning* tuningIn);

	ValueTree getTuning();

	Value* getPitchbendRange();

	void setViewMode(ViewMode modeIn);
	void setLogs(String* inLog, String* outLog, String* retuneLog);
	void updateText();

	void setNumNotes(int numNotesIn);
	void setDescription(String descIn);

	//==============================================================================

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	File loadedFile;
	String fileReadout;

	String* midiInLog = nullptr;
	String* midiOutLog = nullptr;
	String* retunerLog = nullptr;

	ScalaFileReader scalaFileReader;
	ViewMode mode = ScaleFile;

	ValueTree stagedTuning;

	Value pitchbendRange = Value(var(48));

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> tuningText;
    std::unique_ptr<TextEditor> descText;
    std::unique_ptr<ComboBox> viewModeMenu;
    std::unique_ptr<TextButton> loadBtn;
    std::unique_ptr<Label> descriptionLbl;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> numNotesLbl;
    std::unique_ptr<Slider> transposeSld;
    std::unique_ptr<Slider> periodSld;
    std::unique_ptr<Label> transposeLbl;
    std::unique_ptr<Label> periodLbl;
    std::unique_ptr<ComboBox> comboBox;
    std::unique_ptr<TextEditor> fileNameText;
    std::unique_ptr<TextButton> textButton;
    std::unique_ptr<Slider> rootMidiNoteSld;
    std::unique_ptr<Label> generateLbl;
    std::unique_ptr<Slider> periodSlider;
    std::unique_ptr<Slider> genSlider;
    std::unique_ptr<Slider> genMaxSld;
    std::unique_ptr<Slider> rankNumSld;
    std::unique_ptr<Slider> scaleSizeSld;
    std::unique_ptr<Slider> genDownSld;
    std::unique_ptr<TextButton> generateTuningBtn;
    std::unique_ptr<Slider> pitchbendRangeSld;
    std::unique_ptr<Label> periodGenLbl;
    std::unique_ptr<Label> genGenLbl;
    std::unique_ptr<Label> genGenMaxLbl;
    std::unique_ptr<Label> genDownLbl;
    std::unique_ptr<Label> rankLbl;
    std::unique_ptr<Label> sizeGenLbl;
    std::unique_ptr<Label> rangeLbl;
    std::unique_ptr<Slider> expCCNumSld;
    std::unique_ptr<Label> ccnumLbl;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneUpWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

