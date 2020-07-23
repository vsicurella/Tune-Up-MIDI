/*
  ==============================================================================

    ChannelConfigureComponent.cpp
    Created: 20 Jul 2020 6:09:41pm
    Author:  Vincenzo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelConfigureComponent.h"

//==============================================================================
ChannelConfigureComponent::ChannelConfigureComponent()
{
	channelPropertyColours.resize(16);

	// Default colours
	setColour(ChannelConfigureComponent::ColourIds::backgroundColourId, Colour());
	setColour(ChannelConfigureComponent::ColourIds::channelUsedColourId, Colours::lightgoldenrodyellow);
	setColour(ChannelConfigureComponent::ColourIds::channelUnusedColourId, Colours::darkgrey);
	setColour(ChannelConfigureComponent::ColourIds::channelOnColourId, Colours::cadetblue);
	setColour(ChannelConfigureComponent::ColourIds::labelTextColourId, Colours::white);

	updateChannelColours();

	// Default layout
	setLayout(layout);
}

ChannelConfigureComponent::~ChannelConfigureComponent()
{
	removeAllChangeListeners();
}

void ChannelConfigureComponent::paint (juce::Graphics& g)
{
    g.fillAll(findColour(ChannelConfigureComponent::ColourIds::backgroundColourId));

	// Don't paint if channel properties haven't been loaded yet.
	if (channelPropertiesNode.isValid())
	{
		Colour channelColour;
		juce::Rectangle<float> chRect;
		for (int ch = 0; ch < 16; ch++)
		{
			chRect = channelRectangles[ch];
			channelColour = channelPropertyColours[ch];

			if (ch == mouseOverChannel)
			{
				channelColour = channelColour.contrasting(0.25f);
			}

			// TODO: check if channel has note (or do callback)

			g.setColour(channelColour);
			g.fillRect(chRect);

			g.setColour(Colours::black);
			g.drawRect(chRect);
		}

		g.setColour(findColour(ChannelConfigureComponent::ColourIds::labelTextColourId));

		for (int i = 0; i < channelsLabelled.size(); i++)
		{
			int translation = (i % 2 == 0) ? -1 : 1;
			g.drawFittedText(
				String(channelsLabelled[i] + 1),
				channelRectangles[channelsLabelled[i]]
					.translated(translation * labelWidth, 0)
					.withWidth(labelWidth)
					.toNearestInt(),
				juce::Justification::centred,
				1, 0.01f
			);
		}
	}

	g.setColour(Colours::black);
	g.drawRect(0.0f, 0.0f, (float)getWidth(), (float)getHeight(), 1.0f);
}

void ChannelConfigureComponent::resized()
{
	int squareWidth = round((getWidth() - labelWidth * 2) / (float)sizeFactor);

	// adjust size from rounding error
	labelWidth = (getWidth() - squareWidth * sizeFactor) / 2;
	channelControlWidth = getWidth() - labelWidth * 2;
	squareWidth = channelControlWidth / sizeFactor;

	channelSize.setXY(
		squareWidth,
		round(getHeight() / otherSizeFactor)
	);

	updateChannelRectangles();
}

void ChannelConfigureComponent::mouseMove(const MouseEvent& event)
{
	int channelNum = findChannelMouseOver(event);
	
	if (mouseOverChannel != channelNum)
	{
		mouseOverChannel = channelNum;
		repaint();
	}
}

void ChannelConfigureComponent::mouseDown(const MouseEvent& event)
{
	lastChannelMouseClicked = mouseOverChannel;

	ValueTree channelNode = channelPropertiesNode.getChild(lastChannelMouseClicked);
	channelNode.setProperty(TuneUpIDs::channelUsedId, !(bool)channelNode[TuneUpIDs::channelUsedId], nullptr);
	
	sendChangeMessage();

	updateChannelColours();
	repaint();

	// TODO - more options
}

void ChannelConfigureComponent::mouseExit(const MouseEvent& event)
{
	mouseOverChannel = -1;
	repaint();
}

void ChannelConfigureComponent::loadChannelPropertiesNode(ValueTree channelPropertiesIn)
{
	if (channelPropertiesIn.hasType(TuneUpIDs::channelPropertiesNodeId))
	{
		channelPropertiesNode = channelPropertiesIn;

		//for (int i = 0; i < channelControllerColours.size(); i++)
		//	setColour(
		//		appToClassColourIds[i], 
		//		Colour::fromString(channelPropertiesNode[channelControllerColours[i]].toString())
		//	);

		updateChannelColours();
	}
}

void ChannelConfigureComponent::setLabelWidth(int widthIn)
{
	labelWidth = widthIn;

	if (!getBounds().isEmpty())
		resized();
}

void ChannelConfigureComponent::setLayout(ChannelConfigureComponent::Layout layoutIn)
{
	layout = layoutIn;

	sizeFactor = 16 / pow(2, (int)layout);
	otherSizeFactor = 16 / sizeFactor;

	switch (layout)
	{
	case ChannelConfigureComponent::Layout::Line:
		channelsLabelled = { 0, 15 };
		break;

	case ChannelConfigureComponent::Layout::Square:
		channelsLabelled = { 0, 3, 4, 7, 8, 11, 14, 15 };
		break;

	default: // ChannelConfigureComponent::Layout::Rectangle
		channelsLabelled = { 0, 7, 8, 15 };
	}

	updateLayoutFormat();
}

void ChannelConfigureComponent::setOrientation(ChannelConfigureComponent::Orientation orientationIn)
{
	orientation = orientationIn;
	updateLayoutFormat();
}

int ChannelConfigureComponent::findChannelMouseOver(const MouseEvent& eventIn)
{
	int channelNum = -1;

	if (channelBounds.contains(eventIn.getPosition()))
	{
		channelNum = (eventIn.x - labelWidth) / channelSize.x + eventIn.y / channelSize.y * 8;
	}

	return channelNum;
}

void ChannelConfigureComponent::updateChannelRectangles()
{
	channelRectangles.clear();
	int channelNum = 0;
	for (int row = 0; row < format.y; row++)
	{
		for (int col = 0; col < format.x; col++)
		{
			Point<float> cornerTL = Point<float>(labelWidth + col * channelSize.x, row * channelSize.y);
			Point<float> cornerBR = Point<float>(cornerTL.x + channelSize.x, cornerTL.y + channelSize.y);
			channelRectangles.add(juce::Rectangle<float>(cornerTL, cornerBR));
		}
	}

	channelBounds = juce::Rectangle<int>(labelWidth, 0, channelControlWidth, getHeight());
}

void ChannelConfigureComponent::updateChannelColours()
{
	channelPropertyColours.clear();

	for (auto ch : channelPropertiesNode)
	{
		Colour c = ch[TuneUpIDs::channelUsedId]
			? findColour(ChannelConfigureComponent::ColourIds::channelUsedColourId)
			: findColour(ChannelConfigureComponent::ColourIds::channelUnusedColourId);

		channelPropertyColours.add(c);
	}
}

void ChannelConfigureComponent::updateLayoutFormat()
{
	format = orientation == 0
		? Point<int>(sizeFactor, otherSizeFactor) // Horizontal
		: Point<int>({ otherSizeFactor, sizeFactor }); // Vertical

	if (!getBounds().isEmpty())
		resized();
}

ValueTree ChannelConfigureComponent::getChannelPropertiesNode()
{
	return channelPropertiesNode;
}