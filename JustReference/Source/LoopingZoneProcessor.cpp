#include "LoopingZoneProcessor.h"
#include "PluginProcessor.h"

LoopingZoneProcessor::LoopingZoneProcessor() = default;

LoopingZoneProcessor::~LoopingZoneProcessor() = default;

void LoopingZoneProcessor::setLoopDurationInSeconds(double startInSeconds, double endInSeconds)
{
    loopStartInSeconds = startInSeconds;
    loopEndInSeconds = endInSeconds;
}

void LoopingZoneProcessor::process(juce::AudioBuffer<float>& buffer, juce::AudioTransportSource& transportSource)
{
    if (transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(0.0);
    }

    if (loopEnabled)
    {
        if (transportSource.getCurrentPosition() >= loopEndInSeconds)
        {
            transportSource.setPosition(loopStartInSeconds);
        }
    }
}


