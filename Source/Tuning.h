/*
  ==============================================================================

    Tuning.h
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CommonFunctions.h"

class Tuning
{

public:

	static Identifier tuningDefId;
	static Identifier tuningSizeID;
	static Identifier rootMidiNoteID;
	static Identifier generatorListID;
	static Identifier generatorOffsetsID;
	static Identifier generatorAmountsID;
	static Identifier generatorID;
	static Identifier centsTableID;
	static Identifier intervalNodeID;
	static Identifier intervalValueID;
	static Identifier tuningDescID;

protected:

    Array<double> intervalSemitones;
    Array<double> intervalCents;
    
	double periodCents = 1200;
	double periodSemitones = 12;

	int tuningSize = 1;
    int rootMidiNote = 60;     // note tuning is centered on

	String description;
	ValueTree definition;

public:

	class Listener
	{
	public:

		virtual ~Listener() {};
		virtual void tuningChanged() {};
	};

	void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
	void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }


protected:

	ListenerList<Listener> listeners;

public:
    
    // unison tuning
    Tuning();

	/*
		Expects a Scala-like interval table (no unison, ending with period)
	*/
    Tuning(const Array<double>& centsTable, int rootIndex = 0, String description = "");

	/*
		Expects a interval table starting with unison, and using first generator child as period
	*/
	Tuning(ValueTree tuningPropertiesIn);
    ~Tuning();

	const bool isDynamic = false;

	void setRootNote(int rootNoteIn);

	void setDescription(String descIn);
    
    int getTuningSize() const;
    
	Array<double> getIntervalCents() const;
    Array<double> getIntervalSemitones() const;
    
    virtual double getNoteInSemitones(int midiNoteIn) const;
    virtual double getNoteInCents(int midiNoteIn) const;
    
    int getRootNote() const;

	String getDescription() const;

	ValueTree getTuningDefinition();

	virtual int closestNoteToSemitone(double semitoneIn) const;

    static double ratioToSemitones(double ratioIn);
    static double ratioToCents(double ratioIn);
    static double centsToRatio(double centsIn);

	static ValueTree createTuningDefinition(int tuningSize, double periodCents, Array<double> centsTable, String descriptionIn = "");
};
