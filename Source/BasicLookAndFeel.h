/*
  ==============================================================================

    BasicLookAndFeel.h
    Created: 21 Nov 2021 4:55:00pm
    Author:  Gareth

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

class BasicLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BasicLookAndFeel() { 
        setColour(juce::Slider::thumbColourId, highlight);
        setColour(juce::Slider::trackColourId, background);
        setColour(juce::Slider::backgroundColourId, emptySpace);
        setColour(juce::Slider::textBoxBackgroundColourId, emptySpace);
        setColour(juce::Slider::textBoxOutlineColourId, emptySpace);
        setColour(juce::Slider::textBoxTextColourId, textOnEmpty);
        setColour(juce::TabbedButtonBar::ColourIds::frontOutlineColourId, highlight);
        setColour(juce::TabbedButtonBar::ColourIds::frontTextColourId, textOnBackground);
        setColour(juce::TabbedButtonBar::ColourIds::tabOutlineColourId, background.darker());
        setColour(juce::TabbedButtonBar::ColourIds::tabTextColourId, textOnBackground.darker());
        setColour(juce::Label::ColourIds::textColourId, textOnEmpty);
        setColour(juce::Label::ColourIds::backgroundColourId, emptySpace);
        setColour(juce::Label::ColourIds::outlineColourId, emptySpace);
        setColour(juce::Label::ColourIds::textWhenEditingColourId, textOnBackground);
        setColour(juce::Label::ColourIds::backgroundWhenEditingColourId, background);
        setColour(juce::Label::ColourIds::outlineWhenEditingColourId, background);

    }
    const juce::Colour background = juce::Colour::fromRGB(19,51,55).darker(0.8f);
    const juce::Colour highlight = juce::Colours::turquoise;
    const juce::Colour emptySpace = juce::Colours::white;
    const juce::Colour textOnBackground = juce::Colours::white;
    const juce::Colour textOnEmpty = juce::Colour::fromRGB(19, 51, 55).darker(0.8f);
    const float transparency = 0.25f;
    const int border = 10;

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle, juce::Slider& slider) override
    {
        // Background
        g.fillAll(emptySpace);
        auto bounds = slider.getLocalBounds().reduced(5, 0);
        g.setColour(background);
        g.fillRect(bounds);

        // Filled
        auto filledBounds = bounds.removeFromBottom((float)height - sliderPos);
        g.setColour(highlight);
        g.fillRect(filledBounds);
    }
};