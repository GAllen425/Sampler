/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), waveThumbnail(p), adsrComponent(p),
    sequencerComponent(p), pitchSequencerTab(p), mainComponent(juce::TabbedButtonBar::TabsAtTop)
{
    setLookAndFeel(&laf);

    mainComponent.addTab(WAVETHUMBNAIL_NAME, laf.background, &waveThumbnail, false, 0);
    mainComponent.addTab(SEQUENCER_NAME, laf.background, &sequencerComponent, false, 1);
    mainComponent.addTab(PITCH_SLIDER_NAME, laf.background, &pitchSequencerTab, false, 2);
    addAndMakeVisible(mainComponent);
    addAndMakeVisible(adsrComponent);
    sequencerComponent.setVisible(false);

    startTimerHz(30);
    setSize (1200, 800);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(laf.emptySpace);
}

void SamplerAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    mainComponent.setBounds(bounds.removeFromTop(getHeight() * 0.75));
    adsrComponent.setBounds(bounds);
}

void SamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}



