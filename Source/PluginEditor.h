/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ScalaFileReader.h"
#include "UI/MainWindow.h"
#include "UI/CreateScaleWindow.h"
#include "UI/GeneralOptionsWindow.h"
#include "UI/DynamicOptionsWindow.h"
#include "UI/ButtonBar.h"
#include "UI/UnitGrid.h"


//==============================================================================
/**
*/
class TuneupMidiAudioProcessorEditor  :	public AudioProcessorEditor, 
										private Button::Listener,
										private ComboBox::Listener,
										private GeneralOptionsWindow::Listener,
										private ChangeListener
{
public:

	enum ControlMode
	{
		MainWindowMode = 0,
		NewTuningMode,
		LoadTuningMode,
		GeneralOptions,
		ToolbarOptions,
		DynamicOptions
	};

public:
    TuneupMidiAudioProcessorEditor (TuneupMidiAudioProcessor&, TuneUpMidiProcessor&, TuneUpMidiState&);
    ~TuneupMidiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void buttonClicked(Button* buttonClicked) override;
	void comboBoxChanged(ComboBox* comboBoxThatChanged) override;

	//==============================================================================

	// GeneralOptionsWindow::Listener implementation

	//void defaultTuningDirectoryChanged(String directoryPath) override;

	void referenceNoteInChanged(int noteIn) override;

	void referenceFreqInChanged(double freqIn) override;

	void referenceNoteOutChanged(int noteIn) override;

	void referenceFreqOutChanged(double freqIn) override;
	
	void pitchbendRangeChanged(int newPitchBendRange) override;

	void voiceLimitChanged(int limitIn) override;

	void channelModeChanged(FreeChannelMode channelModeIn) override;

	void reuseChannelsChanged(bool reuseChannels) override;

	void resetChannelPitchbendChanged(bool resetPitchbend) override;
	
	//==============================================================================

	void onNewTuning();

	void onFileLoad();

	void reloadPluginState();

	void loadTuningIntoState(ValueTree tuningDefinition);

	void setControlMode(ControlMode modeIn);

private:
    // Functionality
    TuneupMidiAudioProcessor& processor;
	TuneUpMidiProcessor& midiProcessor;
	TuneUpMidiState& pluginState;

	File loadedFile;
	ScalaFileReader scalaFileReader;

	ValueTree lastTuningDefinition;
	ControlMode currentMode = MainWindowMode;

	// Control windows
	std::unique_ptr<TuneUpWindow> mainWindow;
	std::unique_ptr<CreateTuningWindow> createTuningWindow;
	std::unique_ptr<Component /* TODO */> tuningBrowserWindow;
	std::unique_ptr<GeneralOptionsWindow> generalOptionsWindow;
	std::unique_ptr<Component /* TODO */> toolboxOptionsWindow;
	std::unique_ptr<DynamicOptionsWindow> dynamicOptionsWindow;

	Array<Component*> controlWindows;

	// Button Bar
	// Main Window
	std::unique_ptr<TextButton> newTuningButton;
	std::unique_ptr<TextButton> loadTuningButton;
	std::unique_ptr<TextButton> viewButton;
	std::unique_ptr<TextButton> optionsButton;
	std::unique_ptr<TextButton> dynamicToggleButton;
	std::unique_ptr<TextButton> dynamicOptionsButton;
	// Create Scale
	std::unique_ptr<TextButton> backButton;
	std::unique_ptr<TextButton> saveButton;
	std::unique_ptr<ComboBox> newTuningModeBox;
	// Options
	std::unique_ptr<TextButton> generalOptionsButton;
	std::unique_ptr<TextButton> toolboxButton;

	Array<Component*> buttonBarComponents;
	Array<ButtonBar> buttonBars;

	// Toolbar
	// TODO

	// TEMP
	std::unique_ptr<Label> voiceLimitTitle;
	std::unique_ptr<Label> voiceLimitValue;
	std::unique_ptr<Label> channelsOnTitle;
	std::unique_ptr<Label> channelsOnValue;

	// Helpers
	int borderGap = 8;
	int componentGap = 8;
	int barHeight;
	UnitGrid grid;

	String backTrans = TRANS("Back");
	String saveTrans = TRANS("Save");
	String viewTrans = TRANS("View");
	String generalTrans = TRANS("General");
	String toolsTrans = TRANS("Tools");


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuneupMidiAudioProcessorEditor)
};
