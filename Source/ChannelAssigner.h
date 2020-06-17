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
    
    int voiceLimit = -1;
    int numNotesOn = 0;
	
	Array<ChannelMidiPitch> notesOn;
	Array<int> channelsOn;
    
	// allows channels to contain more than one note if pitchbend value is the same
	// but per-note vibrato and other expression is limited
	bool oneChannelPerNote = false; 
    
    
public:
    
    TuneUpMidiChannelAssigner(MPEInstrument* mpeInstIn);
    ~TuneUpMidiChannelAssigner();
    
    /*
        Returns an available channel number for a given MidiPitch.
        Will return a used channel if an identical MidiPitch exists.
     */

	int getFreeOrExistingChannel(int noteIn, int pitchbendIn) const;

	/*
		Helper function that uses a note on a channel, and returns the channel it was used on
	*/
	int noteOn(int noteIn, int pitchbendIn);
    
    /*
        Will return the channel number the MidiPitch exists on.
     */
    int noteOff(int noteIn, int pitchbendIn);

	Array<ChannelMidiPitch> getAllNotesOn() const;
    
    void allNotesOff();
    
    int getChannelOfNote(int noteIn, int pitchbendIn) const;

	/*
		Input channel number 0 - 15 
	*/
	int getPitchbendOfChannel(int channelIn) const;
    
    int getVoiceLimit() const;
    
    int getNumNotesOn() const;
    
    void setVoiceLimit(int voiceLimitIn);
};
