/*
  ==============================================================================

    Tuning.h
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CommonFunctions.h"

class Tuning
{
    Array<double> intervalSemitones;
    Array<double> intervalCents;
    
    int midiNoteOffset = 0;
	double periodCents = 1200;
	double periodSemitones = 12;

	int tuningSize = 1;
    int rootMidiNote = 60;     // note tuning is centered on

    
public:
    
    // unison tuning
    Tuning();    
    Tuning(const Array<double>& centsTable, int rootIndex=0);
    ~Tuning();

	void setRootNote(int rootNoteIn);
    
    int getTuningSize() const;
    
    Array<double> getSemitoneTable() const;
    
    double getNoteInSemitones(int midiNoteIn) const;
    double getNoteInCents(int midiNoteIn) const;
    
    int getRootNote() const;

	int closestNoteToSemitone(double semitoneIn) const;

    static double ratioToSemitones(double ratioIn);
    static double ratioToCents(double ratioIn);
    static double centsToRatio(double centsIn);
};
