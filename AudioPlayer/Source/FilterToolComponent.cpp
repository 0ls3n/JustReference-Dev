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
FilterToolComponent::FilterToolComponent(SoloFilterType& sharedFilterType) : soloFilter(sharedFilterType)
{
    addAndMakeVisible(subFilterButton);
	subFilterButton.setButtonText("Sub");
	subFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	subFilterButton.onClick = [this]() 
		{
			if (soloFilter == SoloFilterType::Sub)
				soloFilter = SoloFilterType::NoSolo; // Toggle off if already selected
			else {
				soloFilter = SoloFilterType::Sub;
			}
				repaint();
		};

	addAndMakeVisible(lowMidFilterButton);
	lowMidFilterButton.setButtonText("Low Mid");
	lowMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	lowMidFilterButton.onClick = [this]() 
		{
			if (soloFilter == SoloFilterType::LowMid)
				soloFilter = SoloFilterType::NoSolo; // Toggle off if already selected
			else {
				soloFilter = SoloFilterType::LowMid;
			}
			repaint();
		};

	addAndMakeVisible(highMidFilterButton);
	highMidFilterButton.setButtonText("High Mid");
	highMidFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	highMidFilterButton.onClick = [this]()
		{
			if (soloFilter == SoloFilterType::HighMid)
				soloFilter = SoloFilterType::NoSolo; // Toggle off if already selected
			else
			{
				soloFilter = SoloFilterType::HighMid;
			}
			repaint();
		};

    addAndMakeVisible(airFilterButton);
	airFilterButton.setButtonText("Air");
	airFilterButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
	airFilterButton.onClick = [this]() 
		{
			if (soloFilter == SoloFilterType::High)
				soloFilter = SoloFilterType::NoSolo; // Toggle off if already selected
			else
			{
				soloFilter = SoloFilterType::High;
			}
			repaint();
		};

	updateFilterButtons();

}

FilterToolComponent::~FilterToolComponent()
{
}

void FilterToolComponent::paint (juce::Graphics& g)
{
    g.fillAll (ApplicationColours().toolbar);
	updateFilterButtons();
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

void FilterToolComponent::updateFilterButtons()
{
	switch (soloFilter)
	{
	case SoloFilterType::Sub:
		changeButtonColour(SubFilterButton, ApplicationColours().primary);
		changeButtonColour(LowMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(HighMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(AirFilterButton, ApplicationColours().secondary);
		break;
	case SoloFilterType::LowMid:
		changeButtonColour(SubFilterButton, ApplicationColours().secondary);
		changeButtonColour(LowMidFilterButton, ApplicationColours().primary);
		changeButtonColour(HighMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(AirFilterButton, ApplicationColours().secondary);
		break;
	case SoloFilterType::HighMid:
		changeButtonColour(SubFilterButton, ApplicationColours().secondary);
		changeButtonColour(LowMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(HighMidFilterButton, ApplicationColours().primary);
		changeButtonColour(AirFilterButton, ApplicationColours().secondary);
		break;
	case SoloFilterType::High:
		changeButtonColour(SubFilterButton, ApplicationColours().secondary);
		changeButtonColour(LowMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(HighMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(AirFilterButton, ApplicationColours().primary);
		break;
	case SoloFilterType::NoSolo:
		changeButtonColour(SubFilterButton, ApplicationColours().secondary);
		changeButtonColour(LowMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(HighMidFilterButton, ApplicationColours().secondary);
		changeButtonColour(AirFilterButton, ApplicationColours().secondary);
		break;

	default:
		break;
	}
}
