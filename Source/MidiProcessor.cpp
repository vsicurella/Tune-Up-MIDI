/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 21 May 2019 6:37:57pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#include "MidiProcessor.h"

MidiProcessor::MidiProcessor(AudioProcessorValueTreeState& svkTreeIn)
    : svkTree(svkTreeIn)
{
    midiSettingsNode = ValueTree(IDs::midiSettingsNode);
    midiMapNode = ValueTree(IDs::midiMapNode);
    
    midiInputFilter.reset(new MidiFilter());
    midiInputRemap.reset(new MidiFilter());
    midiOutputFilter.reset(new MidiFilter());

    midiInput = nullptr;
    midiOutput = nullptr;
    
    originalKeyboardState.reset(new MidiKeyboardState());
    remappedKeyboardState.reset(new MidiKeyboardState());
        
	retuner.reset(new MidiNoteTuner());
    retuner->setPitchbendMax(48);
    
    mpeInst.reset(new MPEInstrument());
    channelAssigner.reset(new TuneUpMidiChannelAssigner(mpeInst.get()));
    channelAssigner->setIgnorePitchbend(true); // temporary

    // default sample rate
    reset(41000);
    
    mpeZone.setLowerZone(maxNumVoices, pitchBendNoteMax, pitchBendGlobalMax);
    mpeInst->setZoneLayout(mpeZone);
    updateMPEMode();
}

MidiProcessor::~MidiProcessor()
{
    
}

void MidiProcessor::connectToParameters()
{
	Array<Identifier> params(
		{
			IDs::periodShift,
			IDs::periodShiftModeSize,
			IDs::transposeAmt,
			IDs::keyboardMidiChannel,
			IDs::mpeOn,
			IDs::mappingMode
		});

	for (auto param : params)
	{
		svkTree.addParameterListener(param, this);
	}
	DBG("MidiProcessor connected to parameters");
}

void MidiProcessor::updateNode()
{
    //midiSettingsNode.setProperty(IDs::mpeThru, mpeThru, nullptr);
    midiSettingsNode.setProperty(IDs::mpeZone, 0 /*do something here*/, nullptr);
    midiSettingsNode.setProperty(IDs::mpeLegacyOn, mpeInst->isLegacyModeEnabled(), nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendNoteMax, pitchBendNoteMax, nullptr);
    midiSettingsNode.setProperty(IDs::pitchBendGlobalMax, mpeInst->getLegacyModePitchbendRange(), nullptr);
    midiSettingsNode.setProperty(IDs::mpePitchTrackingMode, mpePitchbendTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpePressureTrackingMode, mpePressureTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpeTimbreTrackingMode, mpeTimbreTrackingMode, nullptr);
    midiSettingsNode.setProperty(IDs::mpeTuningPreserveMidiNote, mpeTuningPreserveMidiNote, nullptr);
    
    Array<int> inputFilter = midiInputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputFilter), nullptr);
    add_array_to_node(midiMapNode, inputFilter, IDs::midiInputFilter, "Note");
    
    Array<int> inputRemap = midiInputRemap->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
    add_array_to_node(midiMapNode, inputRemap, IDs::midiInputRemap, "Note");
    
    Array<int> outputFilter = midiOutputFilter->getRemappedNotes();
    midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputFilter), nullptr);
    add_array_to_node(midiMapNode, outputFilter, IDs::midiOutputFilter, "Note");
}

bool MidiProcessor::restoreFromNode(ValueTree midiSettingsNodeIn)
{
    if (midiSettingsNodeIn.hasType(IDs::midiSettingsNode))
    {
        midiSettingsNode.copyPropertiesAndChildrenFrom(midiSettingsNodeIn, nullptr);
        
		// Set Note Maps
		if (!midiSettingsNode.getChildWithName(IDs::midiMapNode).isValid())
		{
			midiMapNode = ValueTree(IDs::midiMapNode);
			midiSettingsNode.addChild(midiMapNode, -1, nullptr);
		}

		setMidiMaps(midiSettingsNode.getChildWithName(IDs::midiMapNode));

        return true;
    }
    
    return false;
}

//==============================================================================

StringArray MidiProcessor::getAvailableInputs() const
{
    return MidiInput::getDevices();
}

Array<MidiDeviceInfo> MidiProcessor::getAvailableOutputs() const
{
    return MidiOutput::getAvailableDevices();
}

