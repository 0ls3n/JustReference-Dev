#include <JuceHeader.h>
#include "LoopingZoneComponent.h"

//==============================================================================
LoopingZoneComponent::LoopingZoneComponent(juce::AudioThumbnail& sharedThumbnail, LoopingZoneProcessor& l) : audioThumbnail(sharedThumbnail), loopingZoneProcessor(l)
{
    setInterceptsMouseClicks(false, false);

    loopStartX = loopingZoneProcessor.getLoopStart();
    loopEndX = loopingZoneProcessor.getLoopEnd();
    loopEnabled = loopingZoneProcessor.getLoopEnabled();
    repaint();
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


        if (x1 != 0 || x2 != 0) {
            auto loopRect = juce::Rectangle<int>(x1, 0, x2 - x1, getHeight());

            g.setColour(juce::Colours::lightblue.withAlpha(0.2f));
            g.fillRect(loopRect);

            g.setColour(juce::Colours::lightblue.withAlpha(0.2f));
            g.drawRect(loopRect, 2);
        }
    }
}

void LoopingZoneComponent::resized()
{
    
}

void LoopingZoneComponent::onMouseDown(const juce::MouseEvent& event)
{
    isDraggingLoop = true;
    loopEnabled = false;
    loopStartX = event.x;
	loopEndX = event.x;

    loopingZoneProcessor.setLoopEnabled(loopEnabled);

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

	int distanceDragged = std::abs(loopEndX - loopStartX);

    const int dragThreshold = 4;

    if (distanceDragged >= dragThreshold)
    {
        loopEnabled = true;
    }
    else {
        loopEnabled = false;
        loopStartX = 0;
		loopEndX = 0;
    }

    loopingZoneProcessor.setLoopEnabled(loopEnabled);
    loopingZoneProcessor.setLoopStart(loopStartX);
    loopingZoneProcessor.setLoopEnd(loopEndX);

    loopingZoneProcessor.setLoopDurationInSeconds(std::min(xToTime(loopStartX), xToTime(loopEndX)),
        std::max(xToTime(loopEndX), xToTime(loopStartX)));

    repaint();
}

double LoopingZoneComponent::xToTime(int x) const
{
    double proportion = static_cast<double>(x) / getWidth();
    return proportion * audioThumbnail.getTotalLength();
}

double LoopingZoneComponent::timeToX(double time) const
{
    double proportion = time / audioThumbnail.getTotalLength();
    return proportion * getWidth();
}

