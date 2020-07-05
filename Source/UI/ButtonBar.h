/*
  ==============================================================================

    ButtonBar.h
    Created: 2 Jul 2020 9:42:52pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "UnitGrid.h"

class ButtonBar
{
	Rectangle<int> bounds;
	int gap = 8;

	Array<Component*> components;

	UnitGrid grid;
	Array<int> units;
	float unitDenominator = 0;

public:

	ButtonBar() : grid(1) {};

	void addComponent(Component* componentIn, int widthUnits = 0)
	{
		components.add(componentIn);

		if (widthUnits == 0)
			widthUnits = componentIn->getName().length();

		units.add(widthUnits);
		unitDenominator += widthUnits;

		grid = UnitGrid(unitDenominator);
		grid.setSize(bounds.getWidth());
	}

	void addComponents(Array<Component*> componentsIn, Array<int> unitsIn = Array<int>())
	{
		for (int i = 0; i < componentsIn.size(); i++)
		{
			components.add(componentsIn[i]);

			int numUnits = unitsIn[i];
			if (numUnits == 0)
				numUnits = componentsIn[i]->getName().length();

			units.add(numUnits);
			unitDenominator += numUnits;
		}

		grid = UnitGrid(unitDenominator);
		grid.setSize(bounds.getWidth());
	}

	void removeComponent(Component* componentToRemove)
	{
		int index = components.indexOf(componentToRemove);

		if (index >= 0)
		{
			components.remove(index);
			unitDenominator -= units[index];
			units.remove(index);

			grid = UnitGrid(unitDenominator);
			grid.setSize(bounds.getWidth());
		}
	}

	void setBounds(int x, int y, int width, int height)
	{
		bounds = { x, y,  width, height };
		grid.setSize(width);
	}

	void setGapSize(int gapSize)
	{
		gap = gapSize;
	}

	Rectangle<float> getComponentBounds(Component* componentIn, bool withGap = true)
	{
		int index = components.indexOf(componentIn);

		if (index >= 0)
		{
			int x;

			if (index == 0)
				x = bounds.getX();
			else
			{
				x = components[index - 1]->getRight();
				if (withGap)
					x += gap;
			}

			float width = bounds.getWidth() * units[index] / unitDenominator;

			return Rectangle<float>(x, bounds.getY(), grid.getUnit(units[index]), bounds.getHeight());
		}
		else
			return Rectangle<float>();
	}

	Rectangle<int> getBounds()
	{
		return bounds;
	}

	Array<Component*> getComponents() 
	{
		return components;
	}
};