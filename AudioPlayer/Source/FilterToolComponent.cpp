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
	subFilterButton.onClick = [this]() {
			if (onSubFilterButtonClicked)
				onSubFilterButtonClicked();
		};

	addAndMakeVisible(lowMidFilterButton);
	lowMidFilterButton.setButtonText("Low Mid");
	lowMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	lowMidFilterButton.onClick = [this]() {
			if (onLowMidFilterButtonClicked)
				onLowMidFilterButtonClicked();
		};

	addAndMakeVisible(highMidFilterButton);
	highMidFilterButton.setButtonText("High Mid");
	highMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	highMidFilterButton.onClick = [this]() {
			if (onHighMidFilterButtonClicked)
				onHighMidFilterButtonClicked();
		};

    addAndMakeVisible(airFilterButton);
	airFilterButton.setButtonText("Air");
	airFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	airFilterButton.onClick = [this]() {
			if (onAirFilterButtonClicked)
				onAirFilterButtonClicked();
		};
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

void FilterToolComponent::changeButtonColour(FilterButton filterButton, juce::Colour colour)
{
	switch (filterButton)
	{
	case FilterToolComponent::SubFilterButton:
		subFilterButton.setColour(juce::TextButton::buttonColourId, colour);
		break;
	case FilterToolComponent::LowMidFilterButton:
		lowMidFilterButton.setColour(juce::TextButton::buttonColourId, colour);
		break;
	case FilterToolComponent::HighMidFilterButton:
		highMidFilterButton.setColour(juce::TextButton::buttonColourId, colour);
		break;
	case FilterToolComponent::AirFilterButton:
		airFilterButton.setColour(juce::TextButton::buttonColourId, colour);
		break;
	default:
		break;
	}
}
