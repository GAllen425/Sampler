/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 16 Nov 2021 6:08:52pm
    Author:  Gareth

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(SamplerAudioProcessor& p) : audioProcessor(p)
{
    setLookAndFeel(&laf);
    int textBoxHeight = 25;
    int textBoxWidth = 70;
    float fontSize = 15.0f;
    // ATTACK
    mAttackSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, ATTACK_ID, mAttackSlider);
    mAttackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
    mAttackSlider.setRange(0.000f, 5.0f, 0.001f);
    mAttackSlider.setLookAndFeel(&laf);
    addAndMakeVisible(mAttackSlider);

    mAttackLabel.setFont(fontSize);
    mAttackLabel.setText("Attack", NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);

    // DECAY
    mDecaySliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, DECAY_ID, mDecaySlider);
    mDecaySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
    mDecaySlider.setRange(0.001f, 5.0f, 0.001f);
    mDecaySlider.setLookAndFeel(&laf);
    addAndMakeVisible(mDecaySlider);

    mDecayLabel.setFont(fontSize);
    mDecayLabel.setText("Decay", NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

    // SUSTAIN
    mSustainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, SUSTAIN_ID, mSustainSlider);
    mSustainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
    mSustainSlider.setRange(0.001f, 1.0f, 0.001f);
    mSustainSlider.setLookAndFeel(&laf);
    addAndMakeVisible(mSustainSlider);

    mSustainLabel.setFont(fontSize);
    mSustainLabel.setText("Sustain", NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);

    // RELEASE
    mReleaseSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, RELEASE_ID, mReleaseSlider);
    mReleaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, textBoxWidth, textBoxHeight);
    mReleaseSlider.setRange(0.001f, 5.0f, 0.001f);
    mReleaseSlider.setLookAndFeel(&laf);
    addAndMakeVisible(mReleaseSlider);

    mReleaseLabel.setFont(fontSize);
    mReleaseLabel.setText("Release", NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);

    audioProcessor.apvts.addParameterListener(ATTACK_ID, this);
    audioProcessor.apvts.addParameterListener(DECAY_ID, this);
    audioProcessor.apvts.addParameterListener(SUSTAIN_ID, this);
    audioProcessor.apvts.addParameterListener(RELEASE_ID, this);
}

ADSRComponent::~ADSRComponent()
{
    setLookAndFeel(nullptr);
    mAttackSlider.setLookAndFeel(nullptr);
    mDecaySlider.setLookAndFeel(nullptr);
    mSustainSlider.setLookAndFeel(nullptr);
    mReleaseSlider.setLookAndFeel(nullptr);
}

void ADSRComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colours::white);
}

void ADSRComponent::resized()
{
    auto bounds = getLocalBounds();
    auto width = getWidth();
    auto height = getHeight();
    auto labelHeight = height * 0.2;
    auto buffer = 10;
    auto sizePerSlider = (1.0f / NUM_DIALS) * width;

    mAttackSlider. setBounds(bounds.removeFromLeft(sizePerSlider).reduced(buffer, labelHeight + buffer));
    mDecaySlider.  setBounds(bounds.removeFromLeft(sizePerSlider).reduced(buffer, labelHeight + buffer));
    mSustainSlider.setBounds(bounds.removeFromLeft(sizePerSlider).reduced(buffer, labelHeight + buffer));
    mReleaseSlider.setBounds(bounds.removeFromLeft(sizePerSlider).reduced(buffer, labelHeight + buffer));
}

void ADSRComponent::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == ATTACK_ID || parameterID == DECAY_ID ||
        parameterID == SUSTAIN_ID || parameterID == RELEASE_ID)
    {
        audioProcessor.updateADSR();
    }
}