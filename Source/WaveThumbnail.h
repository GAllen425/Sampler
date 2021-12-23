/*
  ==============================================================================

    WaveThumbnail.h
    Created: 16 Nov 2021 4:35:14pm
    Author:  Gareth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "BasicLookAndFeel.h"

#define WAVETHUMBNAIL_NAME "Sampler"
//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       public juce::FileDragAndDropTarget

{
public:
    WaveThumbnail(SamplerAudioProcessor& p);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Inherited via FileDragAndDropTarget
    virtual bool isInterestedInFileDrag(const StringArray& files) override;
    virtual void filesDropped(const StringArray& files, int x, int y) override;

    std::vector<float> mAudioPoints;
    bool mShouldBePaintingWaveForm{ false };
    String mFileName{ "" };

    SamplerAudioProcessor& audioProcessor;
    BasicLookAndFeel laf;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
