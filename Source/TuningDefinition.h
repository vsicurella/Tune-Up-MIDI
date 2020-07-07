/*
  ==============================================================================

    TuningDefinition.h
    Created: 28 Jun 2020 5:06:40pm
    Author:  Vincenzo

	Helper class for creating tuning definitions and generate tuning objects,
	and also acts as a tuning definition wrapper.

  ==============================================================================
*/

#pragma once
#include "IDs.h"
#include "TuningMath.h"
#include "Tuning.h"

using namespace TuneUpIDs;

class TuningDefinition
{
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


protected:

	ValueTree definition;

public:

	/*
		Renders standard tuning.
	*/
	TuningDefinition();

	TuningDefinition(ValueTree definitionIn);

	~TuningDefinition();

	/*
		Resets definition and copies listeners, but doesn't call listeners
	*/
	//TuningDefinition operator=(const TuningDefinition& toCopy);

	ValueTree getDefinition() const;

	bool isFunctional() const;

	int getTuningSize() const;

	int getRootMidiNote() const;

	void setDefinition(ValueTree definitionIn, bool notifyListeners=true);

	Tuning render() const;

	/*
		Returns definition for 12EDO
	*/
	static ValueTree getStandardTuningDefinition();

	/*
		Determines the size of a tuning, which may not be trivial with some functional tuning defintions.
	*/
	static int determineTuningDefinitionSize(ValueTree tuningDefinitionIn);

	/*
		Extracts generator properties and adds values to arrays input
	*/
	static void extractGeneratorProperties(
		ValueTree tuningDefinitionIn,
		Array<double>& generatorValuesOut,
		Array<int>& generatorAmountsOut,
		Array<int>& generatorOffsetsOut
	);

	/*
		Creates a static tuning definition containing a tuning table, where the last value is interpreted as the period.
		Unison will be handled whether or not provided.
	*/
	static ValueTree createStaticTuningDefinition(
		Array<double> centsTable, 
		int midiRootNote = 60, 
		String nameIn = "",
		String descriptionIn = ""
	);

	/*
		Creates a functional tuning definition for an equal temperament using a cent value as the period.
	*/
	static ValueTree createEqualTemperamentDefinition(
		int numberOfDivisions, 
		double periodInCents, 
		int midiRootNote = 60, 
		String nameIn = "", 
		String description = ""
	);

	/*
		Creates a functional tuning definition for an equal temperament using a ratio as the period.
	*/
	static ValueTree createEqualTemperamentDefinition(
		int numberOfDivisions, 
		float periodAsRatio, 
		int midiRootNote = 60, 
		String nameIn = "", 
		String description = ""
	);

	/*
		Creates a functional tuning definition for an equal temperament using a written pitch as the period, which will be parsed by Scala standards.
	*/
	static ValueTree createEqualTemperamentDefinition(
		int numberOfDivisions, 
		String periodValue, 
		int midiRootNote = 60, 
		String nameIn = "",
		String description = ""
	);

	/*
		Creates a functional tuning definition for a regular temperament, allowing for dynamic tonality.
		The first generator is used as the period and will always use an amount of 1 with no offset, regardless of provided values.
	*/
	static ValueTree createRegularTemperamentDefinition(
		Array<double> generatorCents,
		Array<int> generatorAmounts,
		Array<int> generatorsDown,
		int midiRootNote = 60,
		String nameIn = "",
		String descriptionIn = ""
	);

	/*
		Renders a tuning given a functional tuning parameters.
		The first generator is used as the period will always use an amount of 1 with no offset, regardless of provided values.
	*/
	static Tuning renderFunctionalTuning(
		Array<double> generatorCents,
		Array<int> generatorAmounts,
		Array<int> generatorsDown,
		int midiRootNote = 60,
		String nameIn = "",
		String descriptionIn = ""
	);

	/*
		Renders a tuning based on the given definition.
	*/
	static Tuning renderTuningDefinition(ValueTree tuningDefinitionIn);
};