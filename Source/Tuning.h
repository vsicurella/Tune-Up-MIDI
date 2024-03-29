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

	String name;
	String description;

public:

	/*
		Expects a full interval table in cents, ending with period. May or may not include unison.
	*/
	Tuning(const Array<double>& intervalCentsIn, int rootMidiNoteIn = 60, String nameIn = "", String descriptionIn = "");

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

	String getName() const;
	String getDescription() const;
	String toString() const;

	/*
		Returns the period-reduced scale degree of the note number passed in
	*/
	int getScaleDegree(int midiNoteIn) const;

	/*
		Returns the closest note number to the semitone value passed in, 
		from reference of the root note
	*/
	int closestNoteToSemitone(double semitoneIn) const;
};
