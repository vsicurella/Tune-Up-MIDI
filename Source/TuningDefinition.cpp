/*
  ==============================================================================

    TuningDefinition.cpp
    Created: 28 Jun 2020 5:06:40pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuningDefinition.h"

/*
	Renders standard tuning.
*/
TuningDefinition::TuningDefinition()
{
	definition = getStandardTuningDefinition();
}

TuningDefinition::TuningDefinition(ValueTree definitionIn)
{
	if (definitionIn.hasType(tuningDefinitionId))
		definition = definitionIn;
	else
		definition = createEqualTemperamentDefinition(12, 1200.0);
}

TuningDefinition::~TuningDefinition()
{

}

//TuningDefinition TuningDefinition::operator=(const TuningDefinition& toCopy)
//{ 
//	return TuningDefinition(toCopy.definition); 
//}

ValueTree TuningDefinition::getDefinition() const
{
	return definition;
}

bool TuningDefinition::isFunctional() const
{
	return definition[functionalId];
}

int TuningDefinition::getTuningSize() const
{
	return definition[tuningSizeId];
}

int TuningDefinition::getRootMidiNote() const
{
	return definition[rootMidiNoteId];
}

void TuningDefinition::setDefinition(ValueTree& definitionIn)
{
	definition = definitionIn;
}

Tuning TuningDefinition::render() const
{
	return renderTuningDefinition(definition);
}

int TuningDefinition::isValid() const
{
	return isValid(definition);
}

int TuningDefinition::isValid(ValueTree definitionIn)
{
	if (!definitionIn.hasType(tuningDefinitionId))
		// Not a tuning definition node
		return 0;

	if (definitionIn.getNumChildren() != 1 || definitionIn.getChild(0).getNumChildren() < 1)
		return -1; // Does not have a valid list of generators or any static intervals

	// TODO

	return 1;
}

/*
	Returns definition for 12EDO
*/
ValueTree TuningDefinition::getStandardTuningDefinition()
{
	return createEqualTemperamentDefinition(12, 1200.0);
}

/*
	Determines the size of a tuning, which may not be trivial with some functional tuning defintions.
*/
int TuningDefinition::determineTuningDefinitionSize(ValueTree tuningDefinitionIn)
{
	if (tuningDefinitionIn.hasType(tuningDefinitionId))
	{
		if (tuningDefinitionIn.hasProperty(tuningSizeId))
			return tuningDefinitionIn[tuningSizeId];

		else 
		{
			int determinedSize = 0;

			// static tuning
			if (!tuningDefinitionIn[functionalId])
			{
				ValueTree intervalTableNode = tuningDefinitionIn.getChildWithName(centsTableId);
				
				if (intervalTableNode.isValid())
				{
					determinedSize = intervalTableNode.getNumChildren();

					// subtract one if unison was included
					if ((int)intervalTableNode.getChild(0)[intervalValueId] == 0)
						determinedSize--;
				}
			}

			// functional tuning
			else
			{
				ValueTree generatorList = tuningDefinitionIn.getChildWithName(generatorListId);

				if (generatorList.isValid())
				{
					double period = generatorList.getChild(0)[generatorValueId];

					if (period != 0.0)
					{
						// Equal temperament
						if (generatorList.getNumChildren() == 1)
						{
							return 1;
						}

						double gen, factor;

						// skip first generator (period) because it will always be 1
						for (int i = 1; i < generatorList.getNumChildren(); i++)
						{
							ValueTree node = generatorList.getChild(i);
							gen = node[generatorValueId];

							// check the generator is a factor of the period
							factor = period / gen;
							if (factor < 1)
								factor = 1.0 / factor;

							// TODO: add tolerance?
							if (factor == (int)factor)
							{
								determinedSize += factor;
							}
							else
							{
								determinedSize += (int) node[generatorAmountId];
							}
						}
					}
				}
			}

			return determinedSize;
		}
	}

	return 0;
}

/*
	Extracts generator properties and adds values to arrays input
*/
void TuningDefinition::extractGeneratorProperties(
	ValueTree tuningDefinitionIn,
	Array<double>& generatorValuesOut,
	Array<int>& generatorAmountsOut,
	Array<int>& generatorOffsetsOut)
{
	ValueTree generatorList = tuningDefinitionIn.getChildWithName(generatorListId);

	if (generatorList.isValid())
	{
		generatorValuesOut.clear();
		generatorAmountsOut.clear();
		generatorOffsetsOut.clear();

		for (auto node : generatorList)
		{
			if (node.hasType(generatorNodeId))
			{
				generatorValuesOut.add(node[generatorValueId]);
				generatorAmountsOut.add(node[generatorAmountId]);
				generatorOffsetsOut.add(node[generatorOffsetId]);
			}
		}
	}
}

