/*
  ==============================================================================

    BrandingHeader.cpp
    Created: 22 Jun 2025 9:19:01pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BrandingHeader.h"

//==============================================================================
BrandingHeader::BrandingHeader(juce::String pluginName, juce::String brandName)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	addAndMakeVisible(pluginNameLabel);
	pluginNameLabel.setText(pluginName, juce::NotificationType::dontSendNotification);
	pluginNameLabel.setJustificationType(juce::Justification::centredLeft);
	pluginNameLabel.setFont(juce::Font(20.0f, juce::Font::bold));
	pluginNameLabel.setSize(200, 60);

	addAndMakeVisible(brandNameLabel);
	brandNameLabel.setText(brandName, juce::NotificationType::dontSendNotification);
	brandNameLabel.setJustificationType(juce::Justification::centredRight);
	brandNameLabel.setFont(juce::Font(20.0f, juce::Font::bold));
	brandNameLabel.setSize(200, 60);

}

BrandingHeader::~BrandingHeader()
{
}

void BrandingHeader::paint (juce::Graphics& g)
{
    g.fillAll(ApplicationColours::toolbar);
}

void BrandingHeader::resized()
{
	auto area = getLocalBounds();

	pluginNameLabel.setBounds(area.removeFromLeft(200));
	brandNameLabel.setBounds(area.removeFromRight(200));
}
