#pragma once
#include <JuceHeader.h>

enum class SoloFilterType
{
    Sub,
    LowMid,
    HighMid,
    High,
    NoSolo
};

class SoloFilterProcessing
{
public:
    SoloFilterProcessing(float _lowFreq, float _lowMidFreq, float _highMidFreq, float _highFreq);
    ~SoloFilterProcessing();

    void prepareToPlay(double sampleRate, int samplesPerBlock);

	void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    SoloFilterType& getSoloFilterType() { return soloFilterType; }

private:

	using Filter = juce::dsp::IIR::Filter<float>;

    using MonoChain = juce::dsp::ProcessorChain<
        Filter, 
        Filter, 
        Filter,
        Filter>;

	MonoChain leftChain, rightChain;

    float lowFreq;
    float lowMidFreq;
	float highMidFreq;
    float highFreq;

    SoloFilterType soloFilterType = SoloFilterType::NoSolo;

};
