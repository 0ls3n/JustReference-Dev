/*
  ==============================================================================

    LoopingZoneProcessor.h
    Created: 2 Jul 2025 8:46:21pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioPlayerAudioProcessor;

class LoopingZoneProcessor
{
public:
    LoopingZoneProcessor(AudioPlayerAudioProcessor&);
    ~LoopingZoneProcessor();

    void setLoopStart(int startX) { loopStartX = startX; }
	void setLoopEnd(int endX) { loopEndX = endX; }

	int getLoopStart() const { return loopStartX; }
	int getLoopEnd() const { return loopEndX; }

	void setLoopEnabled(bool isEnabled) { loopEnabled = isEnabled; }

    void process(juce::AudioBuffer<float>& buffer, juce::AudioTransportSource& transportSource);

private:

	bool loopEnabled = false;
    int loopStartX = 0;
	int loopEndX = 0;

    AudioPlayerAudioProcessor& audioProcessor;
};