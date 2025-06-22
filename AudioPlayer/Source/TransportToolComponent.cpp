/*
  ==============================================================================

    TransportToolComponent.cpp
    Created: 22 Jun 2025 8:20:27pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TransportToolComponent.h"

//==============================================================================
TransportToolComponent::TransportToolComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    playButton.setEnabled(false);
    playButton.onClick = [this]()
        {
            if (onPlayButtonClicked) onPlayButtonClicked();
        };

	addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    stopButton.setEnabled(false);
    stopButton.onClick = [this]()
        {
            if (onStopButtonClicked) onStopButtonClicked();
        };

    addAndMakeVisible(openButton);
    openButton.setButtonText("Open file");
    openButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    openButton.onClick = [this]()
        {
            if (onOpenButtonClicked) onOpenButtonClicked();
        };

    addAndMakeVisible(referenceButton);
    referenceButton.setButtonText("Reference");
    referenceButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    referenceButton.setEnabled(true);
    referenceButton.onClick = [this]()
        {
            if (onReferenceButtonClicked) onReferenceButtonClicked();
        };
}

TransportToolComponent::~TransportToolComponent()
{
}

void TransportToolComponent::paint (juce::Graphics& g)
{
    g.fillAll (ApplicationColours().toolbar);
}

void TransportToolComponent::resized()
{
    auto area = getLocalBounds();

    auto buttonWidth = 100;
    auto buttonMargin = 10;

	openButton.setBounds(area.removeFromLeft(buttonWidth).reduced(buttonMargin));
	referenceButton.setBounds(area.removeFromLeft(buttonWidth).reduced(buttonMargin));
	playButton.setBounds(area.removeFromLeft(buttonWidth).reduced(buttonMargin));
	stopButton.setBounds(area.removeFromLeft(buttonWidth).reduced(buttonMargin));

}
