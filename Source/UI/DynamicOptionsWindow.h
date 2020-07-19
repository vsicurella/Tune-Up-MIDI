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

	DynamicOptionsWindow(ValueTree sessionNodeIn);
	~DynamicOptionsWindow();


	void paint(Graphics& g) override;

	void resized() override;

	void visibilityChanged() override;

	//================================================================================

	void loadOptionsNode(ValueTree sessionNodeIn);

	void updateContent();


private:

	Font font;

	ValueTree sessionNode;
	ValueTree tuningOutDefinition;
	
	std::unique_ptr<DynamicTuningTable> dynamicTable;

};