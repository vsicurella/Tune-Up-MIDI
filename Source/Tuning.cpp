/*
  ==============================================================================

    Tuning.cpp
    Created: 26 Nov 2019 9:16:30pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "Tuning.h"

Identifier Tuning::tuningDefId = Identifier("TuningDefinition");
Identifier Tuning::tuningSizeID = Identifier("TuningSize");
Identifier Tuning::rootMidiNoteID = Identifier("RootNote");
Identifier Tuning::generatorListID = Identifier("GeneratorList");
Identifier Tuning::generatorOffsetsID = Identifier("GeneratorOffset");
Identifier Tuning::generatorAmountsID = Identifier("GeneratorAmounts");
Identifier Tuning::generatorValueID = Identifier("GeneratorValue");
Identifier Tuning::centsTableID = Identifier("CentsTable");
Identifier Tuning::intervalNodeID = Identifier("IntervalDesc");
Identifier Tuning::intervalValueID = Identifier("IntervalValue");
Identifier Tuning::tuningDescID = Identifier("Description");

Tuning::Tuning()
{

}

Tuning::Tuning(ValueTree tuningPropertiesIn)
{
	DBG("Creating Tuning: \n" + tuningPropertiesIn.toXmlString());

	tuningSize = (int) tuningPropertiesIn[tuningSizeID];
	periodCents = (double) tuningPropertiesIn.getChildWithName(generatorListID)[generatorValueID];
	periodSemitones = periodCents / 100.0;

	for (auto child : tuningPropertiesIn.getChildWithName(centsTableID))
	{
		double cents = (double)child[intervalValueID];
		intervalCents.add(cents);
		intervalSemitones.add(cents / 100.0);
	}

	description = tuningPropertiesIn[tuningDescID];
}

Tuning::Tuning(const Array<double>& centsTable, int rootIndex, String description)
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

void Tuning::setDescription(String descIn)
{
	description = descIn;
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

String Tuning::getDescription() const
{
	return description;
}

ValueTree Tuning::getTuningDefinition()
{
	if (!definition.isValid())
	{
		definition = createTuningDefinition(tuningSize, periodCents, intervalCents, description);
	}
	
	return definition;
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

ValueTree Tuning::createTuningDefinition(int tuningSize, double periodCents, Array<double> centsTable, String descriptionIn)
{
	ValueTree tree(tuningDefId);
	tree.setProperty(tuningSizeID, tuningSize, nullptr);
	
	ValueTree generator(generatorListID);
	generator.setProperty(generatorValueID, periodCents, nullptr);
	tree.addChild(generator, -1, nullptr);

	ValueTree cents(centsTableID);
	for (auto c : centsTable)
	{
		ValueTree intervalNode(intervalNodeID);
		intervalNode.setProperty(intervalValueID, c, nullptr);
		cents.addChild(intervalNode, -1, nullptr);
	}
	tree.addChild(cents, -1, nullptr);

	tree.setProperty(tuningDescID, descriptionIn, nullptr);

	return tree;
}