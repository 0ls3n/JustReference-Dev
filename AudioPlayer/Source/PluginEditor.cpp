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
    setSize (700, 500);

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

    addAndMakeVisible(&songTitleLabel);
    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);
    songTitleLabel.setJustificationType(juce::Justification::centred);
    songTitleLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    songTitleLabel.setSize(getWidth(), 50);

    formatManager.registerBasicFormats();

    thumbnail.addChangeListener(this);

	chooser.reset();

    updateButtonStates();
}

AudioPlayerAudioProcessorEditor::~AudioPlayerAudioProcessorEditor() { thumbnail.removeChangeListener(this); }

//==============================================================================
void AudioPlayerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(18, 18, 18));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

    juce::ColourGradient gradient(juce::Colours::cyan, 0, 0,
        juce::Colours::lightblue, getWidth(), 0, false);

    

    if (thumbnail.getTotalLength() > 0.0)
    {

        g.setColour(juce::Colours::aqua);
        auto bounds = juce::Rectangle<int>(10, 260, getWidth() - 20, 200);

        // Draw waveform using built-in thumbnail renderer
        thumbnail.drawChannels(g, bounds, 0.0, thumbnail.getTotalLength(), .7f);
    }
    else
    {
        g.setFont(15.0f);
        g.drawFittedText("No waveform loaded", 10, 240, getWidth() - 20, 50, juce::Justification::centred, 1);
    }
   
}

void AudioPlayerAudioProcessorEditor::resized()
{
    openButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    pauseButton.setBounds(10, 90, getWidth() - 20, 30);
    stopButton.setBounds(10, 130, getWidth() - 20, 30);
    songTitleLabel.setBounds(0, 160, getWidth(), 50);
	referenceSwitchButton.setBounds(10, 210, getWidth() - 20, 30);
}

void AudioPlayerAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
        repaint(); // redraw waveform when updated
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

                thumbnail.clear();
                thumbnail.setSource(new juce::FileInputSource(file));

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

    if (audioProcessor.isReferenceActive)
    {
        referenceSwitchButton.setButtonText("A/B (Active)");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    }
    else {
        referenceSwitchButton.setButtonText("A/B (Inactive)");
        referenceSwitchButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightblue);
    }

    songTitleLabel.setText(songTitle, juce::NotificationType::dontSendNotification);


}

void AudioPlayerAudioProcessorEditor::changeTransportState(TransportState state)
{
	audioProcessor.setTransportState(state);
    updateButtonStates();
	repaint();
}
