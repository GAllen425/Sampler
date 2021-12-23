/*
  ==============================================================================

    ADSRComponent.h
    Created: 16 Nov 2021 6:08:52pm
    Author:  Gareth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "BasicLookAndFeel.h"


#define ATTACK_ID "attack"
#define ATTACK_NAME "Attack"
#define DECAY_ID "decay"
#define DECAY_NAME "Decay"
#define SUSTAIN_ID "sustain"
#define SUSTAIN_NAME "Sustain"
#define RELEASE_ID "release"
#define RELEASE_NAME "Release"
#define NUM_DIALS 4
//==============================================================================
/*
*/
class ADSRComponent  : public Component,
                       public AudioProcessorValueTreeState::Listener
{
public:
    ADSRComponent(SamplerAudioProcessor& p);
    ~ADSRComponent() override;

    void paint (Graphics&) override;
    void resized() override;


private:
    // Inherited via Listener
    virtual void parameterChanged(const String& parameterID, float newValue) override;

    Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttackSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mDecaySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mSustainSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mReleaseSliderAttachment;

    SamplerAudioProcessor& audioProcessor;
    BasicLookAndFeel laf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
