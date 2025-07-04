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
WaveformVisualizerComponent::WaveformVisualizerComponent(juce::AudioThumbnail& t, LoopingZoneProcessor& l) : thumbnail(t), loopingZoneProcessor(l)
{
    setInterceptsMouseClicks(true, false);
	thumbnail.addChangeListener(this);

	currentPlayheadTime = 0.0;

	addAndMakeVisible(playheadComponent);
    addAndMakeVisible(loopingComponent);
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
		playheadComponent.setVisible(true);
        g.setColour(waveformColour);
        auto bounds = getLocalBounds();

        // Draw waveform using built-in thumbnail renderer
        thumbnail.drawChannel(g, bounds, 0.0, thumbnail.getTotalLength(), 0,.7f);
    }
    else
    {
		playheadComponent.setVisible(false);
		g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText("Right click, or drag and drop to open a file", getLocalBounds(), juce::Justification::centred, 1);

        g.drawRoundedRectangle(getLocalBounds().toFloat(), 20.0f, 1.0f);
    }
}

void WaveformVisualizerComponent::resized()
{
    playheadComponent.setBounds(getLocalBounds());
    loopingComponent.setBounds(getLocalBounds());
}

void WaveformVisualizerComponent::setWaveformColour(const juce::Colour& colour)
{
    waveformColour = colour;
	repaint();
}

void WaveformVisualizerComponent::setPlayheadTime(double timeInSeconds)
{
    double duration = thumbnail.getTotalLength();

    if (duration > 0.0)
    {
        double x = (timeInSeconds / duration) * getWidth();
		playheadComponent.setPlayheadPosition(x);
    }
}

void WaveformVisualizerComponent::mouseDown(const juce::MouseEvent& e)
{
    double duration = thumbnail.getTotalLength();
    if (duration > 0.0 && e.mods.isLeftButtonDown())
    {
        double clickedTime = (double)e.x / getWidth() * duration;

        loopingComponent.onMouseDown(e);

        if (onSeek != nullptr)
            onSeek(clickedTime);

    }

    if (e.mods.isRightButtonDown())
    {
        onComponentClicked();
    }
}

void WaveformVisualizerComponent::mouseDrag(const juce::MouseEvent& e)
{
    double duration = thumbnail.getTotalLength();
    if (duration > 0.0 && e.mods.isLeftButtonDown())
    {
        loopingComponent.onMouseDrag(e);
    }
}

void WaveformVisualizerComponent::mouseUp(const juce::MouseEvent& e)
{
    double duration = thumbnail.getTotalLength();
    if (duration > 0.0 && e.mods.isLeftButtonDown())
    {
        loopingComponent.onMouseUp(e);
    }
}

void WaveformVisualizerComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint(); // Trigger a repaint when the thumbnail changes
	}
}

bool WaveformVisualizerComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void WaveformVisualizerComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    onFileDropped(files, x, y);
}
