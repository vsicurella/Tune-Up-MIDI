/*
  ==============================================================================

    DynamicOptionsWindow.h
    Created: 13 Jul 2020 3:04:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "CommonUI.h"
#include "DynamicTuningTable.h"

class DynamicOptionsWindow : public Component
{
public:

	DynamicOptionsWindow(ValueTree sessionOptionsNodeIn);
	~DynamicOptionsWindow();


	void paint(Graphics& g) override;

	void resized() override;

	void visibilityChanged() override;

	//================================================================================

	void updateContent();


private:

	Font font;

	ValueTree sessionOptionsNode;
	ValueTree tuningOutDefinition;
	
	std::unique_ptr<DynamicTuningTable> dynamicTable;

};