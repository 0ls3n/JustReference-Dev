/*
  ==============================================================================

    LoopingZoneProcessor.h
    Created: 2 Jul 2025 8:46:21pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



class LoopingZoneProcessor
{
public:
    LoopingZoneProcessor();
    ~LoopingZoneProcessor();

    void setLoopStart(int startX) { loopStartX = startX; }
	void setLoopEnd(int endX) { loopEndX = endX; }

    void setLoopDurationInSeconds(double startInSeconds, double endInSeconds);

	int getLoopStart() const { return loopStartX; }
	int getLoopEnd() const { return loopEndX; }

	void setLoopEnabled(bool isEnabled) { loopEnabled = isEnabled; }
    bool getLoopEnabled() { return loopEnabled; }

    void process(juce::AudioBuffer<float>& buffer, juce::AudioTransportSource& transportSource);

private:

	bool loopEnabled = false;
    int loopStartX = 0;
	int loopEndX = 0;
    double loopStartInSeconds = 0.0f;
    double loopEndInSeconds = 0.0f;
};