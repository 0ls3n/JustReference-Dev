/*
  ==============================================================================

    SlotsComponent.cpp
    Created: 13 Jul 2025 10:40:24pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SlotsComponent.h"
#include "ApplicationColours.h"

//==============================================================================
SlotsComponent::SlotsComponent()
{
    addAndMakeVisible(slot1);
    slot1.setButtonText("Slot 1");
    slot1.setColour(juce::TextButton::buttonColourId, ApplicationColours::primary);

    addAndMakeVisible(slot2);
    slot2.setButtonText("Slot 2");
    slot2.setColour(juce::TextButton::buttonColourId, ApplicationColours::secondary);

    addAndMakeVisible(slot3);
    slot3.setButtonText("Slot 3");
    slot3.setColour(juce::TextButton::buttonColourId, ApplicationColours::secondary);

    addAndMakeVisible(slot4);
    slot4.setButtonText("Slot 4");
    slot4.setColour(juce::TextButton::buttonColourId, ApplicationColours::secondary);

}

SlotsComponent::~SlotsComponent()
{
}

void SlotsComponent::paint (juce::Graphics& g)
{
    g.fillAll(ApplicationColours::background);

}

void SlotsComponent::resized()
{
    auto area = getLocalBounds();

    int buttonWidth = 80;
    int buttonHeight = 30;
    int buttonSpacing = 20;

    int totalWidth = 4 * buttonWidth + 3 * buttonSpacing;
    int startX = (area.getWidth() - totalWidth) / 2;
    int y = (area.getHeight() - buttonHeight) / 2;

    slot1.setBounds(startX, y, buttonWidth, buttonHeight);
    slot2.setBounds(startX + (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
    slot3.setBounds(startX + 2 * (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
    slot4.setBounds(startX + 3 * (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
}
