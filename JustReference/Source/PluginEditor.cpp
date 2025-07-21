/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerAudioProcessorEditor::AudioPlayerAudioProcessorEditor (AudioPlayerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 600);

	setResizable(false, false);

    addAndMakeVisible(brandingHeader);
    addAndMakeVisible(transportTool);
	addAndMakeVisible(filterTool);
    addAndMakeVisible(slotComponent);

    slotComponent.onSlotSelected = [this](SlotSelected slotToBeSelected)
        {
            changeSlot(slotToBeSelected);
        };

	transportTool.onReferenceButtonClicked = [this] { referenceSwitchButtonClicked(); };

    addAndMakeVisible(filterToggleButton);
    filterToggleButton.setButtonText("Filters");
	filterToggleButton.setColour(juce::TextButton::buttonColourId, ApplicationColours::secondary);

    filterToggleButton.onClick = [this]() {
        if (filterIsAnimating)
            return;

        filterIsVisible = !filterIsVisible;
        filterStartHeight = filterCurrentHeight;
        filterTargetHeight = filterIsVisible ? 60 : 0;

        filterAnimationElapsed = 0;
        filterIsAnimating = true;
        };

    addAndMakeVisible(slotMainContent1);
    slotMainContent1.setVisible(true);

    addAndMakeVisible(slotMainContent2);
    slotMainContent2.setVisible(false);

    addAndMakeVisible(slotMainContent3);
    slotMainContent3.setVisible(false);

    addAndMakeVisible(slotMainContent4);
    slotMainContent4.setVisible(false);

    slotMainContent1.update();
    slotMainContent2.update();
    slotMainContent3.update();
    slotMainContent4.update();


    startTimerHz(30);

    updateButtonStates();
}

AudioPlayerAudioProcessorEditor::~AudioPlayerAudioProcessorEditor() {  }

//==============================================================================
void AudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(ApplicationColours::background);
}

void AudioPlayerAudioProcessorEditor::resized()
{
    auto headerAndFooterHeight = 60;
	auto sidebarWidth = 80;
    auto buttonWidth = 100;
    auto buttonMargin = 10;
    auto sideBarWidth = 100;
    auto area = getLocalBounds();
    auto filterButtonContainerHeight = 60;

	auto headerArea = area.removeFromTop(headerAndFooterHeight);
    auto slotArea = area.removeFromTop(60);
	auto transportArea = area.removeFromBottom(headerAndFooterHeight);
	auto filterArea = area.removeFromBottom(filterCurrentHeight);
    auto filterToggleButtonArea = area.removeFromBottom(20);
    auto leftSidebarArea = area.removeFromLeft(sideBarWidth);
    auto rightSidebarArea = area.removeFromRight(sideBarWidth);

	brandingHeader.setBounds(headerArea);
    slotComponent.setBounds(slotArea);
	filterTool.setBounds(filterArea);
    transportTool.setBounds(transportArea);

    slotMainContent1.setBounds(area);
    slotMainContent2.setBounds(area);
    slotMainContent3.setBounds(area);
    slotMainContent4.setBounds(area);

    filterToggleButton.setBounds(getWidth() / 2 - (buttonWidth / 2), filterToggleButtonArea.getY(), buttonWidth, 20);
}

void AudioPlayerAudioProcessorEditor::referenceSwitchButtonClicked()
{
	audioProcessor.isReferenceActive = !audioProcessor.isReferenceActive;

    if (audioProcessor.isReferenceActive)
    {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours::primary);
    }
    else
    {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours::secondary);
    }

	repaint();

    slotMainContent1.update();
    slotMainContent2.update();
    slotMainContent3.update();
    slotMainContent4.update();
}

void AudioPlayerAudioProcessorEditor::updateButtonStates()
{
    changeSlot(audioProcessor.getSlotSelected());

    if (audioProcessor.isReferenceActive)
    {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours::primary);
    }
    else {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours::secondary);
    }
}

void AudioPlayerAudioProcessorEditor::saveFilePath(const juce::String& path) const
{
    audioProcessor.getTreeState().state.setProperty("filePath", path, nullptr);
}

void AudioPlayerAudioProcessorEditor::changeSlot(SlotSelected slot)
{
	switch (slot)
	{
	case SlotSelected::Slot1:
        slotMainContent1.setVisible(true);
        slotMainContent2.setVisible(false);
        slotMainContent3.setVisible(false);
        slotMainContent4.setVisible(false);
        break;
    case SlotSelected::Slot2:
        slotMainContent1.setVisible(false);
        slotMainContent2.setVisible(true);
        slotMainContent3.setVisible(false);
        slotMainContent4.setVisible(false);
        break;
    case SlotSelected::Slot3:
        slotMainContent1.setVisible(false);
        slotMainContent2.setVisible(false);
        slotMainContent3.setVisible(true);
        slotMainContent4.setVisible(false);
        break;
    case SlotSelected::Slot4:
        slotMainContent1.setVisible(false);
        slotMainContent2.setVisible(false);
        slotMainContent3.setVisible(false);
        slotMainContent4.setVisible(true);
        break;
	}
}

void AudioPlayerAudioProcessorEditor::timerCallback()
{

    if (filterIsAnimating)
    {
        filterAnimationElapsed += 1000 / 30;

        float progress = juce::jlimit(0.0f, 1.0f,
            (float)filterAnimationElapsed / (float)filterAnimationDuration);

        // Ease in-out
        float eased = 0.5f - 0.5f * std::cos(progress * juce::MathConstants<float>::pi);

        filterCurrentHeight = juce::jmap(eased,
            (float)filterStartHeight,
            (float)filterTargetHeight);

        resized();

        if (progress >= 1.0f)
        {
            filterCurrentHeight = filterTargetHeight;
            filterIsAnimating = false;
        }
    }
}
