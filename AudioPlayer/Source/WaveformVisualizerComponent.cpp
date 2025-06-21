/*
  ==============================================================================

    WaveformVisualizerComponent.cpp
    Created: 20 Jun 2025 10:45:26pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformVisualizerComponent.h"

//==============================================================================
WaveformVisualizerComponent::WaveformVisualizerComponent()
{
	formatManager.registerBasicFormats();

	thumbnail.addChangeListener(this);
}

WaveformVisualizerComponent::~WaveformVisualizerComponent()
{
    thumbnail.removeChangeListener(this);
}

void WaveformVisualizerComponent::paint (juce::Graphics& g)
{

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

    juce::ColourGradient gradient(ApplicationColours().primary, 0, 0,
        juce::Colours::lightblue, getWidth(), 0, false);

    if (thumbnail.getTotalLength() > 0.0)
    {

        g.setColour(ApplicationColours().primary);
        auto bounds = getLocalBounds();

        // Draw waveform using built-in thumbnail renderer
        thumbnail.drawChannels(g, bounds, 0.0, thumbnail.getTotalLength(), .7f);
    }
    else
    {
		g.setColour(ApplicationColours().primary);
        g.setFont(15.0f);
        g.drawFittedText("No waveform loaded", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void WaveformVisualizerComponent::resized()
{

}

void WaveformVisualizerComponent::setAudioFile(const juce::File& audioFile)
{
    if (audioFile.existsAsFile()) 
    {
        thumbnail.clear();
		thumbnail.setSource(new juce::FileInputSource(audioFile));
    }
}

void WaveformVisualizerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint(); // Trigger a repaint when the thumbnail changes
	}
}
