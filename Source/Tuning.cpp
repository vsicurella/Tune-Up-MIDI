/*
  ==============================================================================

    Tuning.cpp
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "Tuning.h"

Tuning::Tuning()
{

}

Tuning::Tuning(const Array<double>& centsTable, int rootIndex)
{
	tuningSize = centsTable.size();
	
	for (int i = 0; i < tuningSize; i++)
	{
		double note = centsTable[(i + rootIndex) % tuningSize];
		intervalCents.add(note);
		intervalSemitones.add(note / 100);
	}

	// Record and remove period
	periodCents = intervalCents[tuningSize - 1];
	periodSemitones = intervalSemitones[tuningSize - 1];

	intervalCents.remove(tuningSize - 1);
	intervalSemitones.remove(tuningSize - 1);

	// Add unison
	intervalCents.insert(0, 0);
	intervalSemitones.insert(0, 0);
}

Tuning::~Tuning()
{
    
}

void Tuning::setRootNote(int rootNoteIn)
{
	rootMidiNote = rootNoteIn;
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
    return intervalSemitones.size();
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

		int periods = periodSemitones * numPeriods;

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

		int periods = periodCents * numPeriods;

		return intervalCents[modulo(noteDif, tuningSize)] + periods;
	}
	
	return midiNoteIn - (double) rootMidiNote * 100;
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

double Tuning::ratioToSemitones(double ratioIn)
{
    return log2(ratioIn) * 12.0;
}

double Tuning::ratioToCents(double ratioIn)
{
    return log2(ratioIn) * 1200.0;
}

double Tuning::centsToRatio(double centsIn)
{
    return pow(2, centsIn / 1200.0);
}
