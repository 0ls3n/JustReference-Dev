/*
  ==============================================================================

    SectionComponent.h
    Created: 20 Jun 2025 10:10:30pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SectionComponent  : public juce::Component
{
public:
    SectionComponent();
    ~SectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SectionComponent)
};
