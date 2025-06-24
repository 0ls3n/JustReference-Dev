/*
  ==============================================================================

    FilterToolComponent.h
    Created: 24 Jun 2025 1:48:12pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ApplicationColours.h"

//==============================================================================
/*
*/
class FilterToolComponent  : public juce::Component
{
public:

    enum FilterButton
    {
        SubFilterButton,
        LowMidFilterButton,
        HighMidFilterButton,
        AirFilterButton
	};

    FilterToolComponent();
    ~FilterToolComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void changeButtonColour(FilterButton filterType, juce::Colour colour);

	std::function<void()> onSubFilterButtonClicked;
	std::function<void()> onLowMidFilterButtonClicked;
	std::function<void()> onHighMidFilterButtonClicked;
	std::function<void()> onAirFilterButtonClicked;

private:

    juce::TextButton subFilterButton;
	juce::TextButton lowMidFilterButton;
	juce::TextButton highMidFilterButton;
	juce::TextButton airFilterButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterToolComponent)
};
