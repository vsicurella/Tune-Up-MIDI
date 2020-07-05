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

	UnitGrid(int numUnits)
	{
		units = numUnits;
	}

	void setSize(int pixelSizeIn)
	{
		size = pixelSizeIn;

		positions.clear();
		for (int x = 0; x < units; x++)
		{
			positions.add(round(x / (double) units * size));
		}
	}

	int getUnit(int unitIn)
	{
		if (unitIn < units)
			return positions[unitIn];
		else
			return round(unitIn / (double)units * size);
	}	
	
	float getUnit(float fractionalUnitIn)
	{
		return (fractionalUnitIn / units) * size;
	}

private:

	int units;
	int size;
	Array<int> positions;
};

class UnitPlane
{
public:

	UnitPlane(int xUnitsIn, int yUnitsIn)
		: xGrid(xUnitsIn), yGrid(yUnitsIn) {}

	void setSize(int width, int height)
	{
		xGrid.setSize(width);
		yGrid.setSize(height);
	}

	int getX(int unitIn)
	{
		return xGrid.getUnit(unitIn);
	}

	float getX(float unitIn)
	{
		return xGrid.getUnit(unitIn);
	}

	int getY(int unitIn)
	{
		return yGrid.getUnit(unitIn);
	}

	float getY(float unitIn)
	{
		return yGrid.getUnit(unitIn);
	}

private:

	UnitGrid xGrid;
	UnitGrid yGrid;
};