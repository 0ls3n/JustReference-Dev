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

	setResizable(true, true);

    addAndMakeVisible(brandingHeader);
    addAndMakeVisible(transportTool);

	transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
	transportTool.onStopButtonClicked = [this] { stopButtonClicked(); };
	transportTool.onReferenceButtonClicked = [this] { referenceSwitchButtonClicked(); };
	transportTool.onOpenButtonClicked = [this] { openButtonClicked(); };

    // addAndMakeVisible(rightSidebar);
    // addAndMakeVisible(leftSidebar);

    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(25.0f, juce::Font::bold));

	addAndMakeVisible(waveformVisualizer);
    waveformVisualizer.onSeek = [this](double position) {
        audioProcessor.transportSource.setPosition(position);
        repaint();
		};

    startTimerHz(30);

	chooser.reset();

    updateButtonStates();
}

AudioPlayerAudioProcessorEditor::~AudioPlayerAudioProcessorEditor() {  }

//==============================================================================
void AudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(ApplicationColours().background);

    g.setColour (juce::Colours::white);
}

void AudioPlayerAudioProcessorEditor::resized()
{
    auto headerAndFooterHeight = 60;
	auto sidebarWidth = 80;
    auto buttonWidth = 100;
    auto buttonMargin = 10;
    auto sideBarWidth = 100;
    auto area = getLocalBounds();

	auto headerArea = area.removeFromTop(headerAndFooterHeight);
	auto transportArea = area.removeFromBottom(headerAndFooterHeight);
    auto leftSidebarArea = area.removeFromLeft(sideBarWidth);
    auto rightSidebarArea = area.removeFromRight(sideBarWidth);

	brandingHeader.setBounds(headerArea);
    transportTool.setBounds(transportArea);

    songTitleLabel.setBounds(area.removeFromTop(60).reduced(0));

	waveformVisualizer.setBounds(area.removeFromTop(area.getHeight() - headerAndFooterHeight).reduced(20, 20));
}

void AudioPlayerAudioProcessorEditor::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a file...", juce::File{}, "*.wav;*.mp3");

    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                audioProcessor.loadFile(file);
                transportTool.setPlayButtonEnabled(true);
				auto fileName = audioProcessor.getFileName();
                if (fileName != nullptr)
                {
					this->songTitle = *fileName;
                }
                changeTransportState(TransportState::Stopped);

				repaint();
            }
        });
}

void AudioPlayerAudioProcessorEditor::playButtonClicked()
{
    changeTransportState(TransportState::Playing);
}

void AudioPlayerAudioProcessorEditor::pauseButtonClicked()
{
    changeTransportState(TransportState::Paused);
}

void AudioPlayerAudioProcessorEditor::stopButtonClicked()
{
    changeTransportState(TransportState::Stopped);
}

void AudioPlayerAudioProcessorEditor::referenceSwitchButtonClicked()
{
	audioProcessor.isReferenceActive = !audioProcessor.isReferenceActive;

    if (audioProcessor.isReferenceActive)
    {
		transportTool.setReferenceButtonColour(ApplicationColours().primary);
        waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else
    {
		transportTool.setReferenceButtonColour(ApplicationColours().secondary);
        waveformVisualizer.setWaveformColour(ApplicationColours().secondary);
    }

	repaint();
}

void AudioPlayerAudioProcessorEditor::updateButtonStates()
{
	auto state = audioProcessor.getTransportState();

    if (audioProcessor.isFileLoaded())
    {
		transportTool.setPlayButtonEnabled(true);
        waveformVisualizer.repaint();
    }
    else
    {
		transportTool.setPlayButtonEnabled(false);
	}

    switch (state)
    {
    case TransportState::Playing:
        audioProcessor.transportSource.start();
		transportTool.onPlayButtonClicked = [this] { pauseButtonClicked(); };
		transportTool.setStopButtonEnabled(true);
        transportTool.setPlayButtonText("Pause");
        break;
    case TransportState::Paused:
        audioProcessor.transportSource.stop();
		transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
		transportTool.setPlayButtonText("Play");
		transportTool.setStopButtonEnabled(false);
        break;
    case TransportState::Stopped:
        audioProcessor.transportSource.stop();
        audioProcessor.transportSource.setPosition(0.0);
		transportTool.setPlayButtonText("Play");
		transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
		transportTool.setStopButtonEnabled(false);
        break;
    default:
        break;
    }

	auto fileName = audioProcessor.getFileName();
    if (fileName != nullptr)
    {
		this->songTitle = *fileName;
    }

    if (audioProcessor.isReferenceActive)
    {
		transportTool.setReferenceButtonColour(ApplicationColours().primary);
		waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else {
		transportTool.setReferenceButtonColour(ApplicationColours().secondary);
		waveformVisualizer.setWaveformColour(ApplicationColours().secondary);
    }

    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
}

void AudioPlayerAudioProcessorEditor::changeTransportState(TransportState state)
{
	audioProcessor.setTransportState(state);
    updateButtonStates();
	repaint();
}

void AudioPlayerAudioProcessorEditor::timerCallback()
{
    // Update the playhead!!
    waveformVisualizer.setPlayheadTime(audioProcessor.transportSource.getCurrentPosition());
}
