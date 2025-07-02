#include "SoloFilterProcessing.h"

SoloFilterProcessing::SoloFilterProcessing(float _lowFreq, float _lowMidFreq, float _highMidFreq, float _highFreq) : 
	lowFreq(_lowFreq), lowMidFreq(_lowMidFreq), highMidFreq(_highMidFreq), highFreq(_highFreq)
{

}

SoloFilterProcessing::~SoloFilterProcessing()
{
}

void SoloFilterProcessing::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	juce::dsp::ProcessSpec spec;

	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = 2;

	leftChain.prepare(spec);
	rightChain.prepare(spec);

	auto subCoeffecient = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, lowFreq, 8.0f);
	auto lowMidCoeffecient = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, lowMidFreq, 6.0f);
	auto highMidCoeffecient = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, highMidFreq, 7.0f);
	auto highCoeffecient = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, highFreq, 9.0f);

	leftChain.get<0>().coefficients = subCoeffecient;
	rightChain.get<0>().coefficients = subCoeffecient;

	leftChain.get<1>().coefficients = lowMidCoeffecient;
	rightChain.get<1>().coefficients = lowMidCoeffecient;

	leftChain.get<2>().coefficients = highMidCoeffecient;
	rightChain.get<2>().coefficients = highMidCoeffecient;

	leftChain.get<3>().coefficients = highCoeffecient;
	rightChain.get<3>().coefficients = highCoeffecient;
}

void SoloFilterProcessing::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    switch (soloFilterType)
    {
    case SoloFilterType::Sub:
        leftChain.get<0>().process(leftContext);
        rightChain.get<0>().process(rightContext);
        break;
    case SoloFilterType::LowMid:
        leftChain.get<1>().process(leftContext);
        rightChain.get<1>().process(rightContext);
        break;
    case SoloFilterType::HighMid:
        leftChain.get<2>().process(leftContext);
        rightChain.get<2>().process(rightContext);
        break;
    case SoloFilterType::High:
        leftChain.get<3>().process(leftContext);
        rightChain.get<3>().process(rightContext);
        break;
    case SoloFilterType::NoSolo:
        // No filter applied, just pass through the audio
        break;
    default:
        break;
    }
}