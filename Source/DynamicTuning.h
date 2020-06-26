/*
  ==============================================================================

    DynamicTuning.h
    Created: 17 Jun 2020 5:07:45pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
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
	DynamicTuning(ValueTree tuningProperties);
	DynamicTuning(const DynamicTuning& tuningToCopy);
	~DynamicTuning();

	const bool isDynamic = true;

	void setRankNumber(int rankNumberIn);
	void setNumGeneratorsDown(int numGeneratorsBelowRoot);

	void setGenerator(double generatorIn);
	//void setGenerator(int generatorNum, double generatorValue);

	void setPeriod(double periodIn);

	int getRankNumber() const;

public:

	class Listener : public Tuning::Listener
	{
	public:

		virtual ~Listener() {};

		virtual void periodChanged() {};
		virtual void generatorChanged(int numGenerator) {};
		virtual void tuningChanged() {};
	};

	void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
	void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

	ListenerList<Listener> listeners;

public:

	static Array<double> getRank1TableCents(int size, double generator);

	static Array<double> getRank2TableCents(int size, double period, double generator, int generatorsDown = 0);

	static ValueTree dynamicTuningDefinition(int tuningSizeIn, Array<double> generatorsIn, Array<double> generatorAmtsIn, Array<double> generatorOffsetsIn, String descriptionIn = "");
};