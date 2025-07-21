/*
  ==============================================================================

    SlotMainContentComponent.h
    Created: 20 Jul 2025 11:13:19pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SlotsProcessor.h"
#include "WaveformVisualizerComponent.h"

//==============================================================================
/*
*/
class SlotMainContentComponent  : public juce::Component, private juce::Timer
{
public:
	

	SlotMainContentComponent(SlotProcessor& p);
    ~SlotMainContentComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setSongTitle(const juce::String& newTitle) { songTitle = newTitle; }

    void loadFile(const juce::File& file);

    void update();

private:

    void timerCallback() override;

    std::unique_ptr<juce::FileChooser> chooser;

    SlotProcessor& slotProcessor;

    juce::String songTitle;
    juce::Label songTitleLabel;

    WaveformVisualizerComponent waveformVisualizer{ slotProcessor.getAudioThumbnail(), slotProcessor.getLoopingZoneProcessor() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotMainContentComponent)
};
