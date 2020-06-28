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
#include "DynamicTuning.h"

class TuneUpMidiProcessor : 
	public MidiMessageCollector, 
	public MidiCCNotifier,
	public ChangeBroadcaster, 
	private DynamicTuning::Listener
{
	MidiDeviceInfo inputDeviceInfo = MidiInput::getDefaultDevice();
	std::unique_ptr<MidiInput> midiInput;

	MidiDeviceInfo outputDeviceInfo = MidiOutput::getDefaultDevice();
	std::unique_ptr<MidiOutput> midiOutput;

	MidiBuffer inputBuffer;
	int smplInput = 0;

	MidiBuffer ccInput;
	int ccSmpl = 0;

	std::unique_ptr<MPEInstrument> mpeInst;
	std::unique_ptr<MidiNoteTuner> retuner;

	String desc;
	String midiInLog;
	String midiOutLog;
	String retunerLog;

	MPEInstrument mpeInstrument;
	TuneUpMidiChannelAssigner channelAssigner;

	int pitchbendRange = 48;	
	
	Array<int> notesInOn;
	Array<int> notesTunedOn;

	const Tuning standard;
	const Tuning* tuning = nullptr;

public:

	TuneUpMidiProcessor();
	~TuneUpMidiProcessor();

	const Array<int>& getTuningNotesOn() const;

	void setTuning(const Tuning* tuningIn);
	void setPitchbendRange(int pitchbendRangeIn);

	void processMidi(MidiBuffer& bufferIn);

	void resetNotes();

	String* getMidiInLog();
	String* getMidiOutLog();
	String* getRetunerLog();

public:

	class Listener
	{
	public:
		virtual void ccValueChanged(int controlNumber, int controlValue) = 0;
	};

	void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
	void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

	ListenerList<Listener> listeners;


public:

	// MidiInputCallback implementation
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg) override;

	// Dynamic Tuning implementation
	void tuningChanged() override;
};