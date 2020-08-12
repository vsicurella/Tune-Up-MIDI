/*
  ==============================================================================

    ViewToneCircle.cpp
    Created: 5 Aug 2020 11:14:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ViewToneCircle.h"

//==============================================================================
ViewToneCircle::ViewToneCircle(const Tuning& tuningIn, MidiKeyboardState* keyboardInputIn)
	: TuneUpMidiView(tuningIn, keyboardInputIn)
{
	// Set Default Colours
	setColour(circleOutlineColourId, Colours::whitesmoke);
	setColour(intervalStartColourId, Colours::red);
	setColour(intervalMiddleColourId, Colours::red);
	setColour(intervalEndColourId, Colours::red);

	// Calculate interval angles
	for (int i = 0; i < tuning.getTuningSize(); i++)
	{
		degreeAngles.add(scaleDegreeToAngle(i));
	}

	DBGArray(degreeAngles, "ToneCircle Angles");
}

ViewToneCircle::~ViewToneCircle()
{
}

void ViewToneCircle::paint(juce::Graphics& g)
{
	Colour backgroundColour = findColour(TuneUpMidiView::ColourIds::backgroundColourId);
	g.fillAll(backgroundColour);
	
	g.setColour(juce::Colours::black);
	g.drawRect(getLocalBounds(), 1);

	Colour circleColour = findColour(ColourIds::circleOutlineColourId);
	g.setColour(circleColour);
	g.drawEllipse(circleBounds, 1);

	Colour intervalStart = findColour(ColourIds::intervalStartColourId);
	Colour intervalMiddle = findColour(ColourIds::intervalMiddleColourId);
	Colour intervalEnd = findColour(ColourIds::intervalEndColourId);

	intervalTextGradient = ColourGradient(
		intervalStart, degreePoints[0],
		intervalMiddle, degreePoints[degreePoints.size() / 2],
		true
	);

	for (int i = 0; i < degreeAngles.size(); i++)
	{
		float ang = degreeAngles[i];
		Point<float> circleCoord = degreePoints[i];

		g.setColour(circleColour);
		g.fillEllipse(circleCoord.getX() - noteMarkerRadius / 2, +circleCoord.getY() - noteMarkerRadius / 2, noteMarkerRadius, noteMarkerRadius);
	} 

	for (auto noteNum : notesOn)
	{
		int degree = tuning.getScaleDegree(noteNum);
		Point<float> circleCoord = degreePoints[degree];
		Colour intervalColour = intervalTextGradient.getColourAtPosition(degreeAngles[degree] / float_Pi * 2);

		g.setColour(intervalColour.brighter().withAlpha(0.9f));
		g.fillEllipse(circleCoord.getX() - noteMarkerRadius / 2, +circleCoord.getY() - noteMarkerRadius / 2, noteMarkerRadius, noteMarkerRadius);
	}
}

void ViewToneCircle::resized()
{
	center = { getWidth() / 2.0f, getHeight() / 2.0f };
	circleRadius = jmin(getWidth(), getHeight()) * (1 - margin) / 2.0f;
	circleBounds = { center.getX() - circleRadius, center.getY() - circleRadius, circleRadius * 2, circleRadius * 2};

	degreePoints.clear();
	for (auto ang : degreeAngles)
	{
		degreePoints.add(center.getPointOnCircumference(circleRadius, ang));
	}
}

float ViewToneCircle::scaleDegreeToAngle(int scaleDegreeIn) const
{
	return 2.0f * float_Pi * tuning.getNoteInCents(scaleDegreeIn + tuning.getRootNote()) / tuning.getPeriodCents();
}