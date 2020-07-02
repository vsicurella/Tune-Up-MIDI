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
		intervalSemitones.add(note / 100);
	}

	// Record and remove period
	periodCents = intervalCents[tuningSize - 1];
	periodSemitones = intervalSemitones[tuningSize - 1];

	intervalCents.remove(tuningSize - 1);
	intervalSemitones.remove(tuningSize - 1);

	// Just in case
	if (intervalCents[0] != 0.0)
	{
		// Add unision
		intervalCents.insert(0, 0);
		intervalSemitones.insert(0, 0);
	}

	tuningSize = intervalCents.size();

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
	if (intervalSemitones.size() > 0)
	{
		int noteDif = midiNoteIn - rootMidiNote;
		int numPeriods = floor((double)noteDif / tuningSize);

		double periods = periodSemitones * numPeriods;

		return intervalSemitones[modulo(noteDif, tuningSize)] + periods;
	}

	return midiNoteIn - rootMidiNote;
}

double Tuning::getNoteInCents(int midiNoteIn) const
{
	if (intervalSemitones.size() > 0)
	{
		int noteDif = midiNoteIn - rootMidiNote;
		int numPeriods = floor((double)noteDif / tuningSize);

		double periods = periodCents * numPeriods;

		return intervalCents[modulo(noteDif, tuningSize)] + periods;
	}
	
	return midiNoteIn - (double) rootMidiNote * 100;
}

String Tuning::getName() const
{
	return name;
}

String Tuning::getDescription() const
{
	return description;
}

int Tuning::closestNoteToSemitone(double semitoneIn) const
{
	if (intervalSemitones.size() > 0)
	{
		double numPeriods = (int)(semitoneIn * 100 / periodCents);
		double reduced = semitoneIn - (periodCents * numPeriods / 100);
		
		double dif, closestDif = INT_MAX;
		int closestIndex = -1;
		for (int i = 0; i < tuningSize; i++)
		{
			dif = reduced - intervalSemitones[i];
			if (dif < closestDif)
			{
				closestDif = dif;
				closestIndex = i;
			}
		}

		return closestIndex + tuningSize * numPeriods + rootMidiNote;
	}

	return round(semitoneIn) + rootMidiNote;
}