/*
  ==============================================================================

    TuneUpMidiProcessor.h
    Created: 10 Jun 2020 11:14:48am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MidiCCListener.h"
#include "MidiNoteTuner.h"
#include "ChannelAssigner.h"
#include "TuningDefinition.h"
#include "TuneUpModes.h"
#include "IDs.h"

using namespace TuneUpMode;

class TuneUpMidiProcessor : 
	public MidiMessageCollector, 
	public MidiCCNotifier,
	public ChangeBroadcaster
{

public:

	TuneUpMidiProcessor(const Tuning* originTuning, const Tuning* newTuning, Array<int>& notesOn);
	~TuneUpMidiProcessor();

	const Array<int>& getTuningNotesOn() const;

	int getNumberOfNotesOn() const;

	const Array<int>& getChannelsOn() const;

	const Array<int>& getChannelsPitchbend() const;

	void setTuningIn(const Tuning* tuningInIn);
	void setTuningOut(const Tuning* tuningOutIn);

	void setReferenceNoteIn(int noteIn);

	void setReferenceFreqIn(double freqIn);

	void setReferenceNoteOut(int noteOut);

	void setReferenceFreqOut(double freqOut);

	void setPitchbendRange(int pitchbendRangeIn);

	void setVoiceLimit(int voiceLimitIn);

	void setFreeChannelMode(FreeChannelMode channelModeIn);

	void setChannelConfiguration(ValueTree channelPropertiesNodeIn);

	void setReuseChannels(bool reuseChannels);

	void setResetChannelPitchbendWhenEmpty(bool resetPitchbend);

	void processMidi(MidiBuffer& bufferIn);

	void resetNotes();

	String* getMidiInLog();
	String* getMidiOutLog();
	String* getRetunerLog();

	//========================================================================================

	// MidiInputCallback implementation
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;

	//========================================================================================

	class Listener
	{
	public:
		virtual void ccValueChanged(int controlNumber, int controlValue) = 0;
	};

	void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
	void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

	ListenerList<Listener> listeners;


private:
	MidiDeviceInfo inputDeviceInfo = MidiInput::getDefaultDevice();
	std::unique_ptr<MidiInput> midiInput;

	MidiDeviceInfo outputDeviceInfo = MidiOutput::getDefaultDevice();
	std::unique_ptr<MidiOutput> midiOutput;

	MidiBuffer inputBuffer;
	int smplInput = 0;

	MidiBuffer ccInput;
	int ccSmpl = 0;

	//std::unique_ptr<MPEInstrument> mpeInst;

	Array<int> channelsToSkip;
	
	TuneUpMidiChannelAssigner channelAssigner;
	std::unique_ptr<MidiNoteTuner> retuner;

	String desc;
	String midiInLog;
	String midiOutLog;
	String retunerLog;

	// Options
	int transposeDegrees = 0;
	int transposePeriods = 0;
	int pitchbendRange = 2;
	PitchbendInputMode pitchbendInputMode = PitchbendInputMode::AddToTuning;

	bool resetChannelPitchbendWhenEmpty = false;

	Array<int>& notesInOn;
	Array<int> notesTunedOn;

	const Tuning* tuningIn;
	const Tuning* tuningOut = nullptr;
};