/*
  ==============================================================================

    SequencerComponent.h
    Created: 21 Nov 2021 12:54:53pm
    Author:  Gareth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "PluginProcessor.h"
#include "BasicLookAndFeel.h"
#include "Utilities.h"
using namespace juce;

#define DEFAULT_SLIDERS 8

class SequencerSlider : public Slider
{
public:
    SequencerSlider()
    {
        this->setLookAndFeel(&laf);
        this->setSliderStyle(Slider::LinearBarVertical);
        this->setRange(0.0f, 1.0f, 0.01f);
        this->setTextBoxIsEditable(false);
        this->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    }

    // LookAndFeel throws an assertion if it is deleted while another object is pointing to it
    ~SequencerSlider() { this->setLookAndFeel(nullptr); }
private:
    BasicLookAndFeel laf;
};

class SequencerRotarySlider : public Slider
{
public:
    SequencerRotarySlider()
    {
        this->setLookAndFeel(&laf);
        this->setSliderStyle(Slider::RotaryVerticalDrag);
        this->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 40, 20);
    }
    
    // LookAndFeel throws an assertion if it is deleted while another object is pointing to it
    ~SequencerRotarySlider() { this->setLookAndFeel(nullptr); }
private:
    BasicLookAndFeel laf;
};

class SequencerSliders : public Component,
                         public AudioProcessorValueTreeState::Listener
{
public:
    SequencerSliders(SamplerAudioProcessor& p);
    virtual ~SequencerSliders() { this->setLookAndFeel(nullptr); }

    void paint(juce::Graphics&) override;
    void resized() override;

    void InitialiseSequencerSliders(int numberSliders);
    void UpdateSequencerSliders(int numberSliders);

private:

    // Inherited via AudioProcessorValueTreeState::Listener
    virtual void parameterChanged(const String& parameterID, float newValue) override;

    std::vector<std::unique_ptr<Slider>> sliders;
    int activeSliders = DEFAULT_SLIDERS;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;

    BasicLookAndFeel laf;
    SamplerAudioProcessor& audioProcessor;
    virtual std::string GetPrefix() { return SEQUENCER_SLIDER_PREFIX_ID; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencerSliders)
};

class SequencerComponent : public Component,
                           public AudioProcessorValueTreeState::Listener
{
public:
    SequencerComponent(SamplerAudioProcessor& p);
    ~SequencerComponent() { this->setLookAndFeel(nullptr); }

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Inherited via AudioProcessorValueTreeState::Listener
    virtual void parameterChanged(const String& parameterID, float newValue) override;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> subDivisionSliderAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> numberInSubDivisionSliderAttachment;

    Utilities::SubDivision currentSubdivision = Utilities::SubDivision::EIGTH;
    SequencerRotarySlider subDivisionSlider, numberInSubDivisionSlider;
    Label subDivisionLabel, numberInSubDivisionLabel;

    BasicLookAndFeel laf;
    SamplerAudioProcessor& audioProcessor;

    SequencerSliders sequencer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencerComponent)
};

