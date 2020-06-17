/*
  ==============================================================================

    DynamicTuning.h
    Created: 17 Jun 2020 5:07:45pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Tuning.h"

class DynamicTuning : public Tuning
{
	double generator;
	double generatorSemitones;
	Array<double> generators; // TODO

	int numGeneratorsDown = 1;
	Array<int> generatorOffsets; // TODO

	int rankNumber = 2;

	void updateTuning();

public:

	DynamicTuning(int rankNumberIn = 2, int tuningSizeIn = 12, double generatorCentsIn = 700, double periodCentsIn = 1200);
	~DynamicTuning();

	void setRankNumber(int rankNumberIn);
	void setNumGeneratorsDown(int numGeneratorsBelowRoot);

	void setGenerator(double generatorIn);
	//void setGenerator(int generatorNum, double generatorValue);

	void setPeriod(double periodIn);

	int getRankNumber() const;

public:

	class Listener
	{
	public:

		~Listener() {};

		virtual void periodChanged() {};
		virtual void generatorChanged(int numGenerator) {};
		virtual void tuningChanged() {};
	};

	void addListener(Listener* listenerIn);
	void removeListener(Listener* listenerIn);


protected:

	ListenerList<Listener> listeners;

public:

	static Array<double> getRank1TableCents(int size, double generator);

	static Array<double> getRank2TableCents(int size, double period, double generator, int generatorsDown = 0);
};