/*
	Creates a static tuning definition containing a tuning table, where the last value is interpreted as the period.
	Unison will be handled whether or not provided.
*/
ValueTree TuningDefinition::createStaticTuningDefinition(
	Array<double> centsTable, 
	int midiRootNote, 
	String nameIn,
	String descriptionIn)
{
	ValueTree definitionOut(tuningDefinitionId);
	definitionOut.setProperty(functionalId, false, nullptr);
	definitionOut.setProperty(rootMidiNoteId, midiRootNote, nullptr);

	// Strip file directory and extension if in name

	if (File::isAbsolutePath(nameIn))
	{
		nameIn = File(nameIn).getFileNameWithoutExtension();
	}

	definitionOut.setProperty(tuningNameId, nameIn, nullptr);
	definitionOut.setProperty(tuningDescriptionId, descriptionIn, nullptr); // TODO: default description?

	definitionOut.addChild(
		arrayToTree(centsTable, centsTableId, intervalNodeId, intervalValueId),
		-1, nullptr
	);

	definitionOut.setProperty(tuningSizeId, determineTuningDefinitionSize(definitionOut), nullptr);

	return definitionOut;
}

/*
	Creates a functional tuning definition for an equal temperament using a cent value as the period.
*/
ValueTree TuningDefinition::createEqualTemperamentDefinition(
	int numberOfDivisions, 
	double periodInCents, 
	int midiRootNote, 
	String nameIn,
	String description)
{
	ValueTree definitionOut;
	
	if (numberOfDivisions > 0 && periodInCents > 0)
	{
		definitionOut = ValueTree(tuningDefinitionId);
		definitionOut.setProperty(functionalId, true, nullptr);
		definitionOut.setProperty(rootMidiNoteId, midiRootNote, nullptr);

		if (nameIn.length() == 0)
			nameIn = String(numberOfDivisions) + "-ED " + String(periodInCents) + "cents";
		
		definitionOut.setProperty(tuningNameId, nameIn, nullptr);

		if (description.length() == 0)
			description = String(numberOfDivisions) + " equal divisions of " + String(periodInCents) + " cents.";
		
		definitionOut.setProperty(tuningDescriptionId, description, nullptr);
		definitionOut.setProperty(tuningSizeId, 1, nullptr);

		ValueTree generatorList(generatorListId);
		ValueTree node(generatorNodeId);

		node.setProperty(generatorValueId, periodInCents / numberOfDivisions, nullptr);
		node.setProperty(generatorAmountId, 1, nullptr);
		node.setProperty(generatorOffsetId, 0, nullptr);

		generatorList.addChild(node, 0, nullptr);
		definitionOut.addChild(generatorList, 0, nullptr);
	}

	return definitionOut;
}

/*
	Creates a functional tuning definition for an equal temperament using a ratio as the period.
*/
ValueTree TuningDefinition::createEqualTemperamentDefinition(
	int numberOfDivisions, 
	float periodAsRatio, 
	int midiRootNote, 
	String nameIn,
	String description)
{
	if (nameIn.length() == 0)
		nameIn = String(numberOfDivisions) + "-ED" + String(periodAsRatio);

	if (description.length() == 0)
		description = String(numberOfDivisions) + " equal divisions of the ratio " + String(periodAsRatio);

	return createEqualTemperamentDefinition(numberOfDivisions, ratioToCents(periodAsRatio), midiRootNote, nameIn, description);
}

/*
	Creates a functional tuning definition for an equal temperament using a written pitch as the period, which will be parsed by Scala standards.
*/
ValueTree TuningDefinition::createEqualTemperamentDefinition(
	int numberOfDivisions, 
	String periodValue, 
	int midiRootNote, 
	String nameIn,
	String description)
{
	if (nameIn.length() == 0)
		nameIn = String(numberOfDivisions) + "-ED(" + periodValue + ")";

	if (description.length() == 0)
		description = String(numberOfDivisions) + " equal divisions of " + periodValue;

	IntervalType type = getIntervalType(periodValue);

	// TODO: implement comma and NofEDO types

	String name = String(numberOfDivisions) + "-EDO";
	double periodCents = convertInterval(type, IntervalType::Cents, periodValue);
	return createEqualTemperamentDefinition(numberOfDivisions, periodCents, midiRootNote, nameIn, description);
}

