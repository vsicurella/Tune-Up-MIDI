/*
  ==============================================================================

    TuneUpMidiProcessor.cpp
    Created: 10 Jun 2020 11:14:48am
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuneUpMidiProcessor.h"


TuneUpMidiProcessor::TuneUpMidiProcessor(const Tuning* originTuning, const Tuning* newTuning, Array<int>& notesOnIn)
	:	tuningIn(originTuning),
		tuningOut(newTuning),
		notesInOn(notesOnIn),
		channelAssigner(nullptr, notesOnIn) // TODO implement proper MPE zones
{
	retuner.reset(new MidiNoteTuner(*tuningIn, *tuningOut));
	retuner->setPitchbendRange(pitchbendRange);

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

int TuneUpMidiProcessor::getNumberOfNotesOn() const
{
	return channelAssigner.getNumNotesOn();
}

const Array<int>& TuneUpMidiProcessor::getChannelsOn() const
{
	return channelAssigner.getChannelsOn();
}

const Array<int>& TuneUpMidiProcessor::getChannelsPitchbend() const
{
	return channelAssigner.getChannelsPitchbend();
}

void TuneUpMidiProcessor::setTuningIn(const Tuning* tuningInIn)
{
	tuningIn = tuningInIn;
	retuner->setOriginTuning(*tuningIn);

	if (notesInOn.size() > 0)
		resetNotes();
}

void TuneUpMidiProcessor::setTuningOut(const Tuning* tuningOutIn)
{
	tuningOut = tuningOutIn;
	retuner->setNewTuning(*tuningOut);
	
	if (notesInOn.size() > 0)
		resetNotes();
}

void TuneUpMidiProcessor::setReferenceNoteIn(int noteIn)
{
	retuner->setOriginRootNote(noteIn);
}

void TuneUpMidiProcessor::setReferenceFreqIn(double freqIn)
{
	retuner->setOriginRootFreq(freqIn);
}

void TuneUpMidiProcessor::setReferenceNoteOut(int noteOut)
{
	retuner->setDestinationRootNote(noteOut);
}

void TuneUpMidiProcessor::setReferenceFreqOut(double freqOut)
{
	retuner->setDestinationRootFrequency(freqOut);
}

void TuneUpMidiProcessor::setPitchbendRange(int pitchbendRangeIn)
{
	pitchbendRange = pitchbendRangeIn;
	if (retuner.get())
		retuner->setPitchbendRange(pitchbendRange);
}

void TuneUpMidiProcessor::setVoiceLimit(int voiceLimitIn)
{
	channelAssigner.setVoiceLimit(voiceLimitIn);
}

void TuneUpMidiProcessor::setFreeChannelMode(FreeChannelMode channelModeIn)
{
	channelAssigner.setRoundRobinMode(channelModeIn == FreeChannelMode::RoundRobin);
}

void TuneUpMidiProcessor::setChannelConfiguration(ValueTree channelPropertiesNodeIn)
{
	channelsToSkip.clear();

	for (auto ch : channelPropertiesNodeIn)
	{
		if (ch.hasType(TuneUpIDs::channelNodeId) && (int)ch[TuneUpIDs::channelUsedId] == 0)
		{
			channelsToSkip.add((int)ch[TuneUpIDs::channelNumberId]);
		}
	}

	channelAssigner.setChannelsToSkip(channelsToSkip);

	// TODO
}

void TuneUpMidiProcessor::setReuseChannels(bool reuseChannels)
{
	channelAssigner.setOneChannelPerNote(!reuseChannels);
}

void TuneUpMidiProcessor::setResetChannelPitchbendWhenEmpty(bool resetPitchbend)
{
	resetChannelPitchbendWhenEmpty = resetPitchbend;
}

void TuneUpMidiProcessor::processMidi(MidiBuffer& bufferIn)
{
	// add inputBuffer
	if (inputBuffer.getLastEventTime() > 0)
	{
		bufferIn.addEvents(inputBuffer, 0, -1, 0);
		inputBuffer.clear();
		smplInput = 0;
	}

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

		// TODO: Put each case into separate function and set up so that interal note on and note offs can be processed properly
		// no matter where/when the function is called

		// remap and add pitchbend
		if (msg.isNoteOnOrOff())
		{
			noteIn = msg.getNoteNumber();

			int noteChannel = 0;
			int noteTuned;
			int pbValue = 8192;
			retuner->closestNote(noteIn, noteTuned, pbValue);

			desc += "Retuner: New note semitone difference: " + String(retuner->semitonesFromNote(noteIn)) + '\n';
			desc += "Retuner: Note shifted from " + String(noteIn) + " to " + String(noteTuned) + '\n';

			if (noteTuned == -1)
			{
				// TODO: some alert that pitchbend range is not high enough
				continue;
			}

			msg.setNoteNumber(noteTuned);

			// set channel
			if (msg.isNoteOn())
			{
				if (channelAssigner.isUnderVoiceLimit())
				{
					noteChannel = channelAssigner.noteOn(noteIn, pbValue) + 1;

					// No free channel is available
					if (noteChannel < 1 || noteChannel > 16)
						continue;

					jassert(noteChannel > 0 && noteChannel <= 16);

					msg.setChannel(noteChannel);
					notesTunedOn.add(noteTuned);

					pitchMsg = MidiMessage::pitchWheel(noteChannel, pbValue);
					desc += "Retuner: Pitchbend value = " + String(pitchMsg.getPitchWheelValue()) + '\n';

					bufferOut.addEvent(pitchMsg, smplOffset++);
					bufferOut.addEvent(msg, smplOffset++);
				}
			}
			else
			{
				noteChannel = channelAssigner.getChannelOfNote(noteIn) + 1;

				if (noteChannel < 1 || noteChannel > 16)
					continue;

				jassert(noteChannel > 0 && noteChannel <= 16);

				msg.setChannel(noteChannel);
				channelAssigner.noteOff(noteIn);
				notesTunedOn.removeFirstMatchingValue(noteTuned);

				bufferOut.addEvent(msg, smplOffset++);

				if (resetChannelPitchbendWhenEmpty)
				{
					pitchMsg = MidiMessage::pitchWheel(noteChannel, MPEValue::centreValue().as14BitInt());
					bufferOut.addEvent(pitchMsg, smplOffset++);
					desc += pitchMsg.getDescription() + '\n';
				}
			}
		}
		
		// Any other MIDI message
		else
		{
			// TODO: add pitchbend changes to current channel pitchbend

			sendControlMessage(msg);
			bufferOut.addEvent(msg, smplOffset++);

			// TEMP - all allNotesOff messages will be "omni"
			if (msg.isAllNotesOff())
			{
				for (int i = 0; i < getNumberOfNotesOn(); i++)
				{
					bufferOut.addEvent(MidiMessage::noteOff(channelAssigner.getChannelOfNote(notesInOn[i]) + 1, notesTunedOn[i]), smplOffset++);
				}

				notesInOn.clear();
				notesTunedOn.clear();
				channelAssigner.allNotesOff();
			}
		}

		desc += msg.getDescription() + '\n';
		midiOutLog.append(desc, desc.length());
	}

	bufferIn = bufferOut;
	sendChangeMessage();
}

void TuneUpMidiProcessor::resetNotes()
{
	for (auto ch : getChannelsOn())
		inputBuffer.addEvent(MidiMessage::allNotesOff(ch), smplInput++);

	sendChangeMessage();
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

//void TuneUpMidiProcessor::tuningChanged() 
//{
//	// only works if strictly one channel per note is used
//	if (channelAssigner.isOneChannelPerNote())
//	{
//		for (int i = 0; i < notesInOn.size(); i++)
//		{
//			int nodeIn = notesInOn[i];
//			int noteOut = notesTunedOn[i];
//
//			inputBuffer.addEvent(MidiMessage::pitchWheel(
//				channelAssigner.getChannelOfNote(nodeIn) + 1,
//				retuner->pitchbendFromNote(noteOut, nodeIn)
//			), smplInput++);
//		}
//	}
//}

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