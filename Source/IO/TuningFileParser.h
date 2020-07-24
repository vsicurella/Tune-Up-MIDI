/*
  ==============================================================================

    TuningFileParser.h
    Created: 23 Jul 2020 8:04:50pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <iostream>;

#include <JuceHeader.h>

#include "TUN_V2/SCL_Import.h"
#include "TUN_V2/TUN_Scale.h"
#include "TUN_V2/TUN_MultiScaleFile.h"
#include "TUN_V2/TUN_Formula.h"
#include "TUN_V2/TUN_EmbedHTML.h"

#include "../Tuning.h"
#include "../TuningDefinition.h"

class TuningFileParser
{

public:

	// TODO: move somewhere more common?

	enum TuningType
	{
		INV = 0,	// Invalid
		FTD,		// Function Tuning Definition - XML output of Tuning Definition
		SCL,		// Scala
		KBM,		// Keyboard Mapping File
		TUN,		// Anamark Tun 2.0
		MNLGTUNS,	// Korg 'logue Scale Tuning
		MNLGTUNO	// Korg 'logue Octave Tuning
	};

public:

	TuningFileParser() {};
	TuningFileParser(const String& absoluteFilePath);
	~TuningFileParser();

	void loadNewFile(const String& absoluteFilePath);

	void refreshTuning();

	String getFilePath() const;

	ValueTree getTuningDefinition() const;

	const Tuning& getTuning() const;

	TuningType getTuningType() const;

public:
	
	static int determineTuningType(const File& tuningFileIn);

	static ValueTree parseScalaFileDefinition(const File& scalaFile);

	static ValueTree parseTunFileDefinition(const File& tunFile);

private:

	void parseTuning();

private:

	String filePath;
	File fileLoaded;
	TuningType type = TuningType::INV;
	
	ValueTree tuningLoadedDefinition;
	std::unique_ptr<Tuning> tuningLoaded;
};