/*
	Creates a functional tuning definition for a regular temperament, allowing for dynamic tonality.
	The first generator is used as the period and will always use an amount of 1 with no offset, regardless of provided values.
*/
ValueTree TuningDefinition::createRegularTemperamentDefinition(
	Array<double> generatorCents,
	Array<int> generatorAmounts,
	Array<int> generatorsDown,
	int midiRootNote,
	String nameIn,
	String descriptionIn)
{
	ValueTree definitionOut(tuningDefinitionId);
	definitionOut.setProperty(functionalId, true, nullptr);
	definitionOut.setProperty(rootMidiNoteId, midiRootNote, nullptr);

	if (nameIn.length() == 0)
	{
		nameIn = "New Rank-" + String(generatorCents.size()) + " Temperament";
	}

	definitionOut.setProperty(tuningNameId, nameIn, nullptr);

	ValueTree generatorList(generatorListId);
	for (int i = 0; i < generatorCents.size(); i++)
	{
		ValueTree node(generatorNodeId);

		node.setProperty(generatorValueId, generatorCents[i], nullptr);
		node.setProperty(generatorAmountId, generatorAmounts[i], nullptr);
		node.setProperty(generatorOffsetId, generatorsDown[i], nullptr);

		generatorList.addChild(node, i, nullptr);
	}

	definitionOut.addChild(generatorList, 0, nullptr);

	// TODO: do this without packing into ValueTree?
	int tuningSize = determineTuningDefinitionSize(definitionOut);
	definitionOut.setProperty(tuningSizeId, tuningSize, nullptr);

	if (descriptionIn.length() == 0)
	{
		descriptionIn = "A " + String(tuningSize) + "-note temperament generated as follows:";
		for (int i = 0; i < generatorCents.size(); i++)
		{
			ValueTree node = generatorList.getChild(i);
			descriptionIn += "\n\t" + String(i + 1) + ": ";
			descriptionIn += "\t" + String(round(generatorCents[i] * 1000) / 1000.0) + ",";
			descriptionIn += "\t" + String(generatorAmounts[i]) + ",";
			descriptionIn += "\t" + String(generatorsDown[i]);
		}
	}

	definitionOut.setProperty(tuningDescriptionId, descriptionIn, nullptr);

	return definitionOut;
}

/*
	Renders a tuning given a functional tuning parameters.
	The first generator is used as the period will always use an amount of 1 with no offset, regardless of provided values.
*/

Tuning TuningDefinition::renderFunctionalTuning(
	Array<double> generatorCents,
	Array<int> generatorAmounts,
	Array<int> generatorsDown,
	int midiRootNote,
	String nameIn,
	String descriptionIn)
{
	Array<double> centsTable = generateRegularTemperament(
		generatorCents,
		generatorAmounts,
		generatorsDown
	);

	if (nameIn.length() == 0)
		nameIn = "New Rank-" + String(generatorCents.size()) + " Temperament";

	return Tuning(centsTable, midiRootNote, nameIn, descriptionIn);
}

Tuning TuningDefinition::renderTuningDefinition(ValueTree definitionIn)
{
	// Return standard tuning if invalid
	if (!definitionIn.hasType(tuningDefinitionId))
		definitionIn = getStandardTuningDefinition();

	// Render functional tuning
	if (definitionIn[functionalId])
	{
		Array<double> generators;
		Array<int> amounts, offsets;

		extractGeneratorProperties(definitionIn, generators, amounts, offsets);

		// Normalize period
		amounts.set(0, 1);
		offsets.set(0, 0);

		return renderFunctionalTuning(generators, amounts, offsets,
			definitionIn[rootMidiNoteId],
			definitionIn[tuningNameId],
			definitionIn[tuningDescriptionId]
		);
	}

	// Render static tuning
	Array<double> centsTable;
	treeToArray(centsTable, definitionIn.getChildWithName(centsTableId), intervalNodeId, intervalValueId);

	return Tuning(centsTable, definitionIn[rootMidiNoteId], definitionIn[tuningNameId], definitionIn[tuningDescriptionId]);
}