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

    addAndMakeVisible(header);
    addAndMakeVisible(footer);

    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open file");
    openButton.onClick = [this] { openButtonClicked(); };
    openButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton); 
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    stopButton.setEnabled(false);

    addAndMakeVisible(&referenceSwitchButton);
    referenceSwitchButton.setButtonText("Reference");
    referenceSwitchButton.onClick = [this] { referenceSwitchButtonClicked(); };
    referenceSwitchButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
    referenceSwitchButton.setEnabled(true);

    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(25.0f, juce::Font::bold));
    songTitleLabel.setSize(getWidth(), 60);

    addAndMakeVisible(pluginNameLabel);
	pluginNameLabel.setText("AB Reference", juce::NotificationType::dontSendNotification);
	pluginNameLabel.setJustificationType(juce::Justification::centredLeft);
	pluginNameLabel.setFont(juce::Font(20.0f, juce::Font::bold));
	pluginNameLabel.setSize(200, 60);

    addAndMakeVisible(brandNameLabel);
	brandNameLabel.setText("JustMixing", juce::NotificationType::dontSendNotification);
	brandNameLabel.setJustificationType(juce::Justification::centredRight);
	brandNameLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    brandNameLabel.setSize(200, 60);

	addAndMakeVisible(waveformVisualizer);

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
    g.setFont (15.0f);
   
}

void AudioPlayerAudioProcessorEditor::resized()
{
    auto headerAndFooterHeight = 60;
	auto sidebarWidth = 80;
    auto buttonWidth = 100;
    auto buttonMargin = 10;
    auto area = getLocalBounds();

	auto headerArea = area.removeFromTop(headerAndFooterHeight);
	auto footerArea = area.removeFromBottom(headerAndFooterHeight);

	header.setBounds(headerArea);
    footer.setBounds(footerArea);

    pluginNameLabel.setBounds(headerArea.removeFromLeft(200).reduced(0));
	brandNameLabel.setBounds(headerArea.removeFromRight(200).reduced(0));
    songTitleLabel.setBounds(area.removeFromTop(60).reduced(0));


	waveformVisualizer.setBounds(area.removeFromTop(area.getHeight() - headerAndFooterHeight).reduced(0, 20));

	openButton.setBounds(footerArea.removeFromLeft(buttonWidth).reduced(buttonMargin));
    referenceSwitchButton.setBounds(footerArea.removeFromLeft(buttonWidth).reduced(buttonMargin));
    playButton.setBounds(footerArea.removeFromLeft(buttonWidth).reduced(buttonMargin));
	stopButton.setBounds(footerArea.removeFromLeft(buttonWidth).reduced(buttonMargin));

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
                playButton.setEnabled(true);
				auto fileName = audioProcessor.getFileName();
                if (fileName != nullptr)
                {
					this->songTitle = *fileName;
                }
                changeTransportState(TransportState::Stopped);

				waveformVisualizer.setAudioFile(file);

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
        referenceSwitchButton.setButtonText("Reference");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().primary);
        waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else
    {
        referenceSwitchButton.setButtonText("Reference");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
        waveformVisualizer.setWaveformColour(ApplicationColours().secondary);
    }

	repaint();
}

void AudioPlayerAudioProcessorEditor::updateButtonStates()
{
	auto state = audioProcessor.getTransportState();

    if (audioProcessor.isFileLoaded())
    {
        playButton.setEnabled(true);
		waveformVisualizer.setAudioFile(audioProcessor.getCurrentFile());
    }
    else
    {
        playButton.setEnabled(false);
	}

    switch (state)
    {
    case TransportState::Playing:
        audioProcessor.transportSource.start();
        playButton.onClick = [this] { pauseButtonClicked(); };
		stopButton.setEnabled(true);
		playButton.setButtonText("Pause");
        break;
    case TransportState::Paused:
        audioProcessor.transportSource.stop();
		playButton.onClick = [this] { playButtonClicked(); };
		playButton.setButtonText("Play");
		stopButton.setEnabled(false);
        break;
    case TransportState::Stopped:
        audioProcessor.transportSource.stop();
        audioProcessor.transportSource.setPosition(0.0);
		playButton.setButtonText("Play");
		playButton.onClick = [this] { playButtonClicked(); };
		stopButton.setEnabled(false);
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
        referenceSwitchButton.setButtonText("Reference");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().primary);
		waveformVisualizer.setWaveformColour(ApplicationColours().primary);
    }
    else {
        referenceSwitchButton.setButtonText("Reference");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, ApplicationColours().secondary);
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
