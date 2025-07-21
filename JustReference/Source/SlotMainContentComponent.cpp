/*
  ==============================================================================

    SlotMainContentComponent.cpp
    Created: 20 Jul 2025 11:13:19pm
    Author:  rasmu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SlotMainContentComponent.h"



//==============================================================================
SlotMainContentComponent::SlotMainContentComponent(SlotProcessor& p) : slotProcessor(p)
{
    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(25.0f, juce::Font::bold));

    addAndMakeVisible(waveformVisualizer);

    waveformVisualizer.onSeek = [this](double position) {
        slotProcessor.getTransportSource().setPosition(position);
        repaint();
        };

    waveformVisualizer.onFileDropped = [this](const juce::StringArray& files, int x, int y)
        {
            if (files.size() > 0)
            {
                juce::File file(files[0]);
                loadFile(file);
            }
        };

    waveformVisualizer.onComponentClicked = [this]()
        {
            chooser = std::make_unique<juce::FileChooser>("Select a file...", juce::File{}, "*.wav;*.mp3");

            chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    loadFile(file);
                });
        };

    startTimerHz(30);

    chooser.reset();

    update();
}

SlotMainContentComponent::~SlotMainContentComponent() = default;

void SlotMainContentComponent::paint (juce::Graphics& g)
{
    
}

void SlotMainContentComponent::resized()
{
    auto area = getLocalBounds();
    auto headerAndFooterHeight = 60;

    songTitleLabel.setBounds(area.removeFromTop(60));
    waveformVisualizer.setBounds(area.removeFromTop(area.getHeight() - headerAndFooterHeight).reduced(20, 0));
}

void SlotMainContentComponent::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        slotProcessor.loadFile(file);
        auto fileName = slotProcessor.getFileName();

        slotProcessor.getLoopingZoneProcessor().setLoopEnabled(false);
        waveformVisualizer.getLoopingComponent().setLoopEnabled(false);
        waveformVisualizer.getLoopingComponent().repaint();
        if (fileName != nullptr)
        {
            this->songTitle = *fileName;
        }

        songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);

        repaint();
    }
}

void SlotMainContentComponent::update()
{
	if (slotProcessor.isFileLoaded())
	{
        waveformVisualizer.repaint();
	}

    if (slotProcessor.getFileName() != nullptr)
    {
        this->songTitle = *slotProcessor.getFileName();
    }

    if (slotProcessor.getIsReference())
    {
        waveformVisualizer.setWaveformColour(ApplicationColours::primary);
    } else
    {
        waveformVisualizer.setWaveformColour(ApplicationColours::secondary);
    }

    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
}

void SlotMainContentComponent::timerCallback()
{
    waveformVisualizer.setPlayheadTime(slotProcessor.getTransportSource().getCurrentPosition());
}
