/*
  ==============================================================================

    SlotMainContentComponent.cpp
    Created: 20 Jul 2025 11:13:19pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SlotMainContentComponent.h"

//==============================================================================
SlotMainContentComponent::SlotMainContentComponent(SlotProcessor& p) : audioProcessor(p)
{
    addAndMakeVisible(waveformVisualizer);
    waveformVisualizer.onFileDropped = [this](const juce::StringArray& files, int x, int y)
        {

        };

    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(25.0f, juce::Font::bold));
}

SlotMainContentComponent::~SlotMainContentComponent() = default;

void SlotMainContentComponent::paint (juce::Graphics& g)
{
    
}

void SlotMainContentComponent::resized()
{
    auto area = getLocalBounds();


}
