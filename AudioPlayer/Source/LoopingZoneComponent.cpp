#include <JuceHeader.h>
#include "LoopingZoneComponent.h"

//==============================================================================
LoopingZoneComponent::LoopingZoneComponent()
{
    setInterceptsMouseClicks(false, false);
}

LoopingZoneComponent::~LoopingZoneComponent()
{
}

void LoopingZoneComponent::paint (juce::Graphics& g)
{
    if (loopEnabled || isDraggingLoop)
    {
        auto x1 = std::min(loopStartX, loopEndX);
        auto x2 = std::max(loopStartX, loopEndX);
        auto loopRect = juce::Rectangle<int>(x1, 0, x2 - x1, getHeight());

        g.setColour(juce::Colours::lightblue.withAlpha(0.2f));
        g.fillRect(loopRect);

        g.setColour(juce::Colours::lightblue.withAlpha(0.2f));
        g.drawRect(loopRect, 2);
    }
}

void LoopingZoneComponent::resized()
{

}

void LoopingZoneComponent::onMouseDown(const juce::MouseEvent& event)
{
    isDraggingLoop = true;
    loopStartX = event.x;
	loopEndX = event.x;
    repaint();
}

void LoopingZoneComponent::onMouseDrag(const juce::MouseEvent& event)
{
    loopEndX = event.x;
    repaint();
}

void LoopingZoneComponent::onMouseUp(const juce::MouseEvent& event)
{
    isDraggingLoop = false;
    loopEndX = event.x;

    loopEnabled = true;
    repaint();
}

