#pragma once
#include <JuceHeader.h>
#include "LoopingZoneProcessor.h"


class AudioPlayerAudioProcessor;

class SlotProcessor
{

public:

	SlotProcessor(AudioPlayerAudioProcessor& p);
	~SlotProcessor();

	void prepareToPlay(double sampleRate, int samplePerBlock);
	void process(juce::AudioBuffer<float>& buffer);

	void loadFile(const juce::File& file);
	std::shared_ptr<const juce::String> getFileName() const;

	juce::AudioThumbnail& getAudioThumbnail() { return thumbnail; }
	juce::AudioThumbnailCache& getAudioThumbnailCache() { return thumbnailCache; }

	juce::AudioTransportSource& getTransportSource() { return transportSource; }

	LoopingZoneProcessor& getLoopingZoneProcessor() { return loopingZoneProcessor; }

	bool isFileLoaded() const { return readerSource != nullptr; }

	bool getIsReference() const;

private:

	AudioPlayerAudioProcessor& audioProcessor;

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
