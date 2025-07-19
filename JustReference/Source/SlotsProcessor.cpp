/*
  ==============================================================================

    SlotsProcessor.cpp
    Created: 14 Jul 2025 9:01:03pm
    Author:  rasmu

  ==============================================================================
*/

#include "SlotsProcessor.h"

SlotProcessor::SlotProcessor()
{
    formatManager.registerBasicFormats();
}

SlotProcessor::~SlotProcessor()
{
    transportSource.setSource(nullptr);
    readerSource.reset();
}

void SlotProcessor::prepareToPlay(double sampleRate, int samplePerBlock)
{
    transportSource.prepareToPlay(samplePerBlock, sampleRate);
}

void SlotProcessor::process(juce::AudioBuffer<float>& buffer, const bool& isReference)
{
    transportSource.start();
    if (isReference)
    {
        buffer.clear();

        transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
    } else
    {
        juce::AudioBuffer<float> tempBuffer(buffer.getNumChannels(), buffer.getNumSamples());
        transportSource.getNextAudioBlock(juce::AudioSourceChannelInfo(tempBuffer));
    }

    loopingZoneProcessor.process(buffer, transportSource);
}

void SlotProcessor::loadFile(const juce::File& file)
{
    currentFile = file;
    auto* reader = formatManager.createReaderFor(file);
    if (reader != nullptr)
    {
        transportSource.setSource(nullptr);
        readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
        transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        thumbnail.setSource(new juce::FileInputSource(file));
        setFileName(file.getFileNameWithoutExtension());
    }
}

std::shared_ptr<const juce::String> SlotProcessor::getFileName() const
{
    return std::atomic_load_explicit(&fileName, std::memory_order_acquire);
}

void SlotProcessor::setFileName(juce::String newFileName)
{
    std::shared_ptr<const juce::String> newPtr = std::make_shared<juce::String>(newFileName);

    std::atomic_store_explicit(&fileName, newPtr, std::memory_order_release);
}
