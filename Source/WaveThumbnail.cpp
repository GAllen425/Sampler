/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 16 Nov 2021 4:35:14pm
    Author:  Gareth

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(SamplerAudioProcessor& p) : audioProcessor(p)
{
}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(laf.emptySpace);
    g.setColour(laf.background);
    g.fillRect(getLocalBounds().reduced(laf.border));    

    auto waveform = audioProcessor.getWaveForm();
    if (waveform.getNumSamples() > 0)
    {
        // WAVEFORM
        Path p;
        mAudioPoints.clear();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);

        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }

        p.startNewSubPath(0, getHeight() / 2);

        for (int x = 0; x < mAudioPoints.size(); ++x)
        {
            auto y = jmap<float>(mAudioPoints[x], -1.0f, 1.0f, getHeight(), 0);
            p.lineTo(x, y);
        }

        g.setColour(laf.textOnBackground);
        g.strokePath(p, PathStrokeType(2));


        // FILE NAME
        g.setColour(laf.textOnBackground);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(laf.border);
        g.drawFittedText(mFileName, textBounds, Justification::topRight, 1);


        auto playHeadPosition = jmap<int>(audioProcessor.getSampleCount(), 0, audioProcessor.getWaveForm().getNumSamples(), 0, getWidth());
        g.setColour(laf.textOnBackground);
        g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);

        g.setColour(laf.background.withAlpha(laf.transparency));
        g.fillRect(0, 0, playHeadPosition, getHeight());
    }
    else
    {
        g.setColour(laf.textOnBackground);
        g.setFont(40.0f);
        g.drawFittedText("Drop an audio file to load", getLocalBounds(), Justification::centred, 1);
    }
}

void WaveThumbnail::resized()
{
}

bool WaveThumbnail::isInterestedInFileDrag(const StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains("wav") || file.contains("aif") || file.contains("mp3"))
        {
            return true;
        }
    }
    return false;
}

void WaveThumbnail::filesDropped(const StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            auto myFile = std::make_unique<File>(file);
            mFileName = myFile->getFileNameWithoutExtension();

            audioProcessor.loadFile(file);
        }
    }

    repaint();
}

