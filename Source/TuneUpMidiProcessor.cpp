/*
  ==============================================================================

    TuneUpMidiProcessor.cpp
    Created: 10 Jun 2020 11:14:48am
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiProcessor.h"


TuneUpMidiProcessor::TuneUpMidiProcessor()
//	: channelAssigner(nullptr) // TODO implement proper MPE instrument
{
	retuner.reset(new MidiNoteTuner(currentTuning));
	noteChannels.resize(128);
	noteChannels.fill(-1);

	midiInput = MidiInput::openDevice(inputDeviceInfo.name, this);
	//midiOutput = MidiInput::openDevice(outputDeviceInfo.name, this);

	DBG("Midi Input opened: " + inputDeviceInfo.name);
	midiInput->start();
}

TuneUpMidiProcessor::~TuneUpMidiProcessor()
{
	retuner = nullptr;
	currentTuning = nullptr;
}

void TuneUpMidiProcessor::setTuning(const Tuning& tuningIn)
{
	currentTuning = &tuningIn;
	retuner->setNewTuning(currentTuning);
}

void TuneUpMidiProcessor::processMidi(MidiBuffer& bufferIn)
{
	// add inputBuffer
	bufferIn.addEvents(inputBuffer, 0, -1, bufferIn.getLastEventTime());
	inputBuffer.clear();
	smplInput = 0;

	// get note data
	auto events = MidiBuffer::Iterator(bufferIn);
	
	MidiBuffer bufferOut, shortBuffer;
	MidiMessage msg, pitchMsg;
	int smpl, smplOffset = 0;
	while (events.getNextEvent(msg, smpl))
	{
		desc = msg.getDescription() + '\n';
		midiInLog.append(desc, desc.length());
		desc = "";
		int bendChannel = -1;

		// remap and add pitchbend
		if (retuner.get())
		{	
			MPENote note = retuner->closestNote(msg.getNoteNumber());

			desc += "Retuner: New note semitone difference: " + String(retuner->semitonesFromNote(msg.getNoteNumber())) + '\n';
			desc += "Retuner: Note shifted from " + String(msg.getNoteNumber()) + " to " + String(note.initialNote) + '\n';

			msg.setNoteNumber(note.initialNote);

			// set channel
			if (msg.isNoteOn())
			{
				//bendChannel = channelAssigner.noteOn(msg.getNoteNumber(), note.pitchbend.as14BitInt());
				bendChannel = channelAssigner.findMidiChannelForNewNote(bendChannel);

				jassert(bendChannel >= 0 && bendChannel < 16);

				if (bendChannel >= 0 && bendChannel < 16)
				{
					msg.setChannel(bendChannel);
					msg.setNoteNumber(note.initialNote);
					noteChannels.set(msg.getNoteNumber(), bendChannel);
					
					//pitchMsg = MidiMessage::pitchWheel(bendChannel, note.pitchbend.as14BitInt());
					//desc += "Retuner: Pitchbend value = " + String(note.pitchbend.as14BitInt()) + '\n';

					//shortBuffer.addEvent(msg, smpl);

					pitchMsg = MidiMessage::pitchWheel(bendChannel, note.pitchbend.as14BitInt());
					desc += "Retuner: Pitchbend value = " + String(pitchMsg.getPitchWheelValue()) + '\n';
					bufferOut.addEvent(pitchMsg, smpl + smplOffset++);
					bufferOut.addEvent(msg, smpl + smplOffset);
					smplOffset++;
				}
			}
			else if (msg.isNoteOff())
			{
				//bendChannel = channelAssigner.getChannelOfNote(msg.getNoteNumber(), note.pitchbend.as14BitInt());
				bendChannel = noteChannels[msg.getNoteNumber()];
				jassert(bendChannel >= 0 && bendChannel < 16);

				if (bendChannel >= 0 && bendChannel < 16)
				{
					msg.setChannel(bendChannel);
					noteChannels.set(msg.getNoteNumber(), -1);
					
					//pitchMsg = MidiMessage::pitchWheel(bendChannel, 8192);
					//desc += pitchMsg.getDescription() + '\n';

					//channelAssigner.noteOff(msg.getNoteNumber(), note.pitchbend.as14BitInt());

					//smplOffset++;
					//shortBuffer.addEvent(pitchMsg, smpl + smplOffset++);
					bufferOut.addEvent(msg, smpl + smplOffset);
				}
			}
			else
			{
				bendChannel = -1;
			}
		}

		//if (bendChannel > 0)
		//{
		//	pitchMsg = MidiMessage::pitchWheel(bendChannel, channelAssigner.getPitchbendOfChannel(bendChannel - 1));
		//	desc += "Retuner: Pitchbend value = " + String(pitchMsg.getPitchWheelValue()) + '\n';
		//	bufferOut.addEvent(pitchMsg, 0);
		//}

		//bufferOut.addEvents(shortBuffer, 0, -1, smplOffset);
		//shortBuffer.clear();

		desc += msg.getDescription() + '\n';
		midiOutLog.append(desc, desc.length());
	}

	bufferIn = bufferOut;
}

void TuneUpMidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
	if (msg.isNoteOnOrOff())
		inputBuffer.addEvent(msg, smplInput++);
	else
	{
		inputBuffer.addEvent(msg, ccSmpl++);
		listeners.call(&Listener::ccValueChanged, msg.getControllerNumber(), msg.getControllerValue());
	}
}

void TuneUpMidiProcessor::tuningChanged() 
{
	// store current notes
	// calculate new tuning
	// find pitchbend for each note
	// send pitchbend out on appropriate channels
}

String* TuneUpMidiProcessor::getMidiInLog()
{
	return &midiInLog;
}

String* TuneUpMidiProcessor::getMidiOutLog()
{
	return &midiOutLog;
}

String* TuneUpMidiProcessor::getRetunerLog()
{
	return &retunerLog;
}