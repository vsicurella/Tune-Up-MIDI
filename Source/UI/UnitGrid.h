/*
  ==============================================================================

    UnitGrid.h
    Created: 4 Jul 2020 1:10:51pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class UnitGrid
{

public:

	UnitGrid(int horizontalUnits, int verticalUnits)
	{
		xUnits = horizontalUnits;
		yUnits = verticalUnits;
	}

	void setSize(int widthIn, int heightIn)
	{
		width = widthIn;
		height = heightIn;

		xPositions.clear();
		for (int x = 0; x < xUnits; x++)
		{
			xPositions.add(round(x / (double) xUnits * width));
		}

		yPositions.clear();
		for (int y = 0; y < yUnits; y++)
		{
			yPositions.add(round(y / (double) yUnits * height));
		}
	}

	int getX(int xUnitIn)
	{
		if (xUnitIn < xUnits)
			return xPositions[xUnitIn];
		else
			return round(xUnitIn / (double)xUnits * width);
	}

	int getY(int yUnitIn)
	{
		if (yUnitIn < yUnits)
			return yPositions[yUnitIn];
		else
			return round(yUnitIn / (double)yUnits * height);
	}
	
	
	float getXUnitFraction(float fractionalUnitIn)
	{
		return (fractionalUnitIn / xUnits) * width;
	}

	float getYUnitFraction(float fractionalUnitIn)
	{
		return (fractionalUnitIn / yUnits) * height;
	}


private:

	int xUnits;
	int yUnits;

	int width;
	int height;

	Array<int> xPositions;
	Array<int> yPositions;
};