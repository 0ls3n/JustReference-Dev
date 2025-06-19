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
    setSize (300, 300);

	setResizable(true, true);

    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };
    
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);
    

    addAndMakeVisible(&pauseButton);
    pauseButton.setButtonText("Pause");
    pauseButton.onClick = [this] { pauseButtonClicked(); };
    pauseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
    pauseButton.setEnabled(false);

    addAndMakeVisible(&stopButton); 
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(&referenceSwitchButton);
    referenceSwitchButton.setButtonText("A/B (Inactive)");
    referenceSwitchButton.onClick = [this] { referenceSwitchButtonClicked(); };
    referenceSwitchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightblue);
    referenceSwitchButton.setEnabled(true);

	chooser.reset();

    updateButtonStates();
}

AudioPlayerAudioProcessorEditor::~AudioPlayerAudioProcessorEditor() { }

//==============================================================================
void AudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawFittedText(songTitle, 0, 160, getWidth(), 30, juce::Justification::centred, 1);
   
}

void AudioPlayerAudioProcessorEditor::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    pauseButton.setBounds(10, 90, getWidth() - 20, 30);
    stopButton.setBounds(10, 130, getWidth() - 20, 30);
	referenceSwitchButton.setBounds(10, 200, getWidth() - 20, 30);
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
        referenceSwitchButton.setButtonText("A/B (Active)");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    }
    else
    {
        referenceSwitchButton.setButtonText("A/B (Inactive)");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightblue);
    }

	repaint();
}

void AudioPlayerAudioProcessorEditor::updateButtonStates()
{
	auto state = audioProcessor.getTransportState();

    switch (state)
    {
    case TransportState::Playing:
        audioProcessor.transportSource.start();
		playButton.setEnabled(false);
		pauseButton.setEnabled(true);
		stopButton.setEnabled(true);
		playButton.setButtonText("Playing...");
        break;
    case TransportState::Paused:
        audioProcessor.transportSource.stop();
		playButton.setEnabled(true);
		pauseButton.setEnabled(false);
		stopButton.setEnabled(false);
		playButton.setButtonText("Paused...");
        break;
    case TransportState::Stopped:
        audioProcessor.transportSource.stop();
        audioProcessor.transportSource.setPosition(0.0);
		playButton.setEnabled(true);
		pauseButton.setEnabled(false);
		stopButton.setEnabled(false);
		playButton.setButtonText("Play");
        break;
    default:
        break;
    }

	auto fileName = audioProcessor.getFileName();
    if (fileName != nullptr)
    {
		this->songTitle = *fileName;
    }


}

void AudioPlayerAudioProcessorEditor::changeTransportState(TransportState state)
{
	audioProcessor.setTransportState(state);
    updateButtonStates();
	repaint();
}
