/*
  ==============================================================================

    TuneUpModes.h
    Created: 6 Jul 2020 12:31:48pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace TuneUpMode
{
	enum FreeChannelMode {
		RoundRobin = 1,
		FirstAvailable
	};

	enum PitchbendInputMode
	{
		Ignore = 1,
		AddToTuning,
		OverrideTuning
	};

	enum CreateTuningMode
	{
		EqualTemperament = 1,
		RegularTemperament
	};

};