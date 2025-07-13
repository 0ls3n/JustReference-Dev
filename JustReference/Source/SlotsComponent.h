/*
  ==============================================================================

    SlotsComponent.h
    Created: 13 Jul 2025 10:40:24pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SlotsComponent  : public juce::Component
{
public:
    SlotsComponent();
    ~SlotsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::TextButton slot1;
    juce::TextButton slot2;
    juce::TextButton slot3;
    juce::TextButton slot4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlotsComponent)
};
