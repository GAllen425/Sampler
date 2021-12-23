/*
  ==============================================================================

    SequencerComponent.cpp
    Created: 21 Nov 2021 12:54:53pm
    Author:  Gareth

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerComponent.h"

SequencerComponent::SequencerComponent(SamplerAudioProcessor& p) : sequencer(p), audioProcessor(p)
{
    setLookAndFeel(&laf);
    auto fontSize = 15.0f;

    addAndMakeVisible(numberInSubDivisionSlider);
    numberInSubDivisionSlider.setRange(1.0f, static_cast<double>(MAX_SLIDERS), 1.0f);
    numberInSubDivisionSlider.setValue((double)DEFAULT_SLIDERS);
    numberInSubDivisionSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, COUNT_ID, numberInSubDivisionSlider);
    audioProcessor.apvts.addParameterListener(COUNT_ID, this);
    addAndMakeVisible(numberInSubDivisionLabel);
    numberInSubDivisionLabel.setFont(fontSize);
    numberInSubDivisionLabel.setText("Count", NotificationType::dontSendNotification);
    numberInSubDivisionLabel.setJustificationType(Justification::centredTop);
    numberInSubDivisionLabel.attachToComponent(&numberInSubDivisionSlider, false);

    addAndMakeVisible(subDivisionSlider);
    subDivisionSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, SUBDIVISION_ID, subDivisionSlider);
    subDivisionSlider.textFromValueFunction = [](double val) {
        return Utilities::SubDivisionString[static_cast<int>(val)];
    };
    subDivisionSlider.valueFromTextFunction = [](const String& text) {
        Utilities::SubDivision subDivision;
        std::map<std::string, Utilities::SubDivision>::const_iterator it;
        it = Utilities::subDivisionStringToEnum.find(text.toStdString());

        if (it != Utilities::subDivisionStringToEnum.end())
        {
            subDivision = it->second;
        }
        else
        {
            subDivision = Utilities::SubDivision::QUARTER;
        }
        return static_cast<double> (subDivision);
    };
    subDivisionSlider.setRange(0.0f, (double)Utilities::SubDivision::SIZE - 1.0f, 1.0f);
    audioProcessor.apvts.addParameterListener(SUBDIVISION_ID, this);
    addAndMakeVisible(subDivisionLabel);
    subDivisionLabel.setFont(fontSize);
    subDivisionLabel.setText("Sub-Division", NotificationType::dontSendNotification);
    subDivisionLabel.setJustificationType(Justification::centredTop);
    subDivisionLabel.attachToComponent(&subDivisionSlider, false);

    addAndMakeVisible(sequencer);
}

void SequencerComponent::paint(juce::Graphics& g)
{
    g.fillAll(laf.emptySpace);
}

void SequencerComponent::resized()
{
    auto area = getLocalBounds();
    auto subDivisionSliderBounds = area.removeFromLeft(0.1 * getLocalBounds().getWidth());
    auto heightPerSlider = subDivisionSliderBounds.getHeight() / 2;
    auto labelHeight = heightPerSlider * 0.2;
    auto buffer = 10;

    numberInSubDivisionSlider.setBounds(subDivisionSliderBounds.removeFromTop(heightPerSlider).reduced(buffer, buffer + labelHeight));
    subDivisionSlider.setBounds(subDivisionSliderBounds.removeFromTop(heightPerSlider).reduced(buffer, buffer + labelHeight));
    sequencer.setBounds(area);
}

void SequencerComponent::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == COUNT_ID)
    {
        sequencer.UpdateSequencerSliders((int)newValue);
        audioProcessor.updateSequencerSize();

    }
    else if (parameterID == SUBDIVISION_ID)
    {
        audioProcessor.updateSequencerDivision();
    }
}

//==============================================================================
SequencerSliders::SequencerSliders(SamplerAudioProcessor& p) : audioProcessor(p)
{
    setLookAndFeel(&laf);
    InitialiseSequencerSliders(DEFAULT_SLIDERS);
} 
void SequencerSliders::paint (Graphics& g)
{
    g.fillAll(laf.emptySpace);
}
void SequencerSliders::resized()
{
    auto area = getLocalBounds();

    auto sliderWidth = area.getWidth() / activeSliders;
    for (int i = 0; i < activeSliders; i++)
    {
        sliders[i]->setBounds(area.removeFromLeft(sliderWidth));
    }
}
void SequencerSliders::InitialiseSequencerSliders(int numberSliders)
{
    for (int i = 0; i < MAX_SLIDERS; i++)
    {
        sliders.push_back(std::make_unique<SequencerSlider>());

        std::string tempId = GetPrefix() + std::to_string(i);
        sliderAttachments.push_back(std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, tempId, *sliders[i]));
        audioProcessor.apvts.addParameterListener(tempId, this);
        addAndMakeVisible(*sliders[i]);

        if (i >= numberSliders)
        {
            sliders[i]->setVisible(false);
        }
    }
}
void SequencerSliders::UpdateSequencerSliders(int numberSliders)
{
    for (int i = 0; i < MAX_SLIDERS; i++)
    {
        bool visible = i < numberSliders ? true : false;
        sliders[i]->setVisible(visible);
    }

    activeSliders = numberSliders;
    resized();
}
void SequencerSliders::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID.startsWith(GetPrefix()))
    {
        std::string s = GetPrefix();
        int id = parameterID.substring(s.length(), parameterID.length()).getIntValue();
        audioProcessor.updateSequencerID(id);
    }
}



