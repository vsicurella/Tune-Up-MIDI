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
	static Identifier tuneUpMidiSessionId("TuneUpMidiSession");
	static Identifier tuningsListId("TuningsList");

	// Options
	static Identifier defaultTuningFilePathId("DefaultTuningPath");
	static Identifier defaultTuningsListId("DefaultTunings");
	static Identifier referenceNoteInId("ReferenceNoteIn");
	static Identifier referenceFreqInId("ReferenceFreqIn");
	static Identifier referenceNoteOutId("ReferenceNoteOut");
	static Identifier referenceFreqOutId("ReferenceFreqOut");
	static Identifier pitchbendRangeId("PitchbendRange");
	static Identifier channelModeId("ChannelMode");
	static Identifier voiceLimitId("VoiceLimit");
	static Identifier reuseChannelsId("ReuseChannels");
	static Identifier resetChannelPitchbendId("ResetChannelPitchbend");
	static Identifier dynamicTuningModeId("DynamicTuningMode");

	// Channels
	static Identifier channelPropertiesNodeId("ChannelProperties");
	static Identifier channelLayout("ChannelComponentLayout");
	static Identifier channelOrientation("ChannelComponentOrientation");
	static Identifier channelControllerBackgroundColour("ChannelControllerBackgroundColour");
	static Identifier channelUsedColourId("ChannelUsedColour");
	static Identifier channelUnusedColourId("ChannelUnusedColour");
	static Identifier channelOnColourId("ChannelOnColour");
	static Identifier channelControllerTextColourId("ChannelControllerTextColour");
	static Identifier channelNodeId("ChannelNode");
	static Identifier channelNumberId("ChannelNumber");
	static Identifier channelUsedId("ChannelUsed");

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

	// Dynamic Tuning
	static Identifier dynamicTuningId("TuningIsDynamic");
	static Identifier dynamicGenMinId("DynamicGenMin");
	static Identifier dynamicGenMaxId("DynamicGenMax");
	static Identifier dynamicGenCenterId("DynamicCenterValue");
	static Identifier dynamicGenRangeId("DynamicRangeId");
	static Identifier dynamicGenCCNumId("DynamicCCNumber");

	// TUNE-UP MAIN OPTIONS
	static Array<Identifier> tuneUpOptionIds =
	{
		defaultTuningFilePathId,
		defaultTuningsListId,
		tuningsListId,
		referenceNoteInId,
		referenceFreqInId,
		referenceNoteOutId,
		referenceFreqOutId,
		pitchbendRangeId,
		channelModeId,
		voiceLimitId,
		reuseChannelsId,
		resetChannelPitchbendId
	};

	// TUNE-UP PARAMETERS
	static Array<Identifier> tuneUpParameterIds =
	{
		// TODO
	};

	// CHANNEL CONTROLLER COLOURS
	static Array<Identifier> channelControllerColours =
	{
		channelControllerBackgroundColour,
		channelUsedColourId,
		channelUnusedColourId,
		channelOnColourId,
		channelControllerTextColourId
	};
};