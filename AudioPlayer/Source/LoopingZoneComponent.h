#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoopingZoneComponent  : public juce::Component
{
public:
    LoopingZoneComponent();
    ~LoopingZoneComponent() override;

	void onMouseDown(const juce::MouseEvent& event);
	void onMouseDrag(const juce::MouseEvent& event);
	void onMouseUp(const juce::MouseEvent& event);

    bool getIsLooping() { return loopEnabled; }

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Rectangle<int> loopRegion;

    bool isDraggingLoop = false;
    int loopStartX = 0;
	int loopEndX = 0;
    bool loopEnabled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopingZoneComponent)
};
