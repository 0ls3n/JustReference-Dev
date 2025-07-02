#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoopingZoneComponent  : public juce::Component
{
public:
    LoopingZoneComponent(juce::AudioThumbnail&);
    ~LoopingZoneComponent() override;

	void onMouseDown(const juce::MouseEvent& event);
	void onMouseDrag(const juce::MouseEvent& event);
	void onMouseUp(const juce::MouseEvent& event);

    double xToTime(int x) const;
    double timeToX(double time) const;

    bool getIsLooping() { return loopEnabled; }

    int getLoopStart() { return loopStartX; }
    int getLoopEnd() { return loopEndX; }

    void setLoopEnabled(bool enabled) { loopEnabled = enabled; }
    bool getLoopEnabled() { return loopEnabled; }

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Rectangle<int> loopRegion;

    bool isDraggingLoop = false;
    int loopStartX = 0;
	int loopEndX = 0;
    bool loopEnabled = false;

    juce::AudioThumbnail& audioThumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopingZoneComponent)
};
