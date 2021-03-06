/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TuneupMidiAudioProcessor::TuneupMidiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	pluginState.reset(new TuneUpMidiState());
	midiProcessor = pluginState->getMidiProcessor();
}

TuneupMidiAudioProcessor::~TuneupMidiAudioProcessor()
{
}

//==============================================================================
const String TuneupMidiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TuneupMidiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TuneupMidiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TuneupMidiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TuneupMidiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TuneupMidiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TuneupMidiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TuneupMidiAudioProcessor::setCurrentProgram (int index)
{
}

const String TuneupMidiAudioProcessor::getProgramName (int index)
{
    return {};
}

void TuneupMidiAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TuneupMidiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	midiProcessor->resetNotes();
}

void TuneupMidiAudioProcessor::releaseResources()
{
	midiProcessor->resetNotes();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TuneupMidiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TuneupMidiAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	midiProcessor->processMidi(midiMessages);
}

//==============================================================================
bool TuneupMidiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TuneupMidiAudioProcessor::createEditor()
{
    return new TuneupMidiAudioProcessorEditor (*this, *midiProcessor, *pluginState.get());
}

//==============================================================================

TuneUpMidiState* TuneupMidiAudioProcessor::getPluginState()
{
	return pluginState.get();
}

//==============================================================================
void TuneupMidiAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	MemoryOutputStream mos;

	ValueTree stateNode = pluginState->getPluginStateNode();

	//stateNode = ValueTree(); // uncomment to wipe saved state

	DBG("Saved this node to memory:\n" + stateNode.toXmlString());

	stateNode.writeToStream(mos);
	destData.replaceWith(mos.getData(), mos.getDataSize());
}

void TuneupMidiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	MemoryInputStream mis(data, sizeInBytes, false);
	ValueTree stateNode = ValueTree::readFromStream(mis);

	DBG("Found this in memory:\n" + stateNode.toXmlString());

	//stateNode = ValueTree(); // uncomment to test default load

	pluginState->setPluginStateNode(stateNode);

	if	(stateNode.hasType(TuneUpIDs::tuneUpMidiSessionId) 
		&& stateNode.getNumProperties() > 0 
		&& stateNode.getNumChildren() > 0
		)
		pluginState->resetToPluginState(false);
	else
		pluginState->resetToDefaultSession(false);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TuneupMidiAudioProcessor();
}
