/*
  ==============================================================================

    DynamicTuning.cpp
    Created: 17 Jun 2020 5:07:45pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DynamicTuning.h"

DynamicTuning::DynamicTuning(int rankNumberIn, int tuningSizeIn, double generatorCentsIn, double periodCentsIn)
	: rankNumber(rankNumberIn), generator(generatorCentsIn)
{
	tuningSize = tuningSizeIn;

	if (rankNumber == 1)
		periodCents = generator * tuningSize;
	else
		periodCents = periodCentsIn;

	periodSemitones = periodCents / 100;
	generatorSemitones = generator / 100;

	updateTuning();
}

DynamicTuning::DynamicTuning(const DynamicTuning& tuningToCopy)
	: DynamicTuning(tuningToCopy.rankNumber, tuningToCopy.tuningSize, tuningToCopy.generator, tuningToCopy.periodCents)
{
	periodSemitones = tuningToCopy.periodSemitones;
	generatorSemitones = tuningToCopy.generatorSemitones;
	 
	intervalCents = tuningToCopy.intervalCents;
	intervalSemitones = tuningToCopy.intervalSemitones;
}

DynamicTuning::~DynamicTuning()
{

}

void DynamicTuning::updateTuning()
{
	if (rankNumber == 2)
		intervalCents = getRank2TableCents(tuningSize, periodCents, generator, numGeneratorsDown);

	else
		intervalCents = { generator };

	intervalSemitones.clear();
	for (auto i : intervalCents)
		intervalSemitones.add(i / 100.0);

	listeners.call(&Listener::tuningChanged);
}

void DynamicTuning::setRankNumber(int rankNumberIn)
{
	rankNumber = rankNumberIn;

	if (rankNumber == 1)
		periodCents = generator * tuningSize;

	periodSemitones = periodCents / 100;

	updateTuning();
}

void DynamicTuning::setNumGeneratorsDown(int numGeneratorsBelowRoot)
{
	numGeneratorsDown = numGeneratorsBelowRoot;

	updateTuning();
}

void DynamicTuning::setGenerator(double generatorIn)
{
	generator = generatorIn;
	generatorSemitones = generator / 100;

	// TODO: update with multi generators
	listeners.call(&Listener::generatorChanged, 0);
	updateTuning();
}

void DynamicTuning::setPeriod(double periodIn)
{
	if (rankNumber > 1)
	{
		periodCents = periodIn;
		periodSemitones = periodCents / 100;

		listeners.call(&Listener::periodChanged);
		updateTuning();
	}
}

int DynamicTuning::getRankNumber() const
{
	return rankNumber;
}

Array<double> DynamicTuning::getRank2TableCents(int size, double period, double generator, int generatorsDown)
{
	Array<double> intervals;
	double cents = generator * -generatorsDown;
	for (int i = 0; i < size; i++)
	{
		if (cents < 0 || cents >= period)
		{
			cents -= period * floor(cents / period);
		}

		intervals.addIfNotAlreadyThere(cents);
		cents += generator;
	}

	intervals.sort();
	intervals.remove(0);
	intervals.add(period);

	return intervals;
}