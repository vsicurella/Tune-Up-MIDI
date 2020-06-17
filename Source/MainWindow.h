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
#include "Tuning.h"
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

	Tuning& getTuning();

	void setViewMode(ViewMode modeIn);
	void setLogs(String* inLog, String* outLog, String* retuneLog);
	void updateText();

	void setNumNotes(int numNotesIn);
	void setDescription(String descIn);
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

	Tuning tuning;

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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneUpWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

