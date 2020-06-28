/*
  ==============================================================================

    SvkMPEChannelAssigner.h
    Created: 30 Nov 2019 8:45:21pm
    Author:  Vincenzo Sicurella
 
    This class is used to separate MIDI Data so that there is only a single
    MIDI note per channel, within the provided MPEInstrument.

    The advantage of this over the JUCE MPEChannelAssigner is that this explicitly only
    allows one note per channel plus notes are differentiated not just by MIDI note number
    but also pitchbend data. This allows the triggering of the same MIDI note with different
    pitchbend data across different channels.

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CommonFunctions.h"

class TuneUpMidiChannelAssigner
{
	MPEInstrument* mpeInst = nullptr;
	
	Array<int> pitchbendOfChannel; // 16 values representing pitchbend per midi channel, 8192 meaning pitchbend at unity
    
    int voiceLimit = INT_MAX;
    int numNotesOn = 0;
	int lastChannelOn = -1;

	Array<MidiPitch> pitchesOn;
	Array<int>& notesOn; // Tuning degrees, not necessarily MIDI notes
	Array<int> channelsOn;

	Array<int> channelsOfNotes; // index is tuning note, value is channel number
	Array<int> channelsToSkip;
    
	// allows channels to contain more than one note if pitchbend value is the same
	// but per-note vibrato and other expression is limited
	bool oneChannelPerNote = true;
	bool roundRobinMode = true;
    
    
public:
    
    TuneUpMidiChannelAssigner(MPEInstrument* mpeInstIn, Array<int>& notesOnIn);
    ~TuneUpMidiChannelAssigner();

	bool isOneChannelPerNote() const;
	bool isRoundRobinMode() const;

	void setOneChannelPerNote(bool oneNoteEach);
	void setRoundRobinMode(bool roundRobin);

	/*
		Returns midi channel 0 - 15
	*/
	int findNextFreeChannel() const;
    
    /*
        Returns an available channel number for a given MidiPitch.
        Will return a used channel if an identical MidiPitch exists.
     */

	int getFreeOrExistingChannel(int noteIn, int pitchbendIn) const;

	/*
		Helper function that uses a note on a channel, and returns the channel it was used on.

		Note in is tuning degree, not necessarily midi note
	*/
	int noteOn(int noteIn, int pitchbendIn);
    
    /*
        Will return the channel number the MidiPitch exists on.

		Note in is tuning degree, not necessarily midi note
     */
    int noteOff(int noteIn);
    
    void allNotesOff();

	/*
		Input channel num 0-15
	*/

	bool isChannelFree(int channelIn) const;

	bool hasFreeChannels() const;

	Array<int> getChannelsOn() const;
    
    int getChannelOfNote(int noteIn) const;

	/*
		Input channel number 0 - 15 
	*/
	int getPitchbendOfChannel(int channelIn) const;
    
    int getVoiceLimit() const;
    
    int getNumNotesOn() const;
    
    void setVoiceLimit(int voiceLimitIn);
};
