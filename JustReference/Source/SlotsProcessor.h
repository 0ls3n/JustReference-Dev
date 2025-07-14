#pragma once
#include <JuceHeader.h>
#include "LoopingZoneProcessor.h"

class SlotProcessor
{
public:

	SlotProcessor();
	~SlotProcessor();

	void prepareToPlay(double sampleRate, int samplePerBlock);
	void process(juce::AudioBuffer<float>& buffer);

	juce::AudioTransportSource& getTransportSource() { return transportSource; }
private:
	juce::AudioTransportSource transportSource;

	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbnailCache{ 10 };
	juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };

	LoopingZoneProcessor loopingZoneProcessor;


};