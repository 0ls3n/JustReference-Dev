/*
  ==============================================================================

    SectionComponent.cpp
    Created: 20 Jun 2025 10:10:30pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SectionComponent.h"

//==============================================================================
SectionComponent::SectionComponent()
{
    
}

SectionComponent::~SectionComponent()
{
}

void SectionComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(30, 30, 30));
}

void SectionComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
