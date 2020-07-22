/*
  ==============================================================================

    ChannelConfigureComponent.h
    Created: 20 Jul 2020 6:09:41pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CommonUI.h"


//==============================================================================
/*
	A grid representing MIDI channels with mouse interaction to change channel properties.
*/
class ChannelConfigureComponent  : public juce::Component
{
public:

	enum ColourIds
	{
		backgroundColourId = 0x0100100,
		channelUsedColourId = 0x100200,
		channelUnusedColourId = 0x100201,
		channelOffColourId = 0x0100210,
		channelOnColourId = 0x0100211,
		labelTextColourId = 0x0100300
	};

	enum Layout
	{
		Line = 0,
		Rectangle,
		Square
	};

	enum Orientation
	{
		Horizontal = 0,
		Vertical
	};

public:
    ChannelConfigureComponent();
    ~ChannelConfigureComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void loadChannelPropertiesNode(ValueTree channelPropertiesIn);

	void setLabelWidth(int widthIn);

	void setLayout(ChannelConfigureComponent::Layout layoutIn);

	void setOrientation(ChannelConfigureComponent::Orientation orientationIn);

	void updateChannelRectangles();

	void updateChannelColours();

private:

	ValueTree channelPropertiesNode;

	ChannelConfigureComponent::Layout layout = ChannelConfigureComponent::Layout::Rectangle;
	ChannelConfigureComponent::Orientation orientation = ChannelConfigureComponent::Orientation::Horizontal;

	int targetLabelWidth;
	int labelWidth;
	int squareWidth;

	int sizeFactor;
	int otherSizeFactor;
	Point<int> format;

	Array<juce::Rectangle<float>> channelRectangles;
	Array<Colour> channelPropertyColours;
	Array<int> channelsLabelled;

	int lastChannelMouseClicked = 0;
	int mouseOverChannel = 0;
	Point<double> mousePosition;

	Array<int> appToClassColourIds =
	{
		ChannelConfigureComponent::ColourIds::backgroundColourId,
		ChannelConfigureComponent::ColourIds::channelUsedColourId,
		ChannelConfigureComponent::ColourIds::channelUnusedColourId,
		ChannelConfigureComponent::ColourIds::channelOffColourId,
		ChannelConfigureComponent::ColourIds::channelOnColourId,
		ChannelConfigureComponent::ColourIds::labelTextColourId
	};

	//==============================================================================

	int findChannelMouseOver(const MouseEvent& eventIn);

	void updateLayoutFormat();


	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelConfigureComponent)
};
