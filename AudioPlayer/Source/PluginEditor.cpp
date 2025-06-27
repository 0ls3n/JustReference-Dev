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
	addAndMakeVisible(filterTool);

	transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
	transportTool.onStopButtonClicked = [this] { stopButtonClicked(); };
	transportTool.onReferenceButtonClicked = [this] { referenceSwitchButtonClicked(); };
	transportTool.onOpenButtonClicked = [this] { openButtonClicked(); };

    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(25.0f, juce::Font::bold));

	addAndMakeVisible(waveformVisualizer);
    waveformVisualizer.onSeek = [this](double position) {
        audioProcessor.transportSource.setPosition(position);
        repaint();
		};

    waveformVisualizer.onFileDropped = [this](const juce::StringArray& files, int x, int y)
        {
            if (files.size() > 0)
            {
                juce::File file(files[0]);
                if (file.existsAsFile())
                {
                    audioProcessor.loadFile(file);
                    transportTool.setButtonEnabled(TransportToolComponent::ButtonId::PlayButton, true);
                    auto fileName = audioProcessor.getFileName();
                    if (fileName != nullptr)
                    {
                        this->songTitle = *fileName;
                    }
                    changeTransportState(TransportState::Stopped);

                    repaint();
                }
            }
        };

    waveformVisualizer.onComponentClicked = [this]()
        {
            openButtonClicked();
        };

    addAndMakeVisible(filterToggleButton);
    filterToggleButton.setButtonText("Filters");
	filterToggleButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);

    filterToggleButton.onClick = [this]() {
        if (filterIsAnimating)
            return;

        filterIsVisible = !filterIsVisible;
        filterStartHeight = filterCurrentHeight;
        filterTargetHeight = filterIsVisible ? 60 : 0;

        filterAnimationElapsed = 0;
        filterIsAnimating = true;
        };

    startTimerHz(60);

	chooser.reset();

    updateButtonStates();
}

AudioPlayerAudioProcessorEditor::~AudioPlayerAudioProcessorEditor() {  }

//==============================================================================
void AudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(ApplicationColours().background);
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
	auto transportArea = area.removeFromBottom(headerAndFooterHeight);
	auto filterArea = area.removeFromBottom(filterCurrentHeight);
    auto filterToggleButtonArea = area.removeFromBottom(20);
    auto leftSidebarArea = area.removeFromLeft(sideBarWidth);
    auto rightSidebarArea = area.removeFromRight(sideBarWidth);

	brandingHeader.setBounds(headerArea);
    songTitleLabel.setBounds(area.removeFromTop(60));
	waveformVisualizer.setBounds(area.removeFromTop(area.getHeight() - headerAndFooterHeight).reduced(20, 0));
	filterTool.setBounds(filterArea);
    transportTool.setBounds(transportArea);
    filterToggleButton.setBounds(getWidth() / 2 - (buttonWidth / 2), filterToggleButtonArea.getY(), buttonWidth, 20);
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
                transportTool.setButtonEnabled(TransportToolComponent::ButtonId::PlayButton, true);
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
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours().primary);
        waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else
    {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours().secondary);
        waveformVisualizer.setWaveformColour(ApplicationColours().secondary);
    }

	repaint();
}

void AudioPlayerAudioProcessorEditor::updateButtonStates()
{
	auto state = audioProcessor.getTransportState();

    if (audioProcessor.isFileLoaded())
    {
        transportTool.setButtonEnabled(TransportToolComponent::ButtonId::PlayButton, true);
        waveformVisualizer.repaint();
    }
    else
    {
        transportTool.setButtonEnabled(TransportToolComponent::ButtonId::PlayButton, false);
	}

    switch (state)
    {
    case TransportState::Playing:
        audioProcessor.transportSource.start();
		transportTool.onPlayButtonClicked = [this] { pauseButtonClicked(); };
        transportTool.setButtonEnabled(TransportToolComponent::ButtonId::StopButton, true);
		transportTool.setButtonText(TransportToolComponent::ButtonId::PlayButton, "Pause");
        break;
    case TransportState::Paused:
        audioProcessor.transportSource.stop();
		transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
		transportTool.setButtonText(TransportToolComponent::ButtonId::PlayButton, "Play");
		transportTool.setButtonEnabled(TransportToolComponent::ButtonId::StopButton, false);
        break;
    case TransportState::Stopped:
        audioProcessor.transportSource.stop();
        audioProcessor.transportSource.setPosition(0.0);
		transportTool.setButtonText(TransportToolComponent::ButtonId::PlayButton, "Play");
		transportTool.onPlayButtonClicked = [this] { playButtonClicked(); };
		transportTool.setButtonEnabled(TransportToolComponent::ButtonId::StopButton, false);
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
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours().primary);
		waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else {
		transportTool.setButtonColour(TransportToolComponent::ButtonId::ReferenceButton, ApplicationColours().secondary);
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

    if (filterIsAnimating)
    {
        filterAnimationElapsed += 1000 / 60;

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
