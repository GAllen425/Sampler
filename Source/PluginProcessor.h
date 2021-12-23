/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"

// Slider IDs and Names for Sequencer class
#define SEQUENCER_NAME "Sequencer"
#define SUBDIVISION_ID "sub-division"
#define SUBDIVISION_NAME "Sub-Division"
#define COUNT_ID   "count"
#define COUNT_NAME "Count"
#define SEQUENCER_SLIDER_PREFIX_ID "sequencer slider "
#define SEQUENCER_SLIDER_PREFIX_NAME "Sequencer Slider "
#define PITCH_SLIDER_NAME "Pitch"
#define PITCH_SLIDER_PREFIX_ID "pitch slider "
#define PITCH_SLIDER_PREFIX_NAME "Pitch Slider "
#define MAX_SLIDERS 16

using namespace juce;

//==============================================================================
/**
*/
class SamplerAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void loadFile();
    void loadFile(const String& fileName);
    void ReadSamplesFromFormatReader(std::unique_ptr<AudioFormatReader>& formatReader);

    int getNumSamplerSounds() { return mSampler.getNumSounds(); }

    const AudioBuffer<float>& getWaveForm() { return mWaveForm; }

    void updateADSR();

    void updateSequencerID(const int id);

    void updateSequencerSize();
    void updateSequencerDivision();

    std::atomic<int>& getSampleCount() { return mSampleCount; }
    
    AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    Synthesiser mSampler;
    const int numVoices{ 3 };
    AudioBuffer<float> mWaveForm;

    AudioFormatManager mFormatManager;
    ADSR::Parameters adsrParams;
    int sequencerSize = 8;
    double sequencerAmplitude[MAX_SLIDERS] = { 0.0 };
    Utilities::SubDivision sequencerDivision;
    std::atomic<bool> mIsNotePlaying{ false };
    std::atomic<int> mSampleCount{ 0 };
    double releaseTimer{ -1.0 };

    // Sequencer Variables
    int currentNote, lastNoteValue, currentSlider;
    int time;
    float rate;
    juce::SortedSet<int> notes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)


};
