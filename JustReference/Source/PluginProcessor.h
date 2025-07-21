/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SoloFilterProcessing.h"
#include "LoopingZoneProcessor.h"
#include "SlotsProcessor.h"

//==============================================================================
/**
*/

enum class TransportState
{
    Playing,
    Paused,
    Stopped
};

enum class SlotSelected
{
    Slot1,
    Slot2,
    Slot3,
    Slot4
};

class AudioPlayerAudioProcessor  : public juce::AudioProcessor, private juce::ChangeListener
{
public:
    //==============================================================================
    AudioPlayerAudioProcessor();
    ~AudioPlayerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile(const juce::File& file);
    
    juce::AudioTransportSource transportSource;

    TransportState getTransportState() const { return transportState; }
    void setTransportState(TransportState newState) { transportState = newState; }

	bool isFileLoaded() const { return readerSource != nullptr; }

	juce::File getCurrentFile() const { return currentFile; }

	std::atomic<bool> isReferenceActive{ false };

    std::shared_ptr<const juce::String> getFileName();

    juce::AudioFormatManager formatManager;

    juce::AudioThumbnail& getAudioThumbnail() { return audioThumbnail; }
	juce::AudioThumbnailCache& getThumbnailCache() { return thumbnailCache; }

	LoopingZoneProcessor& getLoopingZoneProcessor() { return loopingZoneProcessor; }

    SoloFilterProcessing soloFilterProcessing{ 60.0f, 500.0f, 3000.0f, 10000.0f };

    SlotSelected& getSlotSelected() { return currentSlot; }

    juce::AudioProcessorValueTreeState& getTreeState() { return apvts; }

    SlotProcessor* getSlotProcessor(int index);
private:

    juce::AudioProcessorValueTreeState apvts;

	juce::AudioThumbnailCache thumbnailCache{ 10 };
	juce::AudioThumbnail audioThumbnail{ 256, formatManager, thumbnailCache };

    LoopingZoneProcessor loopingZoneProcessor;

    void setFileName(const juce::String newFilename);
    std::shared_ptr<const juce::String> filename{ std::make_shared<juce::String>("No song loaded...") };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerAudioProcessor)

    TransportState transportState = TransportState::Stopped;

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    juce::File currentFile;

    SlotSelected currentSlot = SlotSelected::Slot1;

    SlotProcessor slotProcessor1 { *this };
    SlotProcessor slotProcessor2 { *this };
    SlotProcessor slotProcessor3 { *this };
    SlotProcessor slotProcessor4 { *this };

    // Inherited via ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
};
