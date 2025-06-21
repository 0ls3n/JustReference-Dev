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

	currentPlayheadTime = 0.0;
}

WaveformVisualizerComponent::~WaveformVisualizerComponent()
{
    thumbnail.removeChangeListener(this);
}

void WaveformVisualizerComponent::paint (juce::Graphics& g)
{

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

    auto duration = thumbnail.getTotalLength();

    if (duration > 0.0)
    {


        g.setColour(waveformColour);
        auto bounds = getLocalBounds();

        // Draw waveform using built-in thumbnail renderer
        thumbnail.drawChannels(g, bounds, 0.0, thumbnail.getTotalLength(), .7f);

        double playheadX = (currentPlayheadTime / duration) * getWidth();
        g.setColour(juce::Colours::white);
        g.drawLine(playheadX, 0.0f, playheadX, (float)getHeight(), 2.0f);

    }
    else
    {
		g.setColour(juce::Colours::white);
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

void WaveformVisualizerComponent::setWaveformColour(const juce::Colour& colour)
{
    waveformColour = colour;
	repaint();
}

void WaveformVisualizerComponent::setPlayheadTime(double timeInSeconds)
{
    currentPlayheadTime = timeInSeconds;
    repaint();
}

void WaveformVisualizerComponent::mouseDown(const juce::MouseEvent& e)
{
    double duration = thumbnail.getTotalLength();
    if (duration > 0.0)
    {
        double clickedTime = (double)e.x / getWidth() * duration;

        if (onSeek != nullptr)
            onSeek(clickedTime);
    }
}

void WaveformVisualizerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint(); // Trigger a repaint when the thumbnail changes
	}
}