MidiInput* MidiProcessor::getInputDevice()
{
    return midiInput.get();
}

MidiOutput*  MidiProcessor::getOutputDevice()
{
    return midiOutput.get();
}

String MidiProcessor::getInputName() const
{
    return midiInputName;
}

String MidiProcessor::getOutputName() const
{
    return midiOutputName;
}

MidiKeyboardState* MidiProcessor::getOriginalKeyboardState()
{
    return originalKeyboardState.get();
}


MidiKeyboardState* MidiProcessor::getRemappedKeyboardState()
{
    return remappedKeyboardState.get();
}

int MidiProcessor::getPeriodShift() const
{
    return periodShift;
}

int MidiProcessor::getMidiChannelOut() const
{
    return midiChannelOut;
}

int MidiProcessor::getTransposeAmt() const
{
    return transposeAmt;
}

bool MidiProcessor::isMPEOn() const
{
    return mpeOn;
}

int MidiProcessor::getPitchbendNoteMax() const
{
    return pitchBendNoteMax;
}

int MidiProcessor::getPitchTrackingMode() const
{
    return mpePitchbendTrackingMode;
}

int MidiProcessor::getPressureTrackingMode() const
{
    return mpePressureTrackingMode;
}

int MidiProcessor::getTimbreTrackingMode() const
{
    return mpeTimbreTrackingMode;
}

int MidiProcessor::getVoiceLimit() const
{
    return maxNumVoices;
}

bool MidiProcessor::isRetuning() const
{
    return doRetuning;
}

int MidiProcessor::isTuningPreservesMidiNote() const
{
    return mpeTuningPreserveMidiNote;
}

NoteMap* MidiProcessor::getInputNoteRemap()
{
	return midiInputRemap->getNoteMap();
}

NoteMap* MidiProcessor::getOutputNoteFilter()
{
	return midiOutputFilter->getNoteMap();
}

MidiFilter* MidiProcessor::getInputRemapMidiFilter()
{
	return midiInputRemap.get();
}

MidiFilter* MidiProcessor::getOutputMidiFilter()
{
	return midiOutputFilter.get();
}

MPEInstrument* MidiProcessor::getMPEInstrument()
{
    return mpeInst.get();
}

int MidiProcessor::getInputNote(int midiNoteIn)
{
    return midiInputRemap->getNoteRemapped(midiNoteIn);
}

int MidiProcessor::getOutputNote(int midiNoteIn)
{
    return midiOutputFilter->getNoteRemapped(midiNoteIn);
}

//==============================================================================

String MidiProcessor::setMidiInput(String deviceID)
{
    //midiInput = MidiInput::openDevice(deviceID, this);
    midiInputName = midiInput->getName();
    if (midiInputName.isNotEmpty())
    {
        inputSelected = deviceID;
        midiSettingsNode.setProperty(IDs::midiInputName, midiInput->getIdentifier(), nullptr);
    }
    else
    {
        inputSelected = "";
        midiInput = nullptr;
    }
    
    return midiInputName;
}

String MidiProcessor::setMidiOutput(String deviceID)
{
    midiOutput = MidiOutput::openDevice(deviceID);
    
    if (midiOutput.get())
    {
        midiOutputName = midiOutput->getName();
        outputSelected = deviceID;
        midiSettingsNode.setProperty(IDs::midiOutputName, midiOutput->getIdentifier(), nullptr);
    }
    else
    {
        midiOutputName = "";
        outputSelected = "";
        midiOutput = nullptr;
    }
    
    return midiOutputName;
}

void MidiProcessor::setModeViewed(Mode* modeViewedIn)
{
	modeViewed = modeViewedIn;
}

void MidiProcessor::setMode1(Mode* mode1In)
{
	mode1 = mode1In;
}

void MidiProcessor::setMode2(Mode* mode2In)
{
	mode2 = mode2In;
}

void MidiProcessor::setTuning(Tuning* tuningIn)
{
	tuning = tuningIn;
	retuner->setNewTuning(tuning);
}

void MidiProcessor::updateMPEMode()
{
    if (isMPEOn())
    {
        mpeInst->setZoneLayout(mpeZone);
        channelAssigner->allNotesOff();
        // TODO: actually turn notes off
    }
    else
    {
        mpeInst->setLegacyModeChannelRange(Range<int>(1, 2));
        channelAssigner->allNotesOff();
    }
}

