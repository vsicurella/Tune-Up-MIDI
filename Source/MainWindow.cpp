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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TuneUpWindow::TuneUpWindow ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    tuningText.reset (new TextEditor ("tuningText"));
    addAndMakeVisible (tuningText.get());
    tuningText->setMultiLine (true);
    tuningText->setReturnKeyStartsNewLine (true);
    tuningText->setReadOnly (false);
    tuningText->setScrollbarsShown (true);
    tuningText->setCaretVisible (true);
    tuningText->setPopupMenuEnabled (true);
    tuningText->setText (String());

    descText.reset (new TextEditor ("descText"));
    addAndMakeVisible (descText.get());
    descText->setMultiLine (true);
    descText->setReturnKeyStartsNewLine (false);
    descText->setReadOnly (false);
    descText->setScrollbarsShown (true);
    descText->setCaretVisible (true);
    descText->setPopupMenuEnabled (true);
    descText->setText (String());

    viewModeMenu.reset (new ComboBox ("viewModeMenu"));
    addAndMakeVisible (viewModeMenu.get());
    viewModeMenu->setEditableText (false);
    viewModeMenu->setJustificationType (Justification::centredLeft);
    viewModeMenu->setTextWhenNothingSelected (TRANS("View Mode"));
    viewModeMenu->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    viewModeMenu->addItem (TRANS("Tuning"), 1);
    viewModeMenu->addItem (TRANS("MIDI input"), 2);
    viewModeMenu->addItem (TRANS("MIDI output"), 3);
    viewModeMenu->addListener (this);

    loadBtn.reset (new TextButton ("loadBtn"));
    addAndMakeVisible (loadBtn.get());
    loadBtn->setButtonText (TRANS("Load"));
    loadBtn->addListener (this);

    loadBtn->setBounds (16, 16, 112, 24);

    descriptionLbl.reset (new Label ("descriptionLbl",
                                     TRANS("Description:")));
    addAndMakeVisible (descriptionLbl.get());
    descriptionLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    descriptionLbl->setJustificationType (Justification::centredLeft);
    descriptionLbl->setEditable (false, false, false);
    descriptionLbl->setColour (TextEditor::textColourId, Colours::black);
    descriptionLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label.reset (new Label ("new label",
                            TRANS("File Name: ")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (136, 16, 80, 24);

    numNotesLbl.reset (new Label ("numNotesLbl",
                                  TRANS("Scale Size: ")));
    addAndMakeVisible (numNotesLbl.get());
    numNotesLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    numNotesLbl->setJustificationType (Justification::centredLeft);
    numNotesLbl->setEditable (false, false, false);
    numNotesLbl->setColour (TextEditor::textColourId, Colours::black);
    numNotesLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    transposeSld.reset (new Slider ("transposeSld"));
    addAndMakeVisible (transposeSld.get());
    transposeSld->setRange (-128, 128, 1);
    transposeSld->setSliderStyle (Slider::IncDecButtons);
    transposeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    transposeSld->addListener (this);

    periodSld.reset (new Slider ("periodSld"));
    addAndMakeVisible (periodSld.get());
    periodSld->setRange (-16, 16, 1);
    periodSld->setSliderStyle (Slider::IncDecButtons);
    periodSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    periodSld->addListener (this);

    transposeLbl.reset (new Label ("transposeLbl",
                                   TRANS("Transpose:")));
    addAndMakeVisible (transposeLbl.get());
    transposeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    transposeLbl->setJustificationType (Justification::centredRight);
    transposeLbl->setEditable (false, false, false);
    transposeLbl->setColour (TextEditor::textColourId, Colours::black);
    transposeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    periodLbl.reset (new Label ("periodLbl",
                                TRANS("Period:")));
    addAndMakeVisible (periodLbl.get());
    periodLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    periodLbl->setJustificationType (Justification::centredRight);
    periodLbl->setEditable (false, false, false);
    periodLbl->setColour (TextEditor::textColourId, Colours::black);
    periodLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    comboBox.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (comboBox.get());
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addItem (TRANS("Multichannel MPE"), 1);
    comboBox->addItem (TRANS("Round Robin MPE"), 2);
    comboBox->addItem (TRANS("Scala KBM"), 3);
    comboBox->addListener (this);

    fileNameText.reset (new TextEditor ("fileNameText"));
    addAndMakeVisible (fileNameText.get());
    fileNameText->setMultiLine (false);
    fileNameText->setReturnKeyStartsNewLine (false);
    fileNameText->setReadOnly (false);
    fileNameText->setScrollbarsShown (true);
    fileNameText->setCaretVisible (true);
    fileNameText->setPopupMenuEnabled (true);
    fileNameText->setText (String());

    textButton.reset (new TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("Mapping Settings"));
    textButton->addListener (this);

    rootMidiNoteSld.reset (new Slider ("rootMidiNoteSld"));
    addAndMakeVisible (rootMidiNoteSld.get());
    rootMidiNoteSld->setRange (0, 127, 1);
    rootMidiNoteSld->setSliderStyle (Slider::IncDecButtons);
    rootMidiNoteSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    rootMidiNoteSld->addListener (this);

    generateLbl.reset (new Label ("new label",
                                  TRANS("Generate Tuning")));
    addAndMakeVisible (generateLbl.get());
    generateLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    generateLbl->setJustificationType (Justification::centredLeft);
    generateLbl->setEditable (false, false, false);
    generateLbl->setColour (TextEditor::textColourId, Colours::black);
    generateLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    periodSlider.reset (new Slider ("periodSlider"));
    addAndMakeVisible (periodSlider.get());
    periodSlider->setRange (0.001, 100000, 0.001);
    periodSlider->setSliderStyle (Slider::LinearHorizontal);
    periodSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    periodSlider->addListener (this);

    genSlider.reset (new Slider ("new slider"));
    addAndMakeVisible (genSlider.get());
    genSlider->setRange (0.001, 10000, 0.001);
    genSlider->setSliderStyle (Slider::LinearHorizontal);
    genSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    genSlider->addListener (this);

    genMaxSld.reset (new Slider ("new slider"));
    addAndMakeVisible (genMaxSld.get());
    genMaxSld->setRange (0.001, 100000, 0.001);
    genMaxSld->setSliderStyle (Slider::LinearHorizontal);
    genMaxSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    genMaxSld->addListener (this);

    rankNumSld.reset (new Slider ("new slider"));
    addAndMakeVisible (rankNumSld.get());
    rankNumSld->setRange (1, 2, 1);
    rankNumSld->setSliderStyle (Slider::IncDecButtons);
    rankNumSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    rankNumSld->addListener (this);

    scaleSizeSld.reset (new Slider ("new slider"));
    addAndMakeVisible (scaleSizeSld.get());
    scaleSizeSld->setRange (1, 128, 1);
    scaleSizeSld->setSliderStyle (Slider::IncDecButtons);
    scaleSizeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    scaleSizeSld->addListener (this);

    genDownSld.reset (new Slider ("new slider"));
    addAndMakeVisible (genDownSld.get());
    genDownSld->setRange (0, 128, 1);
    genDownSld->setSliderStyle (Slider::IncDecButtons);
    genDownSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    genDownSld->addListener (this);

    generateTuningBtn.reset (new TextButton ("new button"));
    addAndMakeVisible (generateTuningBtn.get());
    generateTuningBtn->setButtonText (TRANS("<< GENERATE!"));
    generateTuningBtn->addListener (this);

    pitchbendRangeSld.reset (new Slider ("new slider"));
    addAndMakeVisible (pitchbendRangeSld.get());
    pitchbendRangeSld->setRange (0, 96, 0.001);
    pitchbendRangeSld->setSliderStyle (Slider::LinearHorizontal);
    pitchbendRangeSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    pitchbendRangeSld->addListener (this);

    periodGenLbl.reset (new Label ("new label",
                                   TRANS("Period")));
    addAndMakeVisible (periodGenLbl.get());
    periodGenLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    periodGenLbl->setJustificationType (Justification::centredLeft);
    periodGenLbl->setEditable (false, false, false);
    periodGenLbl->setColour (TextEditor::textColourId, Colours::black);
    periodGenLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    genGenLbl.reset (new Label ("new label",
                                TRANS("Generator (Min)")));
    addAndMakeVisible (genGenLbl.get());
    genGenLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    genGenLbl->setJustificationType (Justification::centredLeft);
    genGenLbl->setEditable (false, false, false);
    genGenLbl->setColour (TextEditor::textColourId, Colours::black);
    genGenLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    genGenMaxLbl.reset (new Label ("new label",
                                   TRANS("Gen (Max)")));
    addAndMakeVisible (genGenMaxLbl.get());
    genGenMaxLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    genGenMaxLbl->setJustificationType (Justification::centredLeft);
    genGenMaxLbl->setEditable (false, false, false);
    genGenMaxLbl->setColour (TextEditor::textColourId, Colours::black);
    genGenMaxLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    genDownLbl.reset (new Label ("new label",
                                 TRANS("Gen\'s Down")));
    addAndMakeVisible (genDownLbl.get());
    genDownLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    genDownLbl->setJustificationType (Justification::centredLeft);
    genDownLbl->setEditable (false, false, false);
    genDownLbl->setColour (TextEditor::textColourId, Colours::black);
    genDownLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rankLbl.reset (new Label ("new label",
                              TRANS("Rank")));
    addAndMakeVisible (rankLbl.get());
    rankLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rankLbl->setJustificationType (Justification::centredLeft);
    rankLbl->setEditable (false, false, false);
    rankLbl->setColour (TextEditor::textColourId, Colours::black);
    rankLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    sizeGenLbl.reset (new Label ("new label",
                                 TRANS("Size")));
    addAndMakeVisible (sizeGenLbl.get());
    sizeGenLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    sizeGenLbl->setJustificationType (Justification::centredLeft);
    sizeGenLbl->setEditable (false, false, false);
    sizeGenLbl->setColour (TextEditor::textColourId, Colours::black);
    sizeGenLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    rangeLbl.reset (new Label ("new label",
                               TRANS("Pitchbend Range")));
    addAndMakeVisible (rangeLbl.get());
    rangeLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    rangeLbl->setJustificationType (Justification::centredLeft);
    rangeLbl->setEditable (false, false, false);
    rangeLbl->setColour (TextEditor::textColourId, Colours::black);
    rangeLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    expCCNumSld.reset (new Slider ("new slider"));
    addAndMakeVisible (expCCNumSld.get());
    expCCNumSld->setRange (0, 128, 1);
    expCCNumSld->setSliderStyle (Slider::IncDecButtons);
    expCCNumSld->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    expCCNumSld->addListener (this);

    ccnumLbl.reset (new Label ("new label",
                               TRANS("Expression CC Channel")));
    addAndMakeVisible (ccnumLbl.get());
    ccnumLbl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    ccnumLbl->setJustificationType (Justification::centredLeft);
    ccnumLbl->setEditable (false, false, false);
    ccnumLbl->setColour (TextEditor::textColourId, Colours::black);
    ccnumLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	periodSlider->setValue(1200);
	genSlider->setValue(696.0);
	genMaxSld->setValue(709.0);
	genDownSld->setValue(1);
	rankNumSld->setValue(2);
	scaleSizeSld->setValue(12);
	pitchbendRangeSld->setValue(48);
	expCCNumSld->setValue(53);
	rootMidiNoteSld->setValue(60);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

TuneUpWindow::~TuneUpWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tuningText = nullptr;
    descText = nullptr;
    viewModeMenu = nullptr;
    loadBtn = nullptr;
    descriptionLbl = nullptr;
    label = nullptr;
    numNotesLbl = nullptr;
    transposeSld = nullptr;
    periodSld = nullptr;
    transposeLbl = nullptr;
    periodLbl = nullptr;
    comboBox = nullptr;
    fileNameText = nullptr;
    textButton = nullptr;
    rootMidiNoteSld = nullptr;
    generateLbl = nullptr;
    periodSlider = nullptr;
    genSlider = nullptr;
    genMaxSld = nullptr;
    rankNumSld = nullptr;
    scaleSizeSld = nullptr;
    genDownSld = nullptr;
    generateTuningBtn = nullptr;
    pitchbendRangeSld = nullptr;
    periodGenLbl = nullptr;
    genGenLbl = nullptr;
    genGenMaxLbl = nullptr;
    genDownLbl = nullptr;
    rankLbl = nullptr;
    sizeGenLbl = nullptr;
    rangeLbl = nullptr;
    expCCNumSld = nullptr;
    ccnumLbl = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TuneUpWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void TuneUpWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tuningText->setBounds (16, 104 + proportionOfHeight (0.1082f) - -8, proportionOfWidth (0.4929f), proportionOfHeight (0.7203f));
    descText->setBounds (16, 104, proportionOfWidth (0.4929f), proportionOfHeight (0.1082f));
    viewModeMenu->setBounds (16, getHeight() - 30, 150, 24);
    descriptionLbl->setBounds (16, 76, proportionOfWidth (0.3255f), 24);
    numNotesLbl->setBounds (16, 48, proportionOfWidth (0.5035f), 24);
    transposeSld->setBounds (16 + proportionOfWidth (0.4929f) - 152, 72, 154, 24);
    periodSld->setBounds (16 + proportionOfWidth (0.4929f) - 152, 44, 154, 24);
    transposeLbl->setBounds (16 + proportionOfWidth (0.4929f) - 248, 72, 86, 24);
    periodLbl->setBounds (16 + proportionOfWidth (0.4929f) - 248, 44, 86, 24);
    comboBox->setBounds (getWidth() - 396, getHeight() - 30, 224, 24);
    fileNameText->setBounds (216, 16, proportionOfWidth (0.2040f), 24);
    textButton->setBounds (getWidth() - 156, getHeight() - 30, 128, 24);
    rootMidiNoteSld->setBounds (proportionOfWidth (0.2406f), 848, 150, 24);
    generateLbl->setBounds (proportionOfWidth (0.5165f), 16, 120, 24);
    periodSlider->setBounds (proportionOfWidth (0.5224f), 56, proportionOfWidth (0.1686f), 24);
    genSlider->setBounds (proportionOfWidth (0.5224f), 104, proportionOfWidth (0.1627f), 24);
    genMaxSld->setBounds (proportionOfWidth (0.7205f), 104, proportionOfWidth (0.1745f), 24);
    rankNumSld->setBounds (proportionOfWidth (0.5224f), 160, proportionOfWidth (0.1120f), 24);
    scaleSizeSld->setBounds (proportionOfWidth (0.5224f), 208, proportionOfWidth (0.1120f), 24);
    genDownSld->setBounds (proportionOfWidth (0.7205f), 56, proportionOfWidth (0.1120f), 24);
    generateTuningBtn->setBounds (proportionOfWidth (0.5224f), 272, proportionOfWidth (0.1120f), 24);
    pitchbendRangeSld->setBounds (proportionOfWidth (0.5224f), 360, 150, 24);
    periodGenLbl->setBounds (proportionOfWidth (0.5165f), 40, 64, 24);
    genGenLbl->setBounds (proportionOfWidth (0.5224f), 88, 142, 24);
    genGenMaxLbl->setBounds (proportionOfWidth (0.7205f), 80, 114, 26);
    genDownLbl->setBounds (proportionOfWidth (0.7205f), 40, 96, 24);
    rankLbl->setBounds (proportionOfWidth (0.5224f), 144, 64, 24);
    sizeGenLbl->setBounds (proportionOfWidth (0.5224f), 192, 64, 24);
    rangeLbl->setBounds (proportionOfWidth (0.5165f), 336, 145, 24);
    expCCNumSld->setBounds (proportionOfWidth (0.5224f), 440, proportionOfWidth (0.1120f), 24);
    ccnumLbl->setBounds (proportionOfWidth (0.5165f), 416, 176, 24);
    //[UserResized] Add your own custom resize handling here..

	tuningText->setSize(proportionOfWidth(0.9670f), getHeight() - tuningText->getY() - viewModeMenu->getHeight() - 16);
	fileNameText->setBounds(216, 16, getWidth() - 222, 24);

    //[/UserResized]
}

void TuneUpWindow::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == viewModeMenu.get())
    {
        //[UserComboBoxCode_viewModeMenu] -- add your combo box handling code here..
		mode = ViewMode(viewModeMenu->getSelectedId() - 1);
		setViewMode(mode);
        //[/UserComboBoxCode_viewModeMenu]
    }
    else if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void TuneUpWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadBtn.get())
    {
        //[UserButtonCode_loadBtn] -- add your button handler code here..
		FileChooser chooser("Select your tuning file", File::getSpecialLocation(File::userDocumentsDirectory));
		chooser.browseForFileToOpen();
		loadedFile = chooser.getResult();
		fileNameText->setText(loadedFile.getFullPathName());

		onFileLoad();
        //[/UserButtonCode_loadBtn]
    }
    else if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == generateTuningBtn.get())
    {
        //[UserButtonCode_generateTuningBtn] -- add your button handler code here..
		stagedTuning = TuningDefinition::createRegularTemperamentDefinition(
			{ periodSlider->getValue(), genSlider->getValue() },
			{ 1, scaleSizeSld->getValue() },
			{ 0, genDownSld->getValue() },
			rootMidiNoteSld->getValue(),
			"Regular temperament using generators [" + String(periodSlider->getValue()) + ", " + String(genSlider->getValue()) + "]"
		);

		DBG("Staged Tuning: \n" + stagedTuning.toXmlString());

		sendChangeMessage();
        //[/UserButtonCode_generateTuningBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void TuneUpWindow::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == transposeSld.get())
    {
        //[UserSliderCode_transposeSld] -- add your slider handling code here..
        //[/UserSliderCode_transposeSld]
    }
    else if (sliderThatWasMoved == periodSld.get())
    {
        //[UserSliderCode_periodSld] -- add your slider handling code here..
        //[/UserSliderCode_periodSld]
    }
    else if (sliderThatWasMoved == rootMidiNoteSld.get())
    {
        //[UserSliderCode_rootMidiNoteSld] -- add your slider handling code here..
        //[/UserSliderCode_rootMidiNoteSld]
    }
    else if (sliderThatWasMoved == periodSlider.get())
    {
        //[UserSliderCode_periodSlider] -- add your slider handling code here..
        //[/UserSliderCode_periodSlider]
    }
    else if (sliderThatWasMoved == genSlider.get())
    {
        //[UserSliderCode_genSlider] -- add your slider handling code here..
        //[/UserSliderCode_genSlider]
    }
    else if (sliderThatWasMoved == genMaxSld.get())
    {
        //[UserSliderCode_genMaxSld] -- add your slider handling code here..
        //[/UserSliderCode_genMaxSld]
    }
    else if (sliderThatWasMoved == rankNumSld.get())
    {
        //[UserSliderCode_rankNumSld] -- add your slider handling code here..
        //[/UserSliderCode_rankNumSld]
    }
    else if (sliderThatWasMoved == scaleSizeSld.get())
    {
        //[UserSliderCode_scaleSizeSld] -- add your slider handling code here..
        //[/UserSliderCode_scaleSizeSld]
    }
    else if (sliderThatWasMoved == genDownSld.get())
    {
        //[UserSliderCode_genDownSld] -- add your slider handling code here..
        //[/UserSliderCode_genDownSld]
    }
    else if (sliderThatWasMoved == pitchbendRangeSld.get())
    {
        //[UserSliderCode_pitchbendRangeSld] -- add your slider handling code here..
		pitchbendRange = pitchbendRangeSld->getValue();
        //[/UserSliderCode_pitchbendRangeSld]
    }
    else if (sliderThatWasMoved == expCCNumSld.get())
    {
        //[UserSliderCode_expCCNumSld] -- add your slider handling code here..
        //[/UserSliderCode_expCCNumSld]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

ValueTree TuneUpWindow::getTuning()
{
	return stagedTuning;
}

Value* TuneUpWindow::getPitchbendRange()
{
	return &pitchbendRange;
}

void TuneUpWindow::setViewMode(ViewMode modeIn)
{
	mode = modeIn;
	updateText();
}

void TuneUpWindow::setLogs(String* inLog, String* outLog, String* retuneLog)
{
	midiInLog = inLog;
	midiOutLog = outLog;
	retunerLog = retuneLog;

	updateText();
}

void TuneUpWindow::updateText()
{
	switch (mode)
	{
	case TuneUpWindow::MidiIn:
		if (midiInLog)
		{
			tuningText->setText(*midiInLog);
		}
		break;

	case TuneUpWindow::MidiOut:
		if (midiOutLog)
		{
			tuningText->setText(*midiOutLog);
		}
		break;

	case TuneUpWindow::Retuner:
		break;

	default:
		tuningText->setText(fileReadout);
		break;
	}

	tuningText->scrollEditorToPositionCaret(INT_MAX, INT_MAX);
}

void TuneUpWindow::setNumNotes(int numNotesIn)
{
	numNotesLbl->setText("Scale Size: " + String(numNotesIn), dontSendNotification);
}

void TuneUpWindow::setDescription(String descIn)
{
	descText->setText(descIn);
}

void TuneUpWindow::onFileLoad()
{
	bool success = scalaFileReader.open(loadedFile);

	if (success)
	{
		ScalaFile file = scalaFileReader.getScalaFile();
		stagedTuning = TuningDefinition::createStaticTuningDefinition(file.cents, rootMidiNoteSld->getValue(), file.description);
		DBG("Staged Tuning: \n" + stagedTuning.toXmlString());
		sendChangeMessage();
	}
	else
	{
		fileReadout = "Error reading file.";
		tuningText->setText(fileReadout);
		setNumNotes(0);
	}
}

void TuneUpWindow::loadTuning(Tuning* tuningIn)
{
	setNumNotes(tuningIn->getTuningSize());
	setDescription(tuningIn->getDescription());

	fileReadout = "";
	for (auto interval : tuningIn->getIntervalCents())
	{
		fileReadout += String(interval) + '\n';
	}

	tuningText->setText(fileReadout);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TuneUpWindow" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="tuningText" id="4aa03066541604" memberName="tuningText"
              virtualName="" explicitFocusOrder="0" pos="16 -867R 49.271% 72.037%"
              posRelativeY="399cc76727f961ef" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="descText" id="399cc76727f961ef" memberName="descText" virtualName=""
              explicitFocusOrder="0" pos="16 104 49.271% 10.833%" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <COMBOBOX name="viewModeMenu" id="cf506df3da600bde" memberName="viewModeMenu"
            virtualName="" explicitFocusOrder="0" pos="16 30R 150 24" editable="0"
            layout="33" items="Tuning&#10;MIDI input&#10;MIDI output" textWhenNonSelected="View Mode"
            textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="loadBtn" id="f7f16c3ff20fb55" memberName="loadBtn" virtualName=""
              explicitFocusOrder="0" pos="16 16 112 24" buttonText="Load" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <LABEL name="descriptionLbl" id="89fa025f4e64a24c" memberName="descriptionLbl"
         virtualName="" explicitFocusOrder="0" pos="16 76 32.552% 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Description:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="99c8c0b56baefb57" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="136 16 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="File Name: " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="numNotesLbl" id="d950d130413305d3" memberName="numNotesLbl"
         virtualName="" explicitFocusOrder="0" pos="16 48 50.365% 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Scale Size: " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="transposeSld" id="bc995a138aa3f5d4" memberName="transposeSld"
          virtualName="" explicitFocusOrder="0" pos="152R 72 154 24" posRelativeX="4aa03066541604"
          min="-128.0" max="128.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="periodSld" id="e4f249a787572d48" memberName="periodSld"
          virtualName="" explicitFocusOrder="0" pos="152R 44 154 24" posRelativeX="4aa03066541604"
          min="-16.0" max="16.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="transposeLbl" id="317662b1b3e7f58" memberName="transposeLbl"
         virtualName="" explicitFocusOrder="0" pos="248R 72 86 24" posRelativeX="4aa03066541604"
         edTextCol="ff000000" edBkgCol="0" labelText="Transpose:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="periodLbl" id="c85851edfd71d83f" memberName="periodLbl"
         virtualName="" explicitFocusOrder="0" pos="248R 44 86 24" posRelativeX="4aa03066541604"
         edTextCol="ff000000" edBkgCol="0" labelText="Period:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <COMBOBOX name="new combo box" id="b4a031f59f71c43f" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="396R 30R 224 24" editable="0"
            layout="33" items="Multichannel MPE&#10;Round Robin MPE&#10;Scala KBM"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTEDITOR name="fileNameText" id="dae9e1751dd6841e" memberName="fileNameText"
              virtualName="" explicitFocusOrder="0" pos="216 16 20.417% 24"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="5d824f0479d00288" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="156R 30R 128 24" buttonText="Mapping Settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="rootMidiNoteSld" id="4f7c3b0530c630a4" memberName="rootMidiNoteSld"
          virtualName="" explicitFocusOrder="0" pos="24.062% 848 150 24"
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="ba2247d1aafe79b4" memberName="generateLbl"
         virtualName="" explicitFocusOrder="0" pos="51.667% 16 120 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Generate Tuning"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="periodSlider" id="b52e05bcf7b7942f" memberName="periodSlider"
          virtualName="" explicitFocusOrder="0" pos="52.24% 56 16.875% 24"
          min="0.001" max="100000.0" int="0.001" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="e46f8473853dc4be" memberName="genSlider"
          virtualName="" explicitFocusOrder="0" pos="52.24% 104 16.25% 24"
          min="0.001" max="10000.0" int="0.001" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="de72a2111472c4c7" memberName="genMaxSld"
          virtualName="" explicitFocusOrder="0" pos="72.031% 104 17.448% 24"
          min="0.001" max="100000.0" int="0.001" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="8f480c0c789dffc3" memberName="rankNumSld"
          virtualName="" explicitFocusOrder="0" pos="52.24% 160 11.198% 24"
          min="1.0" max="2.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="c5d1b24ac32dd291" memberName="scaleSizeSld"
          virtualName="" explicitFocusOrder="0" pos="52.24% 208 11.198% 24"
          min="1.0" max="128.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="1a8e028bc9748f63" memberName="genDownSld"
          virtualName="" explicitFocusOrder="0" pos="72.031% 56 11.198% 24"
          min="0.0" max="128.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="new button" id="8e2e7f811dcf8cfa" memberName="generateTuningBtn"
              virtualName="" explicitFocusOrder="0" pos="52.24% 272 11.198% 24"
              buttonText="&lt;&lt; GENERATE!" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="new slider" id="25e31ce31569133c" memberName="pitchbendRangeSld"
          virtualName="" explicitFocusOrder="0" pos="52.24% 360 150 24"
          min="0.0" max="96.0" int="0.001" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="3c7315c1d1d2350c" memberName="periodGenLbl"
         virtualName="" explicitFocusOrder="0" pos="51.667% 40 64 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Period" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="8985c0a44322cef8" memberName="genGenLbl"
         virtualName="" explicitFocusOrder="0" pos="52.24% 88 142 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Generator (Min)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="5421c81a4dd96acc" memberName="genGenMaxLbl"
         virtualName="" explicitFocusOrder="0" pos="72.031% 80 114 26"
         edTextCol="ff000000" edBkgCol="0" labelText="Gen (Max)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="b3fc87a02d06ccc1" memberName="genDownLbl"
         virtualName="" explicitFocusOrder="0" pos="72.031% 40 96 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Gen's Down" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="d4a9aa0ea3fcc8cf" memberName="rankLbl" virtualName=""
         explicitFocusOrder="0" pos="52.24% 144 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Rank" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="bc23b87714b43f1b" memberName="sizeGenLbl"
         virtualName="" explicitFocusOrder="0" pos="52.24% 192 64 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Size" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="848ebdbb1aace20d" memberName="rangeLbl"
         virtualName="" explicitFocusOrder="0" pos="51.667% 336 145 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Pitchbend Range"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="new slider" id="e3df021e54656e8d" memberName="expCCNumSld"
          virtualName="" explicitFocusOrder="0" pos="52.24% 440 11.198% 24"
          min="0.0" max="128.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="d9a9d6b84f4e63b2" memberName="ccnumLbl"
         virtualName="" explicitFocusOrder="0" pos="51.667% 416 176 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression CC Channel"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

