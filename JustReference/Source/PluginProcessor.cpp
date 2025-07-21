/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerAudioProcessor::AudioPlayerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, juce::Identifier("apvtsJustReference"), 
                           {
                           })
#endif
{
    formatManager.registerBasicFormats();
}

AudioPlayerAudioProcessor::~AudioPlayerAudioProcessor()
{
    transportSource.setSource(nullptr);
}

//==============================================================================
const juce::String AudioPlayerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPlayerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPlayerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPlayerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPlayerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPlayerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioPlayerAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPlayerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioPlayerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    slotProcessor1.prepareToPlay(sampleRate, samplesPerBlock);
    slotProcessor2.prepareToPlay(sampleRate, samplesPerBlock);
    slotProcessor3.prepareToPlay(sampleRate, samplesPerBlock);
    slotProcessor4.prepareToPlay(sampleRate, samplesPerBlock);

	soloFilterProcessing.prepareToPlay(sampleRate, samplesPerBlock);
}

void AudioPlayerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPlayerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void AudioPlayerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (auto* playHead = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo position;
        if (playHead->getCurrentPosition(position) && position.isPlaying)
        {
	        switch (currentSlot)
	        {
	        case SlotSelected::Slot1:
                slotProcessor1.process(buffer);
                break;
	        case SlotSelected::Slot2:
                slotProcessor2.process(buffer);
                break;
	        case SlotSelected::Slot3:
                slotProcessor3.process(buffer);
                break;
	        case SlotSelected::Slot4:
                slotProcessor4.process(buffer);
                break;
	        }
        }
    }

    soloFilterProcessing.process(buffer, midiMessages);
}
//==============================================================================
bool AudioPlayerAudioProcessor::hasEditor() const
{   
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPlayerAudioProcessor::createEditor()
{
    return new AudioPlayerAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPlayerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    apvts.state.writeToStream(stream);
}

void AudioPlayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.state = tree;
    }
}

SlotProcessor* AudioPlayerAudioProcessor::getSlotProcessor(int index)
{
	switch (index)
	{
	case 1:
        return &slotProcessor1;
        break;
	case 2:
        return &slotProcessor2;
        break;
	case 3:
        return &slotProcessor3;
        break;
	case 4:
        return &slotProcessor4;
		break;
	default:
        return nullptr;
        break;

	}
}

void AudioPlayerAudioProcessor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlayerAudioProcessor();
}
