#include "LoopingZoneProcessor.h"
#include "PluginProcessor.h"

LoopingZoneProcessor::LoopingZoneProcessor(AudioPlayerAudioProcessor& p) : audioProcessor(p)
{

}

LoopingZoneProcessor::~LoopingZoneProcessor() = default;

void LoopingZoneProcessor::process(juce::AudioBuffer<float>& buffer, juce::AudioTransportSource& transportSource)
{
    if (transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(0.0);
    }
}


