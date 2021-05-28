/*
  ==============================================================================

    ViewToneCircle.h
    Created: 5 Aug 2020 11:14:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ViewBase.h"

//==============================================================================
/*
*/
class ViewToneCircle  : public TuneUpMidiView
{
public:

	enum ColourIds
	{
		circleOutlineColourId = 0x00100100,
		intervalStartColourId = 0x00200100,
		intervalMiddleColourId = 0x00200101,
		intervalEndColourId = 0x00200102
	};

public:
    ViewToneCircle(const Tuning& tuningIn, ValueTree tuningDefinitionIn, MidiKeyboardState* keyboardInputIn);
    ~ViewToneCircle() override;

    void paint (juce::Graphics&) override;
    void resized() override;

//==============================================================================
private:

	float scaleDegreeToAngle(int scaleDegreeIn) const;

//==============================================================================

private:

	Array<float> degreeAngles;
	Array<Point<float>> degreePoints;
	ColourGradient intervalTextGradient;

	int tuningSize = 1;
	double centsPeriod = 1200;

	// Drawing helpers
	Point<float> center;
	
	float margin = 1.0f / 8.0f;
	float circleRadius;
	Rectangle<float> circleBounds;

	float noteMarkerRadius = 6.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewToneCircle)
};
