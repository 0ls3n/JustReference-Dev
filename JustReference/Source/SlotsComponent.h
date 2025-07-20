/*
  ==============================================================================

    SlotsComponent.h
    Created: 13 Jul 2025 10:40:24pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "WaveformVisualizerComponent.h"

class AudioPlayerAudioProcessor;


class SlotsComponent  : public juce::Component
{
public:

    SlotsComponent(SlotSelected& sharedSlotSelected);
    ~SlotsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    SlotSelected getCurrentSlotSelected() const { return currentSlot; }
    void setCurrentSlotSelected(SlotSelected slotToBeSelected) { currentSlot = slotToBeSelected; }

private:

    void updateButtonState();

    juce::TextButton slot1;
    juce::TextButton slot2;
    juce::TextButton slot3;
    juce::TextButton slot4;

    SlotSelected& currentSlot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotsComponent)
};
