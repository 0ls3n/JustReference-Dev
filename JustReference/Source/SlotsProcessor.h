#pragma once
#include <JuceHeader.h>
#include "LoopingZoneProcessor.h"

class SlotProcessor
{
public:

	SlotProcessor();
	~SlotProcessor();

	void prepareToPlay(double sampleRate, int samplePerBlock);
	void process(juce::AudioBuffer<float>& buffer, bool& isReference);

	void loadFile(const juce::File& file);
	std::shared_ptr<const juce::String> getFileName() const;

	juce::AudioThumbnail& getAudioThumbnail() { return thumbnail; }
	juce::AudioThumbnailCache& getAudioThumbnailCache() { return thumbnailCache; }

	juce::AudioTransportSource& getTransportSource() { return transportSource; }
private:
	juce::AudioTransportSource transportSource;
	juce::File currentFile;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

	void setFileName(juce::String newFileName);
	std::shared_ptr<const juce::String> fileName{ std::make_shared<juce::String>("No song loaded...") };

	juce::AudioThumbnailCache thumbnailCache{ 10 };
	juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };

	LoopingZoneProcessor loopingZoneProcessor;
};