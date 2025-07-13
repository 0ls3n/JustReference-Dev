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
#include "TransportToolComponent.h"
#include "BrandingHeader.h"
#include "FilterToolComponent.h"
#include "SlotsComponent.h"

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

    void loadFile(const juce::File& file);

    void openButtonClicked();
	void referenceSwitchButtonClicked();

	void updateButtonStates();

    juce::String songTitle;

    void saveFilePath(const juce::String& path) const;

    juce::Label songTitleLabel;

    BrandingHeader brandingHeader{ "JustReference", "JustMixing" };
    FilterToolComponent filterTool{ audioProcessor.soloFilterProcessing.getSoloFilterType() };
    SlotsComponent slotComponent;
	TransportToolComponent transportTool;

    WaveformVisualizerComponent waveformVisualizer{ audioProcessor.getAudioThumbnail(), audioProcessor.getLoopingZoneProcessor() };

    void timerCallback() override;

    int filterTargetHeight = 60;     // Target height for the filter area
    int filterCurrentHeight = 0;    // Animated current height
    int filterStartHeight = 0;      // Start height of the animation
    int filterAnimationDuration = 200; // ms
    int filterAnimationElapsed = 0;

    bool filterIsVisible = false;
    bool filterIsAnimating = false;

    juce::TextButton filterToggleButton;
	

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerAudioProcessorEditor)

};
