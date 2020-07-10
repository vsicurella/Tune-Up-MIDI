/*
  ==============================================================================

    GeneralOptionsWindow.h
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../TuningDefinition.h"
#include "../TuneUpModes.h"
#include "UnitGrid.h"

static std::function<double(int)> getStandardTuningFrequency = [](int noteIn)
{
	return pow(2, (noteIn - 69) / 12.0) * 440.0;
};



using namespace TuneUpMode;

class GeneralOptionsWindow :	public Component,
								private Button::Listener,
								private ComboBox::Listener,
								private Slider::Listener,
								private TextEditor::Listener
{

public:

	// Probably should move elsewhere
	const ValueTree STD_TUNING = TuningDefinition::getStandardTuningDefinition();

	GeneralOptionsWindow();
	~GeneralOptionsWindow();

	void paint(Graphics& g) override;

	void resized() override;

	//================================================================================

	void buttonClicked(Button* btnClicked) override;

	void comboBoxChanged(ComboBox* comboBoxThatChanged) override;

	void sliderValueChanged(Slider* sliderThatChanged) override;

	void textEditorReturnKeyPressed(TextEditor& source) override;

	//================================================================================
	
	/*
		Initializes all available options, and sets them to factory default if necessary.
		Returns true if the passed in optionsIn node was valid and synchronized.
		If false, you'll need to synchronize the trees again.
	*/
	bool initializeOptions(ValueTree optionsIn);

	void loadSessionOptions(ValueTree sessionOptionsIn);

	ValueTree getDefaultOptionsNode();

	ValueTree getSessionOptionsNode();

	/*
		Checks if the passed in File is valid, and sets the tuning path if it is
	*/

	void setDefaultTuningPath(File pathFileIn, bool notifyListeners = true);

	void setTuningIn(ValueTree tuningInDefinition, bool notifyListeners = true);

	void setTuningOut(ValueTree tuningOutDefinition, bool notifyListeners = true);

	void setReferenceNoteIn(int noteIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setReferenceFreqIn(double freqIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setReferenceNoteOut(int noteIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setReferenceFreqOut(double freqIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setPitchbendRange(int pitchbendRangeIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setChannelConfiguration(/* TODO */ bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setChannelMode(TuneUpMode::FreeChannelMode modeIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setVoiceLimit(int limitIn, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setResetChannelPitchbend(bool resetChannels, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setReuseChannels(bool reuseChannels, bool updateUI = true, bool saveAsDefault = false, bool notifyListeners = true);

	void setDynamicTuningMode(bool isDynamicTuning);

	//================================================================================

	class Listener
	{
	public:

		virtual ~Listener() {};

		virtual void defaultTuningDirectoryChanged(String directoryPath) {};

		virtual void tuningInChanged(String tuningInPath) {};

		virtual void tuningOutChanged(String tuningOutPath) {};

		virtual void referenceNoteInChanged(int noteIn) {};

		virtual void referenceFreqInChanged(double freqIn) {};

		virtual void referenceNoteOutChanged(int noteIn) {};

		virtual void referenceFreqOutChanged(double freqIn) {};

		virtual void pitchbendRangeChanged(int pitchbendRangeIn) {};

		virtual void channelConfigurationChanged(/* TODO */) {};

		virtual void channelModeChanged(TuneUpMode::FreeChannelMode modeIn) {};

		virtual void voiceLimitChanged(int limitIn) {};

		virtual void resetChannelPitchbendChanged(bool resetChannels) {};

		virtual void reuseChannelsChanged(bool reuseChannels) {};

		virtual void dynamicTuningModeChanged(bool isDynamicTuning) {};
	};

	void addListener(Listener* listenerIn) { listeners.add(listenerIn); }

	void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

	ListenerList<Listener> listeners;


private:

	Font font;

	ValueTree defaultOptionsNode;
	ValueTree sessionOptionsNode;

	ValueTree defaultTuningsListNode;

	std::unique_ptr<Label> defaultTuningDirLabel;
	std::unique_ptr<TextEditor> defaultTuningDirEditor;
	std::unique_ptr<TextButton> defaultTuningDirButton;
	
	std::unique_ptr<Label> defaultTuningInLabel;
	std::unique_ptr<ComboBox> defaultTuningInBox;

	std::unique_ptr<Label> defaultTuningOutLabel;
	std::unique_ptr<ComboBox> defaultTuningOutBox;

	std::unique_ptr<Label> referenceNoteInLabel;
	std::unique_ptr<Slider> referenceNoteInSlider;

	std::unique_ptr<Label> referenceFreqInLabel;
	std::unique_ptr<Slider> referenceFreqInSlider;
	std::unique_ptr<TextButton> referenceFreqInAutoBtn;

	std::unique_ptr<Label> referenceNoteOutLabel;
	std::unique_ptr<Slider> referenceNoteOutSlider;

	std::unique_ptr<Label> referenceFreqOutLabel;
	std::unique_ptr<Slider> referenceFreqOutSlider;
	std::unique_ptr<TextButton> referenceFreqOutAutoBtn;

	std::unique_ptr<Label> pitchbendRangeLabel;
	std::unique_ptr<Slider> pitchbendRangeSlider;
	
	std::unique_ptr<Label> channelControlLabel;
	std::unique_ptr<Component /* TODO */> channelController;

	std::unique_ptr<Label> channelModeLabel;
	std::unique_ptr<ComboBox> channelModeBox;

	std::unique_ptr<Label> voiceLimitLabel;
	std::unique_ptr<Slider> voiceLimitSlider;

	std::unique_ptr<ToggleButton> reuseChannelsButton;
	std::unique_ptr<ToggleButton> resetChannelPitchbendButton;

	// TODO: Generalize this and make it Common
	Array<Identifier> availableOptions = 
	{
		TuneUpIDs::defaultTuningFilePathId,
		TuneUpIDs::tuningsListId,
		TuneUpIDs::referenceNoteInId,
		TuneUpIDs::referenceFreqInId,
		TuneUpIDs::referenceNoteOutId,
		TuneUpIDs::referenceFreqOutId,
		TuneUpIDs::pitchbendRangeId,
		TuneUpIDs::channelConfigurationId,
		TuneUpIDs::channelModeId,
		TuneUpIDs::voiceLimitId,
		TuneUpIDs::reuseChannelsId,
		TuneUpIDs::resetChannelPitchbendId
	};

	// Helpers
	UnitPlane grid;
	int stdGap = 8;

	String standardTrans = TRANS("Standard") + " (12EDO)";
	String currentTrans = TRANS("Current Tuning");
	String browseTrans = TRANS("Browse") + " ...";
	String reuseTrans = TRANS("Reuse channels when possible");
	String resetTrans = TRANS("Reset empty channel pitchbend");

private:

	void ensureNodeHasTuningListNode(ValueTree nodeIn);
};