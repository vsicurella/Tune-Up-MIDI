/*
  ==============================================================================

    SvkMPEChannelAssigner.cpp
    Created: 30 Nov 2019 8:45:21pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "ChannelAssigner.h"

TuneUpMidiChannelAssigner::TuneUpMidiChannelAssigner(MPEInstrument* mpeInstIn)
{
    mpeInst = mpeInstIn;

	pitchbendOfChannel.resize(16);
	pitchbendOfChannel.fill(8192);
}

TuneUpMidiChannelAssigner::~TuneUpMidiChannelAssigner()
{
    
}

int TuneUpMidiChannelAssigner::getFreeOrExistingChannel(int noteIn, int pitchbendIn) const
{
    // TODO: implement voice limit and add MPEZone compatibility
    //int channelMax = mpeInst->getZoneLayout().getLowerZone().getLastMemberChannel();

	int note, pitchbend;
	bool isSameNote = false;

	if (!oneChannelPerNote)
	{
		int channelOut = -1;

		for (auto n : notesOn)
		{
			int ch = n.key;
			pitchbend = pitchbendOfChannel[ch];

			if (pitchbendIn == pitchbend)
			{
				channelOut = ch;
			}

			if (channelOut == ch && n.value.key == noteIn)
			{
				isSameNote = true;
				break;
			}
		}

		return channelOut + 1;
	}

	for (int i = 0; i < 16; i++)
	{
		if (!channelsOn.contains(i))
		{
			return i;
		}
	}
	
    return -1;
}

int TuneUpMidiChannelAssigner::noteOn(int noteIn, int pitchbendIn)
{
	int channel = 0;
	if (mpeInst && mpeInst->isMemberChannel(channel) || !mpeInst)
	{
		channel = getFreeOrExistingChannel(noteIn, pitchbendIn);

		ChannelMidiPitch pitch = ChannelMidiPitch(channel, MidiPitch(noteIn, pitchbendIn));
		notesOn.addIfNotAlreadyThere(pitch);
		channelsOn.addIfNotAlreadyThere(channel);
		pitchbendOfChannel.set(channel, pitchbendIn);
	}

	numNotesOn = notesOn.size();
	return channel + 1;
}

int TuneUpMidiChannelAssigner::noteOff(int noteIn, int pitchbendIn)
{
	int channel = -1;

	for (int i = 0; i < notesOn.size(); i++)
	{
		ChannelMidiPitch note = notesOn[i];

		if (note.value.key == noteIn && note.value.value == pitchbendIn)
		{
			channel = note.key;
			notesOn.remove(i);

			// turn off channel if no other notes on it
			bool channelHasOtherNotes = false;
			for (auto n : notesOn)
			{
				if (n.key == channel)
					channelHasOtherNotes = true;
			}

			if (!channelHasOtherNotes)
			{
				channelsOn.removeAllInstancesOf(channel);
				pitchbendOfChannel.set(channel, 8192);
			}

			break;
		}
	}

	numNotesOn = notesOn.size();
	return channel + 1;
}

void TuneUpMidiChannelAssigner::allNotesOff()
{
	pitchbendOfChannel.fill(8192);
	notesOn.clear();
	channelsOn.clear();
}

int TuneUpMidiChannelAssigner::getChannelOfNote(int noteIn, int pitchbendIn) const
{
	for (auto n : notesOn)
	{
		if (n.value.key == noteIn && n.value.value == pitchbendIn)
		{
			return n.key + 1;
		}
	}

	return -1;
}

int TuneUpMidiChannelAssigner::getPitchbendOfChannel(int channelIn) const
{
	return pitchbendOfChannel[channelIn];
}


Array<ChannelMidiPitch> TuneUpMidiChannelAssigner::getAllNotesOn() const
{
	return notesOn;
}

int TuneUpMidiChannelAssigner::getVoiceLimit() const
{
    return voiceLimit;
}

int TuneUpMidiChannelAssigner::getNumNotesOn() const
{
    return getAllNotesOn().size();
}

void TuneUpMidiChannelAssigner::setVoiceLimit(int voiceLimitIn)
{
    voiceLimit = voiceLimitIn;
}