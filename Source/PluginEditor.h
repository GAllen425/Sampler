/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"
#include "SequencerComponent.h"
#include "BasicLookAndFeel.h"
#include "PitchSequencer.h"

using namespace juce;


//==============================================================================
/**
*/
class SamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public Timer
{
public:
    SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Inherited via Timer
    virtual void timerCallback() override;

    SamplerAudioProcessor& audioProcessor;

    TabbedComponent mainComponent;
    WaveThumbnail waveThumbnail;
    ADSRComponent adsrComponent;
    SequencerComponent sequencerComponent;
    PitchSequencer pitchSequencerTab;

    BasicLookAndFeel laf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)


};
