/*
  ==============================================================================

    WaveformVisualizerComponent.h
    Created: 20 Jun 2025 10:45:26pm
    Author:  rasmu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ApplicationColours.h"
#include "PlayheadComponent.h"

//==============================================================================
/*
*/
class WaveformVisualizerComponent : public juce::Component, private juce::ChangeListener, public juce::FileDragAndDropTarget
{
public:
    WaveformVisualizerComponent(juce::AudioThumbnail&);
    ~WaveformVisualizerComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setWaveformColour(const juce::Colour& colour);

    void setPlayheadTime(double timeInSeconds);

    std::function<void(double timeInSeconds)> onSeek;
    std::function<void(const juce::StringArray&, int, int)> onFileDropped;
    std::function<void()> onComponentClicked;

private:

    void WaveformVisualizerComponent::mouseDown(const juce::MouseEvent& e) override;
    void WaveformVisualizerComponent::mouseDrag(const juce::MouseEvent& e) override;

    juce::AudioThumbnail& thumbnail;

    juce::Colour waveformColour{ ApplicationColours().secondary };

    double currentPlayheadTime = 0.0f;

    // Inherited via ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

	PlayheadComponent playheadComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformVisualizerComponent)
};