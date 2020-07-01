/*
  ==============================================================================

    CommonFunctions.h
    Created: 6 May 2019 7:31:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

template <class T, class U>
struct Pair
{
	T key;
	U value;

	Pair<T, U>(T keyIn, U valueIn)
	{
		key = keyIn;
		value = valueIn;
	}
};

struct MidiPitch : Pair<int, int>
{
    MidiPitch(int midiNoteIn = -1, int pitchbendIn = 8192)
    : Pair<int, int>(midiNoteIn, pitchbendIn) {}
};

struct ChannelMidiPitch : Pair<int, MidiPitch>
{
	ChannelMidiPitch(int channelIn = -1, MidiPitch pitchIn = MidiPitch())
		: Pair<int, MidiPitch>(channelIn, pitchIn) {}

	bool operator==(const ChannelMidiPitch& other) const
	{
		if (other.key == key && 
			other.value.key == value.key && 
			other.value.value == value.value)

			return true;

		return false;
	}
};

class TextFilterIntOrSpace : public TextEditor::InputFilter
{
	String filterNewText(TextEditor&, const String& newInput) override
	{
		String out;

		for (int c = 0; c < newInput.length(); c++)
		{
			if (newInput[c] >= 48 && newInput[c] < 58)
			{
				out += newInput[c];
				continue;
			}

			out += " ";
		}

		return out;
	}
};

class TextFilterInt : public TextEditor::InputFilter
{
	String filterNewText(TextEditor&, const String& newInput) override
	{
		String out;

		for (int c = 0; c < newInput.length(); c++)
		{
			if (newInput[c] == 45 || (newInput[c] >= 48 && newInput[c] < 58))
			{
				out += newInput[c];
				continue;
			}
		}

		return out;
	}
};

static Point<int> pointFromString(Point<int>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<int>(val1.getIntValue(), val2.getIntValue());
	return p;
}

static Point<float> pointFromString(Point<float>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<float>(val1.getFloatValue(), val2.getFloatValue());
	return p;
}

static Point<double> pointFromString(Point<double>& p, String pointToStringIn)
{
	String val1 = pointToStringIn.upToFirstOccurrenceOf(",", false, true);
	String val2 = pointToStringIn.fromFirstOccurrenceOf(",", false, true);
	p = Point<double>(val1.getDoubleValue(), val2.getDoubleValue());
	return p;
}

static ValueTree extractNode(ValueTree nodeOrigin, Identifier nodeType)
{
    ValueTree nodeOut;
    
    if (nodeOrigin.hasType(nodeType))
        nodeOut = nodeOrigin;
    else
        nodeOut = nodeOrigin.getChildWithName(nodeType);
        
    return nodeOut;
}

static Array<ValueTree> extractNodes(ValueTree nodeOrigin, Identifier nodeType)
{
	Array<ValueTree> nodesOut;
	ValueTree childNode;
	
	int numChildren = nodeOrigin.getNumChildren();
	int level = 0;

	while (level < numChildren)
	{
		childNode = nodeOrigin.getChild(level);

		if (childNode.hasType(nodeType))
			nodesOut.add(childNode);

		else if (childNode.getNumChildren() > 0)
			nodesOut.addArray(extractNodes(childNode, nodeType));

		level++;
	}

	return nodesOut;
}

template <class T>
static ValueTree arrayToTree(const Array<T>& arrayIn, Identifier arrayId, Identifier valueNodeId, Identifier valuePropertyId)
{
	ValueTree arrayTree = ValueTree(arrayId);
	ValueTree item;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(valueNodeId);
		item.setProperty(valuePropertyId, arrayIn[i], nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	return arrayTree;
}

template <class T>
static void treeToArray(Array<T>& arrayOut, ValueTree arrayNodeIn, Identifier valueNodeId, Identifier valuePropertyId)
{
	if (arrayNodeIn.isValid())
	{
		for (auto child : arrayNodeIn)
		{
			if (child.hasType(valueNodeId))
				arrayOut.add(child[valuePropertyId]);
		}
	}
}

template <class T>
static void add_array_to_node(ValueTree nodeIn, const Array<Point<T>>& arrayIn, Identifier arrayID, Identifier itemId)
{
	ValueTree arrayTree = ValueTree(arrayID);
	ValueTree item;
	for (int i = 0; i < arrayIn.size(); i++)
	{
		item = ValueTree(itemId);
		item.setProperty("Value", arrayIn[i].toString(), nullptr);
		arrayTree.addChild(item, i, nullptr);
	}

	nodeIn.addChild(arrayTree, -1, nullptr);
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<int>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value").toString()));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<float>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
}

static void get_array_from_node(const ValueTree nodeIn, Array<Point<double>>& arrayIn, Identifier arrayID)
{
	ValueTree childArray = nodeIn.getChildWithName(arrayID);

	if (childArray.isValid())
		for (int i = 0; i < childArray.getNumChildren(); i++)
			arrayIn.add(pointFromString(arrayIn.getReference(0), childArray.getChild(i).getProperty("Value")));
}

template <class T>
static void set_value_in_array(ValueTree nodeIn, Identifier arrayID, int indexToChange, T valueToSet)
{
    ValueTree arrayNode = nodeIn.getChildWithName(arrayID);
    
    if (arrayNode.isValid() && indexToChange >= 0 && indexToChange < arrayNode.getNumChildren())
    {
        arrayNode.getChild(indexToChange).setProperty("Value", valueToSet, nullptr);
    }
}

static int modulo(int numIn, int mod)
{
	int val = 0;

	if (mod != 0)
		val = ((numIn % mod) + mod) % mod;

	return val;
}

template <class T>
static T sumArray(Array<T> arrayIn)
{
	T sum = 0;
	for (auto i : arrayIn)
		sum += i;

	return sum;
}

template <class T>
static T sumUpToIndex(Array<T> arrayIn, int indexIn)
{
	indexIn = modulo(indexIn, arrayIn.size());
	T sum = 0;

	for (int i = 0; i < indexIn; i++)
	{
		sum += arrayIn[i];
	}

	return sum;
}

template <class T>
static T sumUpToRingIndex(Array<T> arrayIn, int indexIn)
{
	T sum = 0;

	for (int i = 0; i < indexIn; i++)
	{
		sum += arrayIn[i % arrayIn.size()];
	}

	return sum;
}

template <class T>
Array<T> reverseArray(const Array<T>& arrayIn)
{
	Array<T> outArray;
	outArray.resize(arrayIn.size());

	for (int i = 0; i < arrayIn.size(); i++)
	{
		outArray.set(i, arrayIn.getUnchecked(arrayIn.size() - i - 1));
	}

	return outArray;
}

template <class T>
String arrayToString(const Array<T>& arrayIn)
{
    String arrayString = "";
    
    for (int i = 0; i < arrayIn.size(); i++)
    {
        arrayString+= String(arrayIn[i]);
        
        if (i + 1 < arrayIn.size())
            arrayString += ", ";
    }
    
    return arrayString;
}

template <class T>
static void DBGArray(Array<T>& arrayIn, String arrayName="Array")
{
	DBG(arrayName + ":");

	for (int i = 0; i < arrayIn.size(); i++)
	{
		DBG(String(i) + ": " + String(arrayIn[i]));
	}
}
