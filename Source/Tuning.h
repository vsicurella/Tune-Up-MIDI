/*
  ==============================================================================

    Tuning.h
    Created: 28 June 5:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include "CommonFunctions.h"

class Tuning
{

protected:

	Array<double> intervalCents;
    Array<double> intervalSemitones;
    
	double periodCents = 1200;
	double periodSemitones = 12;

	int tuningSize = 12;
    int rootMidiNote = 60; // note tuning is centered on

	String description;

public:
    
    // Will function like standard tuning
	Tuning() {};

	/*
		Expects a full interval table in cents, ending with period. May or may not include unison.
	*/
	Tuning(const Array<double>& intervalCentsIn, int rootMidiNoteIn = 60, String description = "");

	void setRootNote(int rootNoteIn);
	void setDescription(String descIn);

    int getTuningSize() const;

	double getPeriodCents() const;
	double getPeriodSemitones() const;
	
	Array<double> getIntervalCents() const;
    Array<double> getIntervalSemitones() const;
    
    virtual double getNoteInSemitones(int midiNoteIn) const;
    virtual double getNoteInCents(int midiNoteIn) const;
    
    int getRootNote() const;
	String getDescription() const;

	int closestNoteToSemitone(double semitoneIn) const;
};
