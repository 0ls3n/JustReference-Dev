/*
  ==============================================================================

    WaveformVisualizerComponent.h
    Created: 20 Jun 2025 10:45:26pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformVisualizerComponent  : public juce::Component, private juce::ChangeListener
{
public:
    WaveformVisualizerComponent();
    ~WaveformVisualizerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void setAudioFile(const juce::File& audioFile);

private:

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache{ 10 };
	juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };

    // Inherited via ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformVisualizerComponent)
    

        

};
