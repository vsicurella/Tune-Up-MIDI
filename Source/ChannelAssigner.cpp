/*
  ==============================================================================

    SvkMPEChannelAssigner.cpp
    Created: 30 Nov 2019 8:45:21pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "ChannelAssigner.h"

TuneUpMidiChannelAssigner::TuneUpMidiChannelAssigner(MPEInstrument* mpeInstIn, Array<int>& notesOnIn)
	: notesOn(notesOnIn)
{
	pitchbendOfChannel.resize(16);
	pitchbendOfChannel.fill(8192);

	channelsOfNotes.resize(128);
	channelsOfNotes.fill(-1);
}

TuneUpMidiChannelAssigner::~TuneUpMidiChannelAssigner()
{
    
}


bool TuneUpMidiChannelAssigner::isOneChannelPerNote() const
{
	return oneChannelPerNote;
}

bool TuneUpMidiChannelAssigner::isRoundRobinMode() const
{
	return roundRobinMode;
}

void TuneUpMidiChannelAssigner::setOneChannelPerNote(bool oneNoteEach)
{
	oneChannelPerNote = oneChannelPerNote;
}

void TuneUpMidiChannelAssigner::setRoundRobinMode(bool roundRobin)
{
	roundRobinMode = roundRobin;
}

void TuneUpMidiChannelAssigner::setChannelsToSkip(Array<int> channelsToSkipIn)
{
	channelsToSkip = channelsToSkipIn;

	// TODO ?
}

int TuneUpMidiChannelAssigner::findNextFreeChannel() const
{
	if (numNotesOn <= voiceLimit)
	{
		if (roundRobinMode)
		{
			int freeChannel = lastChannelOn;
			do
			{
				freeChannel = (freeChannel + 1) % 16;

				if (!channelsOn.contains(freeChannel) && !channelsToSkip.contains(freeChannel))
				{
					return freeChannel;
				}
				
			} while (freeChannel != lastChannelOn - 1);
		}

		else
		{
			for (int i = 0; i < 16; i++)
			{
				if (!channelsOn.contains(i) && !channelsToSkip.contains(i))
				{
					return  i;
				}
			}
		}
	}

	return -1;
}

int TuneUpMidiChannelAssigner::getFreeOrExistingChannel(int noteIn, int pitchbendIn) const
{
    // TODO: implement MPEZone compatibility

	if (!oneChannelPerNote)
	{
		for (auto ch : channelsOn)
		{
			int pb = pitchbendOfChannel[ch];
			if (pb == pitchbendIn)
				return ch;
		}
	}
	
	return findNextFreeChannel();
}

int TuneUpMidiChannelAssigner::noteOn(int noteIn, int pitchbendIn)
{
	int channel;
	
	// If this returns a valid index, a Note Off message was missed somewhere
	int indexOfNote = notesOn.indexOf(noteIn);

	if (indexOfNote < 0)
		channel = getFreeOrExistingChannel(noteIn, pitchbendIn);
	else
		channel = channelsOfNotes[noteIn];
	
	if (channel >= 0 && channel < 16)
	{
		if (indexOfNote < 0)
		{
			notesOn.add(noteIn);
			channelsOn.addUsingDefaultSort(channel);
		}

		lastChannelOn = channel;

		numNotesOn = notesOn.size();

		
		pitchbendOfChannel.set(channel, pitchbendIn);
		channelsOfNotes.set(noteIn, channel);	
	}

	return channel;
}

int TuneUpMidiChannelAssigner::noteOff(int noteIn)
{
	int note, channel = -1;
	// Improve
	for (int i = 0; i < notesOn.size(); i++)
	{
		note = notesOn[i];
		channel = channelsOfNotes[note];

		if (note == noteIn)
		{
			notesOn.remove(i);
			channelsOfNotes.set(note, -1);
			numNotesOn = notesOn.size();

			bool removeChannel = true;
			if (!oneChannelPerNote)
			{
				// retain channel if has other notes on it
				for (auto ch : channelsOfNotes)
				{
					if (channel == channelsOn[ch])
					{
						removeChannel = false;
						break;
					}
				}
			}

			if (removeChannel)
			{
				channelsOn.removeFirstMatchingValue(channel);
				pitchbendOfChannel.set(channel, 8192);
			}

			break;
		}
	}

	return channel;
}

void TuneUpMidiChannelAssigner::allNotesOff()
{
	pitchbendOfChannel.fill(8192);
	notesOn.clear();
	channelsOn.clear();
	numNotesOn = 0;
}

bool TuneUpMidiChannelAssigner::isChannelFree(int channelIn) const
{
	return channelsOn.indexOf(channelIn) < 0;
}

bool TuneUpMidiChannelAssigner::isUnderVoiceLimit() const
{
	return notesOn.size() < voiceLimit;
}

const Array<int>& TuneUpMidiChannelAssigner::getChannelsOn() const
{
	return channelsOn;
}

int TuneUpMidiChannelAssigner::getChannelOfNote(int noteIn) const
{
	return channelsOfNotes[noteIn];
}

const Array<int>& TuneUpMidiChannelAssigner::getChannelsPitchbend() const
{
	return pitchbendOfChannel;
}

int TuneUpMidiChannelAssigner::getPitchbendOfChannel(int channelIn) const
{
	return pitchbendOfChannel[channelIn];
}

int TuneUpMidiChannelAssigner::getVoiceLimit() const
{
    return voiceLimit;
}

int TuneUpMidiChannelAssigner::getNumNotesOn() const
{
	return numNotesOn;
}

void TuneUpMidiChannelAssigner::setVoiceLimit(int voiceLimitIn)
{
    voiceLimit = voiceLimitIn;
}