/*
  ==============================================================================

    TuningMath.h
    Created: 9 Jun 2020 1:26:25pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum IntervalType
{
	Invalid = 0,
	Cents = 1,
	Ratio = 2,
	Decimal = 3,
	NofEDO = 4
};

static IntervalType getIntervalType(String lineIn)
{
	bool hasDecimal = false;
	bool hasFraction = false;
	bool hasComma = false;
	bool hasBackslash = false;

	auto hasAType = [&hasDecimal, &hasFraction, &hasComma, &hasBackslash]()
	{
		return hasDecimal || hasFraction || hasComma || hasBackslash;
	};

	for (int i = 0; i < lineIn.length(); i++)
	{
		char c = lineIn[i];

		if (c == 92)
		{
			if (hasAType())
				return IntervalType::Invalid;

			hasBackslash = true;
		}

		else if (c < 44 || c > 59 || c == 45)
			return IntervalType::Invalid;

		else if (c == 46)
		{
			if (hasAType())
				return IntervalType::Invalid;

			hasDecimal = true;
		}

		else if (c == 47 || c == 58)
		{
			if (hasAType())
				return IntervalType::Invalid;

			hasFraction = true;
		}


		else if (c == 44)
		{
			if (hasAType())
				return IntervalType::Invalid;

			hasComma = true;
		}
	}

	if (hasDecimal)
		return IntervalType::Cents;

	else if (hasFraction)
		return IntervalType::Ratio;

	else if (hasComma)
		return IntervalType::Decimal;

	else if (hasBackslash)
		return IntervalType::NofEDO;

	return IntervalType::Invalid;
}

static double centsToRatio(double centsIn)
{
	return pow(2, centsIn / 1200);
}

static double ratioToCents(double ratioIn)
{
	return log2(ratioIn) * 1200;
}

static double parseRatio(String ratioIn)
{
	StringRef separator = ratioIn.containsChar(':') ? ":" : "/";
	return ratioIn.upToFirstOccurrenceOf(separator, false, true).getDoubleValue() 
		/ ratioIn.fromLastOccurrenceOf(separator, false, true).getDoubleValue();
}

static double convertInterval(IntervalType typeIn, IntervalType typeOut, String intervalIn)
{
	if (typeIn == IntervalType::Cents && typeOut == IntervalType::Ratio)
	{
		return centsToRatio(intervalIn.getDoubleValue());
	}

	else if (typeIn == IntervalType::Ratio && typeOut == IntervalType::Cents)
	{
		return ratioToCents(parseRatio(intervalIn));
	}

	else if (typeIn == typeOut)
	{
		switch (typeIn)
		{
		case (IntervalType::Ratio):
			return parseRatio(intervalIn);
		default:
			return intervalIn.getDoubleValue();
		}
	}

	return -1.0;
}

static double intervalTo(IntervalType typeOut, String intervalIn)
{
	return convertInterval(getIntervalType(intervalIn), typeOut, intervalIn);
}