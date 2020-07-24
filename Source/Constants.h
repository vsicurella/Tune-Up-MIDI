/*
  ==============================================================================

    Constants.h
    Created: 13 Jul 2020 12:35:06pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TuningDefinition.h"

static String applicationName = "Tune-Up MIDI!";
static String applicationFolderPath = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile(applicationName).getFullPathName();
static String defaultOptionsFilePath = File(applicationFolderPath).getChildFile(applicationName + ".default").getFullPathName();

static ValueTree STD_TUNING; // Initialized in PluginState

static String supportedTuningFileTypes = "*.scl; *.tun; *.mnlgtuns; *.mnlgtuno;";