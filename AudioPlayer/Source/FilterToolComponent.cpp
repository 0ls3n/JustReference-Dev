/*
  ==============================================================================

    FilterToolComponent.cpp
    Created: 24 Jun 2025 1:48:12pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterToolComponent.h"

//==============================================================================
FilterToolComponent::FilterToolComponent()
{
    addAndMakeVisible(subFilterButton);
	subFilterButton.setButtonText("Sub");
	subFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	//subFilterButton.setSize(20, getHeight());

	addAndMakeVisible(lowMidFilterButton);
	lowMidFilterButton.setButtonText("Low Mid");
	lowMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	//lowMidFilterButton.setSize(20, getHeight());

	addAndMakeVisible(highMidFilterButton);
	highMidFilterButton.setButtonText("High Mid");
	highMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	//highMidFilterButton.setSize(20, getHeight());

    addAndMakeVisible(airFilterButton);
	airFilterButton.setButtonText("Air");
	airFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	//airFilterButton.setSize(20, getHeight());
}

FilterToolComponent::~FilterToolComponent()
{
}

void FilterToolComponent::paint (juce::Graphics& g)
{
    g.fillAll (ApplicationColours().toolbar);
}

void FilterToolComponent::resized()
{
	auto area = getLocalBounds();
	
	int buttonWidth = 80;
	int buttonHeight = 30;
	int buttonSpacing = 20;

	int totalWidth = 4 * buttonWidth + 3 * buttonSpacing;
	int startX = (area.getWidth() - totalWidth) / 2;
	int y = (area.getHeight() - buttonHeight) / 2;

	subFilterButton.setBounds(startX, y, buttonWidth, buttonHeight);
	lowMidFilterButton.setBounds(startX + (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
	highMidFilterButton.setBounds(startX + 2 * (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
	airFilterButton.setBounds(startX + 3 * (buttonWidth + buttonSpacing), y, buttonWidth, buttonHeight);
}
