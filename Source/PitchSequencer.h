/*
  ==============================================================================

    LevelsComponent.h
    Created: 20 Dec 2021 10:04:57pm
    Author:  Gareth

  ==============================================================================
*/

#include  "SequencerComponent.h"

#pragma once
class PitchSequencer : public SequencerSliders
{
public:
    PitchSequencer(SamplerAudioProcessor& p);
    ~PitchSequencer();

private:

    virtual std::string GetPrefix() override { return PITCH_SLIDER_PREFIX_ID; }
    BasicLookAndFeel laf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchSequencer)
};