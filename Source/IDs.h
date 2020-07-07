/*
  ==============================================================================

    IDs.h
    Created: 6 Jul 2020 11:23:02am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace TuneUpIDs
{
	// Parents
	static Identifier tuneUpMidiStateId("TuneUpMidiState");
	static Identifier tuningsInAndOutNodeId("TuningsInAndOut");

	// Options
	static Identifier defaultOptionsNodeId("DefaultOptions");
	static Identifier sessionOptionsNodeId("SessionOptions");
	static Identifier defaultTuningFilePathId("DefaultTuningPath");
	static Identifier defaultTuningsListId("DefaultTuningsList");
	static Identifier referenceNoteInId("ReferenceNoteIn");
	static Identifier referenceFreqInId("ReferenceFreqIn");
	static Identifier referenceNoteOutId("ReferenceNoteOut");
	static Identifier referenceFreqOutId("ReferenceFreqOut");
	static Identifier pitchbendRangeId("PitchbendRange");
	static Identifier channelConfigurationId("ChannelConfiguration");
	static Identifier channelModeId("ChannelMode");
	static Identifier voiceLimitId("VoiceLimit");
	static Identifier reuseChannelsId("ReuseChannels");
	static Identifier resetChannelPitchbendId("ResetChannelPitchbend");
	static Identifier dynamicTuningModeId("DynamicTuningMode");

	// Control Values
	static Identifier ccListId("CCList");
	static Identifier ccNodeId("CCNode");
	static Identifier ccNumberId("CCNumber");
	static Identifier ccDestinationId("CCDestination");
	static Identifier ccRangeId("CCRange");
	static Identifier ccValueId("CCValue");
	static Identifier ccValueDefaultId("CCDefaultValue");

	// Tuning Definition

	static Identifier tuningDefinitionId("TuningDefinition");
	static Identifier functionalId("TuningIsFunctional");
	static Identifier tuningSizeId("TuningSize");
	static Identifier rootMidiNoteId("RootNote");
	static Identifier generatorListId("GeneratorList");
	static Identifier generatorNodeId("GeneratorNode");
	static Identifier generatorOffsetId("GeneratorOffset");
	static Identifier generatorAmountId("GeneratorAmount");
	static Identifier generatorValueId("GeneratorValue");
	static Identifier centsTableId("CentsTable");
	static Identifier intervalNodeId("IntervalDesc");
	static Identifier intervalValueId("IntervalValue");
	static Identifier tuningNameId("Name");
	static Identifier tuningDescriptionId("Description");

};