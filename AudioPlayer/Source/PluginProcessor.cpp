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
                       )
#endif
{
    formatManager.registerBasicFormats();

    audioThumbnail.addChangeListener(this);
}

AudioPlayerAudioProcessor::~AudioPlayerAudioProcessor()
{
    transportSource.setSource(nullptr);
	readerSource.reset();
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
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
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
    if (isReferenceActive)
    {
		// REFERENCE: Process the audio block with the transport source
        buffer.clear();
        transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
    }
    else {
		// DAW: Process the audio regularly
        juce::AudioBuffer<float> tempBuffer(buffer.getNumChannels(), buffer.getNumSamples());
        transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(tempBuffer));
    }
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPlayerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void AudioPlayerAudioProcessor::loadFile(const juce::File& file)
{
	currentFile = file;
    auto* reader = formatManager.createReaderFor(file);
    if (reader != nullptr)
    {
        transportSource.setSource(nullptr);
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        audioThumbnail.setSource(new juce::FileInputSource(file));
        setFileName(file.getFileNameWithoutExtension());
    }
}

void AudioPlayerAudioProcessor::setFileName(const juce::String newFilename)
{
    std::shared_ptr<const juce::String> newPtr = std::make_shared<juce::String>(newFilename);

	std::atomic_store_explicit(&filename, newPtr, std::memory_order_release);
}

void AudioPlayerAudioProcessor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &audioThumbnail)
    {
        // Handle thumbnail changes if necessary
	}
}

std::shared_ptr<const juce::String> AudioPlayerAudioProcessor::getFileName()
{
	return std::atomic_load_explicit(&filename, std::memory_order_acquire);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlayerAudioProcessor();
}
