/*
  ==============================================================================

    PlayheadComponent.h
    Created: 23 Jun 2025 7:36:06pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlayheadComponent  : public juce::Component
{
public:
    PlayheadComponent();
    ~PlayheadComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setPlayheadPosition(double x);

private:

    double playheadX = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayheadComponent)
};
