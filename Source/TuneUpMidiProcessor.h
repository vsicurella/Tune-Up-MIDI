/*
  ==============================================================================

    TuneUpMidiProcessor.h
    Created: 10 Jun 2020 11:14:48am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TunedNoteInterpreter.h"
#include "ChannelAssigner.h"

class TuneUpMidiProcessor : public MidiMessageCollector, public ChangeBroadcaster, private DynamicTuning::Listener
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
	//TuneUpMidiChannelAssigner channelAssigner;
	MPEChannelAssigner channelAssigner;
	Array<int> noteChannels;

	const Tuning* currentTuning = nullptr;

public:

	TuneUpMidiProcessor();
	~TuneUpMidiProcessor();

	void setTuning(const Tuning& tuningIn);
	void processMidi(MidiBuffer& bufferIn);

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