void MidiProcessor::setPitchBendNoteMax(int bendAmtIn)
{
    pitchBendNoteMax = bendAmtIn;
    retuner->setPitchbendMax(pitchBendNoteMax);
}

void MidiProcessor::setPitchBendGlobalMax(int bendAmtIn)
{
    pitchBendGlobalMax = bendAmtIn;
    //mpeInst->setLegacyModePitchbendRange(bendAmtIn);
}

void MidiProcessor::setTuningPreservesMidiNote(bool preserveMidiNote)
{
    mpeTuningPreserveMidiNote = preserveMidiNote;
}

void MidiProcessor::setPitchbendTrackingMode(int modeIn)
{
    mpePitchbendTrackingMode = modeIn;
    mpeInst->setPitchbendTrackingMode(MPEInstrument::TrackingMode(mpePitchbendTrackingMode));
}

void MidiProcessor::setPressureTrackingMode(int modeIn)
{
    mpePressureTrackingMode = modeIn;
    mpeInst->setPressureTrackingMode(MPEInstrument::TrackingMode(mpePressureTrackingMode));
}

void MidiProcessor::setTimbreTrackingMode(int modeIn)
{
    mpeTimbreTrackingMode = modeIn;
    mpeInst->setTimbreTrackingMode(MPEInstrument::TrackingMode(mpeTimbreTrackingMode));
}

void MidiProcessor::setVoiceLimit(int maxVoicesIn)
{
    maxNumVoices = maxVoicesIn;
    mpeZone.setLowerZone(maxVoicesIn, pitchBendNoteMax, pitchBendGlobalMax);
    mpeInst->setZoneLayout(mpeZone);
}

void MidiProcessor::setRetuneOn(bool retuneOn)
{
    doRetuning = retuneOn;
}

void MidiProcessor::setInputToFilter(bool doRemap)
{
    isInputFiltered = doRemap;
}

void MidiProcessor::setInputToRemap(bool doRemap)
{
    isInputRemapped = doRemap;
}

void MidiProcessor::setOutputToFilter(bool doFilter)
{
    isOutputFiltered = doFilter;
}

void MidiProcessor::setMidiMaps(ValueTree midiMapIn)
{
    if (midiMapIn.hasType(IDs::midiMapNode))
    {
        Array<int> map;
        
        get_array_from_node(midiMapIn, map, IDs::midiInputFilter);
        
        if (map.size() > 0)
            setInputFilter(map);
        else
            resetInputFilter();
        
        map.clear();
        get_array_from_node(midiMapIn, map, IDs::midiInputRemap);
        
        if (map.size() > 0)
            setInputRemap(map);
        else
            resetInputMap();
        
        map.clear();
        get_array_from_node(midiMapIn, map, IDs::midiOutputFilter);
        
        if (map.size() > 0)
            setOutputFilter(map);
        else
            resetOutputFilter();
        
        midiMapNode.copyPropertiesAndChildrenFrom(midiMapIn, nullptr);
    }
}

void MidiProcessor::setInputFilter(Array<int> mapIn, bool updateNode)
{
    setInputFilter(NoteMap(mapIn));
}

void MidiProcessor::setInputFilter(NoteMap mapIn, bool updateNode)
{
    midiInputFilter->setNoteMap(mapIn);

    if (updateNode)
    {
        midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputFilter), nullptr);
        add_array_to_node(midiMapNode, midiInputFilter->getNoteMap()->getValues(), IDs::midiInputFilter, "Note");
    }
}

void MidiProcessor::setInputRemap(Array<int> mapIn, bool updateNode)
{
    setInputRemap(NoteMap(mapIn));
}

void MidiProcessor::setInputRemap(NoteMap mapIn, bool updateNode)
{
	midiInputRemap->setNoteMap(mapIn);

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiInputRemap), nullptr);
		add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiInputRemap, "Note");
	}
}

void MidiProcessor::setOutputFilter(Array<int> mapIn, bool updateNode)
{
    setOutputFilter(NoteMap(mapIn));
}

void MidiProcessor::setOutputFilter(NoteMap mapIn, bool updateNode)
{
	midiOutputFilter->setNoteMap(mapIn.getValues());

	if (updateNode)
	{
		midiMapNode.removeChild(midiMapNode.getChildWithName(IDs::midiOutputFilter), nullptr);
		add_array_to_node(midiMapNode, midiInputRemap->getNoteMap()->getValues(), IDs::midiOutputFilter, "Note");
	}
}

