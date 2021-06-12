/*
  ==============================================================================

    Tuning.cpp
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "Tuning.h"

Tuning::Tuning(const Array<double>& centsTable, int rootMidiNoteIn, String nameIn, String descriptionIn)
{
	tuningSize = centsTable.size();
	rootMidiNote = rootMidiNoteIn;
	
	for (int i = 0; i < centsTable.size(); i++)
	{
		double note = centsTable[i];
		intervalCents.add(note);
	}

	// Record and remove period
	periodCents = intervalCents[tuningSize - 1];
	periodSemitones = periodCents / 100.0;
	intervalCents.removeFirstMatchingValue(periodCents);

	// Just in case
	if (intervalCents[0] != 0.0 || intervalCents.size() == 0)
	{
		// Add unision
		intervalCents.insert(0, 0);
	}

	tuningSize = intervalCents.size();
	jassert(tuningSize > 0);

	// Cache Semitones
	for (int i = 0; i < 128; i++)
	{
		intervalSemitones.add(getNoteInCents(i) / 100.0);
	}
	
	name = nameIn;
	description = descriptionIn;
}

void Tuning::setRootNote(int rootNoteIn)
{
	rootMidiNote = rootNoteIn;
}

void Tuning::setDescription(String descIn)
{
	description = descIn;
}

double Tuning::getPeriodCents() const
{
	return periodCents;
}

double Tuning::getPeriodSemitones() const
{
	return periodSemitones;
}

Array<double> Tuning::getIntervalCents() const
{
	return intervalCents;
}

Array<double> Tuning::getIntervalSemitones() const
{
    return intervalSemitones;
}

int Tuning::getTuningSize() const
{
	return tuningSize;
}

int Tuning::getRootNote() const
{
    return rootMidiNote;
}

double Tuning::getNoteInSemitones(int midiNoteIn) const
{
	// Return value that will always fail
	if (midiNoteIn < 0 || midiNoteIn > 127)
		return INT_MAX;

	return intervalSemitones[midiNoteIn];
}

double Tuning::getNoteInCents(int midiNoteIn) const
{
	int noteDif = midiNoteIn - rootMidiNote;
	int numPeriods = floor((double)noteDif / tuningSize);

	double periods = periodCents * numPeriods;

	return intervalCents[modulo(noteDif, tuningSize)] + periods;
}

String Tuning::getName() const
{
	return name;
}

String Tuning::getDescription() const
{
	return description;
}

String Tuning::toString() const
{
	String tableStr = "";
	for (auto cents : intervalCents)
		tableStr << String(cents) << newLine;

	return tableStr;
}

int Tuning::getScaleDegree(int midiNoteIn) const
{
	return modulo(midiNoteIn - rootMidiNote, tuningSize);
}

int Tuning::closestNoteToSemitone(double semitoneIn) const
{
	if (tuningSize == 1)
	{
		return round(semitoneIn / periodSemitones) + rootMidiNote;
	}
	
	double dif, closestDif = INT_MAX;
	int closestIndex = -1;
	
	if (semitoneIn >= 0)
	{
		for (int i = 60; i < 128; i++)
		{
			dif = abs(semitoneIn - intervalSemitones[i]);
			if (dif < closestDif)
			{
				closestDif = dif;
				closestIndex = i;
			}
		}
	}
	else
	{
		for (int i = 60; i >= 0; i--)
		{
			dif = abs(semitoneIn - intervalSemitones[i]);
			if (dif < closestDif)
			{
				closestDif = dif;
				closestIndex = i;
			}
		}
	}

	return closestIndex;
}