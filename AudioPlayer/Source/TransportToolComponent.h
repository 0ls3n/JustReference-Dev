/*
  ==============================================================================

    TransportToolComponent.h
    Created: 22 Jun 2025 8:20:27pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ApplicationColours.h"

//==============================================================================
/*
*/
class TransportToolComponent  : public juce::Component
{
public:
    TransportToolComponent();
    ~TransportToolComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void setPlayButtonEnabled(bool enabled) { playButton.setEnabled(enabled); }
	void setStopButtonEnabled(bool enabled) { stopButton.setEnabled(enabled); }

	void setPlayButtonText(const juce::String& text) { playButton.setButtonText(text); }

    void setReferenceButtonColour(const juce::Colour& colour) { 
        referenceButton.setColour(juce::TextButton::buttonColourId, colour); 
	}

    std::function<void()> onPlayButtonClicked;
	std::function<void()> onStopButtonClicked;
	std::function<void()> onOpenButtonClicked;
    std::function<void()> onReferenceButtonClicked;

private:

    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton openButton;
    juce::TextButton referenceButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportToolComponent)
};
