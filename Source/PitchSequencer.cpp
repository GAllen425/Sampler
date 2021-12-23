/*
  ==============================================================================

    LevelsComponent.cpp
    Created: 20 Dec 2021 10:04:58pm
    Author:  Gareth

  ==============================================================================
*/

#include "PitchSequencer.h"

PitchSequencer::PitchSequencer(SamplerAudioProcessor& p) : SequencerSliders(p)
{

}

PitchSequencer::~PitchSequencer()
{
    this->setLookAndFeel(nullptr);
}



