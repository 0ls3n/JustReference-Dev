/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SectionComponent.h"
#include "WaveformVisualizerComponent.h"
#include "ApplicationColours.h"
#include "TransportToolComponent.h"

//==============================================================================
/**
*/
class AudioPlayerAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    AudioPlayerAudioProcessorEditor (AudioPlayerAudioProcessor&);
    ~AudioPlayerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    AudioPlayerAudioProcessor& audioProcessor;
    std::unique_ptr<juce::FileChooser> chooser;

    void openButtonClicked();
    void playButtonClicked();
    void pauseButtonClicked();
    void stopButtonClicked();
	void referenceSwitchButtonClicked();

	void updateButtonStates();

    void changeTransportState(TransportState state);

    juce::String songTitle;

    juce::Label songTitleLabel;
    juce::Label pluginNameLabel;
    juce::Label brandNameLabel;

	SectionComponent header;
    SectionComponent leftSidebar;
    SectionComponent rightSidebar;

	TransportToolComponent transportTool;

    WaveformVisualizerComponent waveformVisualizer{ audioProcessor.getAudioThumbnail() };

    void timerCallback() override;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerAudioProcessorEditor)

};
