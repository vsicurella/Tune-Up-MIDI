/*
  ==============================================================================

    TuningDefinition.cpp
    Created: 28 Jun 2020 5:06:40pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuningDefinition.h"

Identifier TuningDefinition::tuningDefId = Identifier("TuningDefinition");
Identifier TuningDefinition::functionalID = Identifier("TuningIsFunctional");
Identifier TuningDefinition::tuningSizeID = Identifier("TuningSize");
Identifier TuningDefinition::rootMidiNoteID = Identifier("RootNote");
Identifier TuningDefinition::generatorListId = Identifier("GeneratorList");
Identifier TuningDefinition::generatorNodeId = Identifier("GeneratorNode");
Identifier TuningDefinition::generatorOffsetId = Identifier("GeneratorOffset");
Identifier TuningDefinition::generatorAmountId = Identifier("GeneratorAmount");
Identifier TuningDefinition::generatorValueId = Identifier("GeneratorValue");
Identifier TuningDefinition::centsTableID = Identifier("CentsTable");
Identifier TuningDefinition::intervalNodeID = Identifier("IntervalDesc");
Identifier TuningDefinition::intervalValueID = Identifier("IntervalValue");
Identifier TuningDefinition::tuningNameId = Identifier("Name");
Identifier TuningDefinition::tuningDescriptionId = Identifier("Description");

/*
	Renders standard tuning.
*/
TuningDefinition::TuningDefinition()
{
	definition = getStandardTuningDefinition();
}

TuningDefinition::TuningDefinition(ValueTree definitionIn)
{
	if (definitionIn.hasType(tuningDefId))
		definition = definitionIn;
	else
		definition = createEqualTemperamentDefinition(12, 1200.0);
}

TuningDefinition::~TuningDefinition()
{
	listeners.clear();
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
	return definition[functionalID];
}

int TuningDefinition::getTuningSize() const
{
	return definition[tuningSizeID];
}

int TuningDefinition::getRootMidiNote() const
{
	return definition[rootMidiNoteID];
}

void TuningDefinition::setDefinition(ValueTree definitionIn, bool notifyListeners)
{
	definition = definitionIn;

	if (notifyListeners)
		listeners.call(&Listener::tuningChanged);
}

Tuning TuningDefinition::render() const
{
	return renderTuningDefinition(definition);
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
	if (tuningDefinitionIn.hasType(tuningDefId))
	{
		if (tuningDefinitionIn.hasProperty(tuningSizeID))
			return tuningDefinitionIn[tuningSizeID];

		else 
		{
			int determinedSize = 0;

			// static tuning
			if (!tuningDefinitionIn[functionalID])
			{
				ValueTree intervalTableNode = tuningDefinitionIn.getChildWithName(centsTableID);
				
				if (intervalTableNode.isValid())
				{
					determinedSize = intervalTableNode.getNumChildren();

					// subtract one if unison was included
					if ((int)intervalTableNode.getChild(0)[intervalValueID] == 0)
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
						determinedSize = 1;

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
	ValueTree definitionOut(tuningDefId);
	definitionOut.setProperty(functionalID, false, nullptr);
	definitionOut.setProperty(rootMidiNoteID, midiRootNote, nullptr);
	definitionOut.setProperty(tuningNameId, nameIn, nullptr);
	definitionOut.setProperty(tuningDescriptionId, descriptionIn, nullptr); // TODO: default description?

	definitionOut.addChild(
		arrayToTree(centsTable, centsTableID, intervalNodeID, intervalValueID),
		-1, nullptr
	);

	definitionOut.setProperty(tuningSizeID, determineTuningDefinitionSize(definitionOut), nullptr);

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
		definitionOut = ValueTree(tuningDefId);
		definitionOut.setProperty(functionalID, true, nullptr);
		definitionOut.setProperty(rootMidiNoteID, midiRootNote, nullptr);

		if (nameIn.length() == 0)
			nameIn = String(numberOfDivisions) + "-ED " + String(periodInCents) + "cents";
		
		definitionOut.setProperty(tuningNameId, nameIn, nullptr);

		if (description.length() == 0)
			description = String(numberOfDivisions) + " equal divisions of " + String(periodInCents) + " cents.";
		
		definitionOut.setProperty(tuningDescriptionId, description, nullptr);
		definitionOut.setProperty(tuningSizeID, 1, nullptr);

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
	ValueTree definitionOut(tuningDefId);
	definitionOut.setProperty(functionalID, true, nullptr);
	definitionOut.setProperty(rootMidiNoteID, midiRootNote, nullptr);

	if (nameIn.length() == 0)
	{
		nameIn = "New Rank-" + String(generatorCents.size()) + " Regular Temperament";
	}

	definitionOut.setProperty(tuningNameId, nameIn, nullptr);

	if (descriptionIn.length() == 0)
	{
		// TODO: default regular temperament definition
	}

	definitionOut.setProperty(tuningDescriptionId, descriptionIn, nullptr);

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
	definitionOut.setProperty(tuningSizeID, determineTuningDefinitionSize(definitionOut), nullptr);

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
		nameIn = "New Regular Temperament";

	return Tuning(centsTable, midiRootNote, nameIn, descriptionIn);
}

Tuning TuningDefinition::renderTuningDefinition(ValueTree definitionIn)
{
	// Return standard tuning if invalid
	if (!definitionIn.hasType(tuningDefId))
		definitionIn = getStandardTuningDefinition();

	// Render functional tuning
	if (definitionIn[functionalID])
	{
		Array<double> generators;
		Array<int> amounts, offsets;

		extractGeneratorProperties(definitionIn, generators, amounts, offsets);

		// Normalize period
		amounts.set(0, 1);
		offsets.set(0, 0);

		return renderFunctionalTuning(generators, amounts, offsets,
			definitionIn[rootMidiNoteID],
			definitionIn[tuningNameId],
			definitionIn[tuningDescriptionId]
		);
	}

	// Render static tuning
	Array<double> centsTable;
	treeToArray(centsTable, definitionIn.getChildWithName(centsTableID), intervalNodeID, intervalValueID);

	return Tuning(centsTable, definitionIn[rootMidiNoteID], definitionIn[tuningNameId], definitionIn[tuningDescriptionId]);
}