/*
  ==============================================================================

    DynamicOptionsWindow.cpp
    Created: 13 Jul 2020 3:04:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DynamicOptionsWindow.h"

DynamicOptionsWindow::DynamicOptionsWindow(ValueTree sessionNodeIn)
{
	sessionNode = sessionNodeIn;
	tuningOutDefinition = sessionNode.getChildWithName(TuneUpIDs::tuningsListId).getChild(1);

	dynamicTable.reset(new DynamicTuningTable(tuningOutDefinition));
	addAndMakeVisible(dynamicTable.get());

}

DynamicOptionsWindow::~DynamicOptionsWindow()
{
	dynamicTable = nullptr;
}


void DynamicOptionsWindow::paint(Graphics& g)
{

}

void DynamicOptionsWindow::resized()
{
	dynamicTable->setBoundsInset(BorderSize<int>(0));
}

void DynamicOptionsWindow::visibilityChanged()
{
	if (isVisible())
		dynamicTable->updateContent();
}

void DynamicOptionsWindow::loadOptionsNode(ValueTree sessionNodeIn)
{
	sessionNode = sessionNodeIn;
	tuningOutDefinition = sessionNode.getChild(0).getChild(1);
}

void DynamicOptionsWindow::updateContent()
{
	dynamicTable->updateContent();
}