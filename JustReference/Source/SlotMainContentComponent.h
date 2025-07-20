/*
  ==============================================================================

    SlotMainContentComponent.h
    Created: 20 Jul 2025 11:13:19pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "SlotsProcessor.h"
#include "WaveformVisualizerComponent.h"

//==============================================================================
/*
*/
class SlotMainContentComponent  : public juce::Component
{
public:
    SlotMainContentComponent(SlotProcessor& p);
    ~SlotMainContentComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setSongTitle(const juce::String& newTitle) { songTitle = newTitle; }

    void loadFile();

private:

    SlotProcessor& audioProcessor;

    juce::String songTitle;
    juce::Label songTitleLabel;

    WaveformVisualizerComponent waveformVisualizer{ audioProcessor.getAudioThumbnail(), audioProcessor.getLoopingZoneProcessor() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotMainContentComponent)
};
