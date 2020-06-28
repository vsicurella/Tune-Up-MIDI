/*
  ==============================================================================

    TuneUpMidiProcessor.cpp
    Created: 10 Jun 2020 11:14:48am
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiProcessor.h"


TuneUpMidiProcessor::TuneUpMidiProcessor()
	: channelAssigner(nullptr, notesInOn) // TODO implement proper MPE instrument
{
	retuner.reset(new MidiNoteTuner(standard, standard));

	midiInput = MidiInput::openDevice(inputDeviceInfo.name, this);
	if (midiInput.get())
		midiInput->start();
	DBG("Midi Input opened: " + inputDeviceInfo.name);


	//midiOutput = MidiInput::openDevice(outputDeviceInfo.name, this);

}

TuneUpMidiProcessor::~TuneUpMidiProcessor()
{
	retuner = nullptr;
}

const Array<int>& TuneUpMidiProcessor::getTuningNotesOn() const
{
	return notesInOn;
}

void TuneUpMidiProcessor::setTuning(const Tuning* tuningIn)
{
	tuning = tuningIn;
	retuner->setNewTuning(*tuning);
	tuningChanged();
}

void TuneUpMidiProcessor::setPitchbendRange(int pitchbendRangeIn)
{
	pitchbendRange = pitchbendRangeIn;
	if (retuner.get())
		retuner->setPitchbendRange(pitchbendRange);
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
	int noteIn, smpl, smplOffset = 0;
	while (events.getNextEvent(msg, smpl))
	{
		desc = msg.getDescription() + '\n';
		midiInLog.append(desc, desc.length());
		desc = "";

		// remap and add pitchbend
		if (msg.isNoteOnOrOff())
		{
			noteIn = msg.getNoteNumber();

			int noteChannel = 0;
			MPENote noteTuned = retuner->closestNote(noteIn);

			desc += "Retuner: New note semitone difference: " + String(retuner->semitonesFromNote(noteIn)) + '\n';
			desc += "Retuner: Note shifted from " + String(noteIn) + " to " + String(noteTuned.initialNote) + '\n';

			msg.setNoteNumber(noteTuned.initialNote);

			// set channel
			if (msg.isNoteOn())
			{
				if (channelAssigner.hasFreeChannels())
				{
					noteChannel = channelAssigner.noteOn(noteIn, noteTuned.pitchbend.as14BitInt()) + 1;
					jassert(noteChannel > 0 && noteChannel <= 16);

					msg.setChannel(noteChannel);
					notesTunedOn.add(noteTuned.initialNote);

					pitchMsg = MidiMessage::pitchWheel(noteChannel, noteTuned.pitchbend.as14BitInt());
					desc += "Retuner: Pitchbend value = " + String(pitchMsg.getPitchWheelValue()) + '\n';

					bufferOut.addEvent(pitchMsg, smplOffset++);
					bufferOut.addEvent(msg, smplOffset++);
				}
			}
			else
			{
				noteChannel = channelAssigner.getChannelOfNote(noteIn) + 1;
				//jassert(bendChannel >= 0 && bendChannel < 16);

				if (noteChannel > 0 && noteChannel <= 16)
				{
					msg.setChannel(noteChannel);
					channelAssigner.noteOff(noteIn);
					notesTunedOn.removeFirstMatchingValue(noteTuned.initialNote);
					
					bufferOut.addEvent(msg, smplOffset++);
					desc += pitchMsg.getDescription() + '\n';
				}
			}
		}
		
		// Any other MIDI message
		else
		{
			sendControlMessage(msg);
			bufferOut.addEvent(msg, smplOffset++);
		}

		desc += msg.getDescription() + '\n';
		midiOutLog.append(desc, desc.length());
	}

	bufferIn = bufferOut;
}

void TuneUpMidiProcessor::resetNotes()
{
	notesInOn.clear();
	notesTunedOn.clear();
	channelAssigner.allNotesOff();
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
	// only works if strictly one channel per note is used
	if (channelAssigner.isOneChannelPerNote())
	{
		for (int i = 0; i < notesInOn.size(); i++)
		{
			int nodeIn = notesInOn[i];
			int noteOut = notesTunedOn[i];

			inputBuffer.addEvent(MidiMessage::pitchWheel(
				channelAssigner.getChannelOfNote(nodeIn) + 1,
				retuner->pitchbendFromNote(noteOut, nodeIn)
			), smplInput++);
		}
	}
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