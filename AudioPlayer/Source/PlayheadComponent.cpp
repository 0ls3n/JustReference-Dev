/*
  ==============================================================================

    PlayheadComponent.cpp
    Created: 23 Jun 2025 7:36:06pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayheadComponent.h"

//==============================================================================
PlayheadComponent::PlayheadComponent()
{
	setInterceptsMouseClicks(false, false);
}

PlayheadComponent::~PlayheadComponent()
{
}

void PlayheadComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.drawLine(playheadX, 0.0f, playheadX, (float)getHeight(), 2.0f);
}

void PlayheadComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void PlayheadComponent::setPlayheadPosition(double x)
{
    auto oldX = playheadX;
    playheadX = x;

    repaint(juce::Rectangle<int>((int)oldX - 2, 0, 4, getHeight()));
    repaint(juce::Rectangle<int>((int)playheadX - 2, 0, 4, getHeight()));
}
