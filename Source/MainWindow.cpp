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


    //[UserPreSize]
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

    tuningText->setBounds (16, 104 + proportionOfHeight (0.1082f) - -8, proportionOfWidth (0.9662f), proportionOfHeight (0.7198f));
    descText->setBounds (16, 104, proportionOfWidth (0.9662f), proportionOfHeight (0.1082f));
    viewModeMenu->setBounds (16, getHeight() - 30, 150, 24);
    descriptionLbl->setBounds (16, 76, proportionOfWidth (0.3251f), 24);
    numNotesLbl->setBounds (16, 48, proportionOfWidth (0.5038f), 24);
    transposeSld->setBounds (16 + proportionOfWidth (0.9662f) - 152, 72, 154, 24);
    periodSld->setBounds (16 + proportionOfWidth (0.9662f) - 152, 44, 154, 24);
    transposeLbl->setBounds (16 + proportionOfWidth (0.9662f) - 248, 72, 86, 24);
    periodLbl->setBounds (16 + proportionOfWidth (0.9662f) - 248, 44, 86, 24);
    comboBox->setBounds (getWidth() - 396, getHeight() - 30, 224, 24);
    fileNameText->setBounds (216, 16, proportionOfWidth (0.8176f), 24);
    textButton->setBounds (getWidth() - 156, getHeight() - 30, 128, 24);
    rootMidiNoteSld->setBounds (proportionOfWidth (0.2402f), 848, 150, 24);
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

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

Tuning& TuneUpWindow::getTuning()
{
	return tuning;
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
	fileReadout = "";
	if (success)
	{
		ScalaFile file = scalaFileReader.getScalaFile();

		setNumNotes(file.numNotes);
		setDescription(file.description);

		fileReadout = "";
		for (auto c : file.cents)
		{
			fileReadout += String(c) + '\n';
		}

		tuning = Tuning(file.cents);
		sendChangeMessage();
	}
	else
	{
		fileReadout = "Error reading file.";
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
              virtualName="" explicitFocusOrder="0" pos="16 -8R 96.622% 71.982%"
              posRelativeY="399cc76727f961ef" initialText="" multiline="1"
              retKeyStartsLine="1" readonly="0" scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTEDITOR name="descText" id="399cc76727f961ef" memberName="descText" virtualName=""
              explicitFocusOrder="0" pos="16 104 96.622% 10.82%" initialText=""
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
         virtualName="" explicitFocusOrder="0" pos="16 76 32.508% 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Description:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="99c8c0b56baefb57" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="136 16 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="File Name: " editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="numNotesLbl" id="d950d130413305d3" memberName="numNotesLbl"
         virtualName="" explicitFocusOrder="0" pos="16 48 50.375% 24"
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
              virtualName="" explicitFocusOrder="0" pos="216 16 81.757% 24"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="5d824f0479d00288" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="156R 30R 128 24" buttonText="Mapping Settings"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="rootMidiNoteSld" id="4f7c3b0530c630a4" memberName="rootMidiNoteSld"
          virtualName="" explicitFocusOrder="0" pos="24.024% 848 150 24"
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