void MidiProcessor::resetInputFilter(bool updateNode)
{
    setInputFilter(NoteMap(), updateNode);
}

void MidiProcessor::resetInputMap(bool updateNode)
{
    setInputRemap(NoteMap(), updateNode);
}

void MidiProcessor::resetOutputFilter(bool updateNode)
{
    setOutputFilter(NoteMap(), updateNode);
}

void MidiProcessor::mapNoteForInputFilter(int noteIn, int noteOut, bool updateNode)
{
    midiInputFilter->setNote(noteIn, noteOut);
    
    if (updateNode)
        set_value_in_array(midiMapNode, IDs::midiInputFilter, noteIn, noteOut);
}

void MidiProcessor::mapNoteForInputRemap(int noteIn, int noteOut, bool updateNode)
{
    midiInputRemap->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiInputRemap, noteIn, noteOut);
}

void MidiProcessor::mapNoteForOutputFilter(int noteIn, int noteOut, bool updateNode)
{
    midiOutputFilter->setNote(noteIn, noteOut);

	if (updateNode)
		set_value_in_array(midiMapNode, IDs::midiOutputFilter, noteIn, noteOut);
}

//==============================================================================

void MidiProcessor::processMidi(MidiBuffer &midiMessages)
{
    // TODO: handle note offs if period/transpose is changed before note offs
    
    // Process external input
    auto inputEvents = MidiBuffer::Iterator(midiMessages);
    MidiMessage msg;
    int smpl;
    int midiNote;
    msgCount = 0;
    
    while (inputEvents.getNextEvent(msg, smpl))
    {
        if (isInputFiltered)
        {
            midiNote = midiInputFilter->getNoteRemapped(msg.getNoteNumber());
            msg.setNoteNumber(midiNote);
        }
        
        // check for out of range notes?
        originalKeyboardState->processNextMidiEvent(msg);
    
        if (isInputRemapped)
        {
            midiNote = midiInputRemap->getNoteRemapped(msg.getNoteNumber());
			msg.setNoteNumber(midiNote);
        }
        
        remappedKeyboardState->processNextMidiEvent(msg);
        msg.setTimeStamp((double)smpl + ++msgCount);
        addMessageToQueue(msg);
    }
    
    MidiBuffer preBuffer;
    removeNextBlockOfMessages(preBuffer, 4096);
    
    auto allMidiEvents = MidiBuffer::Iterator(preBuffer);
    msgCount = 0;
    
    // Output Filtering
    while (allMidiEvents.getNextEvent(msg, smpl))
    {
        // Process transpositions
        
        midiNote = msg.getNoteNumber() + transposeAmt;
        midiNote += (periodShift * mode2->getScaleSize());
        msg.setNoteNumber(midiNote);
        
        if (isOutputFiltered)
        {
            midiNote = midiOutputFilter->getNoteRemapped(msg.getNoteNumber());
            msg.setNoteNumber(midiNote);
        }
    }
    
    if (isMPEOn())
    {
        MidiBuffer mpeBuffer;
        mpeBuffer = convertToMPE(preBuffer);
        preBuffer = mpeBuffer;
    }
    
    midiMessages = preBuffer;
    sendBufferToOutputs(midiMessages);
    msgCount = 0;
}

