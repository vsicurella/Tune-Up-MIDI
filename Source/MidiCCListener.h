/*
  ==============================================================================

    MidiCCListener.h
    Created: 28 Jun 2020 1:36:40pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MidiCCListener
{

protected:

	std::map<int, std::function<void(int)>> controlChangeMap;

public:

	MidiCCListener() {};
	virtual ~MidiCCListener() {};

	virtual void controlValueChanged(int controlNumber, int controlValue) {};
};

class MidiCCNotifier
{
protected:

	ListenerList <MidiCCListener> midiControlListeners;

public:

	MidiCCNotifier() {};

	virtual ~MidiCCNotifier() 
	{
		midiControlListeners.clear();
	};

	void addControlListener(MidiCCListener* listenerIn)
	{
		midiControlListeners.add(listenerIn);
	}

	void removeControlListener(MidiCCListener* listenerIn)
	{
		midiControlListeners.remove(listenerIn);
	}

	void sendControlMessage(const MidiMessage& msg)
	{
		if (msg.isController())
		{
			midiControlListeners.call(&MidiCCListener::controlValueChanged, msg.getControllerNumber(), msg.getControllerValue());
		}
	}
};