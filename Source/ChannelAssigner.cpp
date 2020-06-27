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
    mpeInst = mpeInstIn;

	pitchbendOfChannel.resize(16);
	pitchbendOfChannel.fill(8192);

	channelsOfNotes.resize(128);
	channelsOfNotes.fill(-1);
}

TuneUpMidiChannelAssigner::~TuneUpMidiChannelAssigner()
{
    
}

int TuneUpMidiChannelAssigner::findNextFreeChannel() const
{
	int freeChannel = -1;

	if (numNotesOn <= voiceLimit)
	{
		if (roundRobinMode)
		{
			do
			{
				freeChannel = (channelsOn[channelsOn.size() - 1] + 1) % 16;
			} while (channelsOn.contains(freeChannel) && !channelsToSkip.contains(freeChannel));
		}

		else
		{
			for (int i = 0; i < 16; i++)
			{
				if (!channelsOn.contains(i) && !channelsToSkip.contains(i))
				{
					freeChannel = i;
					break;
				}
			}
		}
	}

	return freeChannel;
}

int TuneUpMidiChannelAssigner::getFreeOrExistingChannel(int noteIn, int pitchbendIn) const
{
    // TODO: implement voice limit and add MPEZone compatibility
    //int channelMax = mpeInst->getZoneLayout().getLowerZone().getLastMemberChannel();

	int note, pitchbend;
	bool isSameNote = false;

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
	int channel = -1;
	if (mpeInst && mpeInst->isMemberChannel(channel) || !mpeInst)
	{
		channel = getFreeOrExistingChannel(noteIn, pitchbendIn);
		
		if (!notesOn.contains(noteIn))
		{
			notesOn.add(noteIn);
			numNotesOn = notesOn.size();

			channelsOn.addUsingDefaultSort(channel);
			pitchbendOfChannel.set(channel, pitchbendIn);

			channelsOfNotes.set(noteIn, channel);
		}
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
}

bool TuneUpMidiChannelAssigner::isChannelFree(int channelIn) const
{
	return channelsOn.indexOf(channelIn) < 0;
}

int TuneUpMidiChannelAssigner::getChannelOfNote(int noteIn) const
{
	return channelsOfNotes[noteIn];
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