MidiBuffer MidiProcessor::convertToMPE(const MidiBuffer& bufferIn)
{
    MidiBuffer mpeOut;

    auto midiEvents = MidiBuffer::Iterator(bufferIn);
    MidiMessage msgIn, msgOut, pitchBendOut;
    int smpl;
    int mpeMsgCount = 0;

    MPENote mpeNote;
    MidiPitch midiPitch(-1, 0);
    int pitchBend;
    int newChannel;

    while (midiEvents.getNextEvent(msgIn, smpl))
    {
        if (msgIn.isNoteOff())
        {
            if (mpeTuningPreserveMidiNote || tuning == nullptr)
            {
                midiPitch = MidiPitch(msgIn.getNoteNumber(), 8192);
            }
            else
            {
                midiPitch = retuner->closestNoteForSemitone(tuning->getNoteInSemitones(msgIn.getNoteNumber()));
            }
            
            newChannel = channelAssigner->noteOff(midiPitch);
            
            msgOut = MidiMessage::noteOff(newChannel, midiPitch.key, (uint8) msgIn.getVelocity());
            
            mpeOut.addEvent(MidiMessage::pitchWheel(newChannel, midiPitch.value), smpl + mpeMsgCount);
            mpeOut.addEvent(msgOut, smpl + ++mpeMsgCount);
        }
        
        else if (msgIn.isNoteOn())
        {
            
			if (tuning == nullptr)
			{
				pitchBend = 8192;
				midiPitch = MidiPitch(msgIn.getNoteNumber(), pitchBend);
			}
            if (mpeTuningPreserveMidiNote)
            {
                pitchBend = retuner->pitchbendFromNote(msgIn.getNoteNumber());
                midiPitch = MidiPitch(msgIn.getNoteNumber(), pitchBend);
            }
            else
            {
                midiPitch = retuner->closestNoteForSemitone(tuning->getNoteInSemitones(msgIn.getNoteNumber()));
            }

            newChannel = channelAssigner->findMidiChannelForNote(midiPitch);
            
            msgOut = MidiMessage::noteOn(newChannel, midiPitch.key, (uint8) msgIn.getVelocity());
            pitchBendOut = MidiMessage::pitchWheel(newChannel, midiPitch.value);
            
            mpeOut.addEvent(pitchBendOut, smpl + mpeMsgCount);
            mpeOut.addEvent(msgOut, smpl + ++mpeMsgCount);
        }
    }

    return mpeOut;
}

void MidiProcessor::sendMsgToOutputs(const MidiMessage& msg)
{
    if (midiOutput)
        midiOutput->sendMessageNow(msg);
}

void MidiProcessor::sendBufferToOutputs(const MidiBuffer& bufferToSend)
{
    if (midiOutput)
    {
        midiOutput->sendBlockOfMessagesNow(bufferToSend);
    }
}

void MidiProcessor::allNotesOff()
{
    for (int i = 1; i <= 16; i++)
    {
        addMessageToQueue(MidiMessage::allNotesOff(i));
    }
}

void MidiProcessor::allNotesOff(int channelNumber)
{
    addMessageToQueue(MidiMessage::allNotesOff(channelNumber));
}

//==============================================================================


void MidiProcessor::pauseMidiInput(bool setPaused)
{
    midiInputPaused = setPaused;
}

bool MidiProcessor::isMidiPaused()
{
    return midiInputPaused;
}

//==============================================================================

void MidiProcessor::parameterChanged(const String& paramID, float newValue)
{
	if (paramID == IDs::mappingMode.toString())
	{
		isInputRemapped = newValue > 1;
	}
    else if (paramID == IDs::periodShift.toString())
    {
        periodShift = newValue;
    }
    else if (paramID == IDs::periodShiftModeSize.toString())
    {
        periodShiftModeSIze = newValue;
    }
    else if (paramID == IDs::transposeAmt.toString())
    {
        transposeAmt = newValue;
    }
    else if (paramID == IDs::keyboardMidiChannel.toString())
    {
        //allNotesOff(midiChannelOut);
        midiChannelOut = newValue;
    }
    else if (paramID == IDs::mpeOn.toString())
    {
        mpeOn = newValue;
    }
    
    DBG("Midi processor updated");
}

void MidiProcessor::handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void MidiProcessor::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    MidiMessage msg = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
    msg.setTimeStamp(++msgCount);
    addMessageToQueue(msg);
}

void MidiProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& msg)
{
    
    MidiMessage myMsg = MidiMessage(msg);
    ++msgCount;
    addMessageToQueue(myMsg);
}
//
//
//void SvkMidiProcessor::noteAdded(MPENote newNote)
//{
//    MidiMessage msgTuned = MidiMessage::noteOn(newNote.midiChannel, newNote.initialNote, (uint8) newNote.noteOnVelocity.as7BitInt());
//
//    MidiMessage pitchbend = MidiMessage::pitchWheel(newNote.midiChannel, tuning->getPitchBendAtMidiNote(newNote.initialNote));
//    addMessageToQueue(pitchbend);
//    addMessageToQueue(msgTuned);
//}
//
//void SvkMidiProcessor::noteReleased (MPENote finishedNote)
//{
//    MidiMessage msgOff = MidiMessage::noteOff(finishedNote.midiChannel, finishedNote.initialNote, (uint8) finishedNote.noteOffVelocity.as7BitInt());
//    addMessageToQueue(msgOff);
//}
