/*
  ==============================================================================

    GeneralOptionsWindow.h
    Created: 5 Jul 2020 6:50:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "UnitGrid.h"

static std::function<double(int)> getStandardTuningFrequency = [=](int noteIn)
{
	return pow(2, (noteIn - 69) / 12.0) * 440.0;
};

class GeneralOptionsWindow : public Component
{

public:

	enum FreeChannelMode {
		RoundRobin = 1,
		FirstAvailable
	};

public:

	GeneralOptionsWindow(ValueTree defaultOptions);
	~GeneralOptionsWindow();

	void paint(Graphics& g) override;

	void resized() override;

	//================================================================================
	
	void loadOptions(ValueTree optionsIn, bool factoryOptionIfEmpty = false);

	ValueTree getOptionsNode();

	//================================================================================

	class Listener
	{
	public:


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


private:

	Font font;
	ValueTree generalOptionsNode;

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

	// Helpers
	UnitPlane grid;
	int stdGap = 8;

	String standardTrans = TRANS("Standard") + " (12EDO)";
	String currentTrans = TRANS("Current Tuning");
	String browseTrans = TRANS("Browse") + " ...";
	String reuseTrans = TRANS("Reuse channels when possible");
	String resetTrans = TRANS("Reset empty channel pitchbend");
};