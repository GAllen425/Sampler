/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessor::SamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    mFormatManager.registerBasicFormats();
    for (int i = 0; i < numVoices; i++)
    {
        mSampler.addVoice(new SamplerVoice());
    }

}

SamplerAudioProcessor::~SamplerAudioProcessor()
{
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState::ParameterLayout params;

    struct paramDetails {
        std::string name;
        NormalisableRange<float> range;
        float defaultValue;

        paramDetails(std::string _name, NormalisableRange<float> _range, float _defaultValue, float centreValueForSkew = -1.0f)
        {
            name = _name;
            range = _range;
            if (centreValueForSkew >= 0.0f)
                range.setSkewForCentre(centreValueForSkew);
            defaultValue = _defaultValue;
        }
    };
    std::map<std::string, paramDetails> idNameMap
    {
        { ATTACK_ID,  paramDetails(ATTACK_NAME,  NormalisableRange<float>(0.000f, 5.0f, 0.001f), 0.000f, 1.0f) },
        { DECAY_ID,   paramDetails(DECAY_NAME,   NormalisableRange<float>(0.001f, 5.0f, 0.001f), 0.100f, 1.0f) },
        { SUSTAIN_ID, paramDetails(SUSTAIN_NAME, NormalisableRange<float>(0.001f, 1.0f, 0.001f), 1.000f, 0.5f) },
        { RELEASE_ID, paramDetails(RELEASE_NAME, NormalisableRange<float>(0.001f, 5.0f, 0.001f), 0.100f, 1.0f) },
        { COUNT_ID,   paramDetails(COUNT_NAME, NormalisableRange<float>(1.0f, static_cast<double>(MAX_SLIDERS), 1.0f), 8.0f) },
        { SUBDIVISION_ID, paramDetails(SUBDIVISION_NAME, NormalisableRange<float>(0.0f, 6.0f, 1.0f), 0.0f) }
    };

    for (int i = 0; i < MAX_SLIDERS; i++)
    {
        std::string tempId   = SEQUENCER_SLIDER_PREFIX_ID   + std::to_string(i);
        std::string tempName = SEQUENCER_SLIDER_PREFIX_NAME + std::to_string(i);

        idNameMap.insert(std::pair<std::string, paramDetails> (tempId, paramDetails(tempName, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f)));
    }

    std::map<std::string, paramDetails>::iterator it;
    for (it = idNameMap.begin(); it != idNameMap.end(); it++)
    {
        params.add(std::make_unique<AudioParameterFloat>(it->first, (it->second).name, (it->second).range, (it->second).defaultValue));
    }

    return params;
}



const juce::String SamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
    notes.clear();
    currentNote = 0;
    currentSlider = 0;
    lastNoteValue = -1;
    time = 0;
    rate = static_cast<float>(sampleRate);

}

void SamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, numSamples);
    }

    auto noteDuration = Utilities::getNumSamplesToNextNote(rate, 120, sequencerDivision);

    MidiBuffer sequencedMidi;
    MidiMessage m;
    MidiBuffer::Iterator it{ midiMessages };
    int samplePosition;
    while (it.getNextEvent(m, samplePosition))
    {
        if (m.isNoteOn())
        {
            mIsNotePlaying = true;
            mSampleCount = samplePosition;
            time = 0;
            currentSlider = 0;
            m.setVelocity(sequencerAmplitude[currentSlider]);
            notes.add(m.getNoteNumber());
        }
        else if (m.isNoteOff())
        {
            mIsNotePlaying = false;
            releaseTimer = adsrParams.release;
            notes.removeValue(m.getNoteNumber());
        }

        sequencedMidi.addEvent(m, samplePosition);
    }

    midiMessages = sequencedMidi;

    if (mIsNotePlaying)
    {
        mSampleCount += buffer.getNumSamples();
    }
    else if (releaseTimer > 0.0)
    {
            releaseTimer -= (buffer.getNumSamples() / getSampleRate());
            mSampleCount += buffer.getNumSamples();
    }
    else
    {
        mSampleCount = 0;
    }

    if ((time + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));

        if (lastNoteValue > 0)
        {
            midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
            lastNoteValue = -1;
        }

        if (notes.size() > 0)
        {
            currentNote = (currentNote + 1) % notes.size();
            currentSlider = (currentSlider + 1) % sequencerSize;
            lastNoteValue = notes[currentNote];
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)(127 * sequencerAmplitude[currentSlider])), offset);
            mSampleCount = offset;
        }
    }

    // If there are no notes currently held down then the timer should be set to zero until a key is pressed
    if (notes.size() == 0)
    {
        time = 0;
        currentSlider = 0;
    }
    else
    {
        time = (time + numSamples) % noteDuration;
    }
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    midiMessages.clear();
}

//==============================================================================
bool SamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SamplerAudioProcessor::loadFile()
{
    mSampler.clearSounds();
    FileChooser fileChooser{ "Please select a file" };

    if (fileChooser.browseForFileToOpen() )
    {
        auto file = fileChooser.getResult();
        std::unique_ptr<AudioFormatReader> formatReader(mFormatManager.createReaderFor(file));

        ReadSamplesFromFormatReader(formatReader);
    }

    updateADSR();
}

void SamplerAudioProcessor::loadFile(const String& fileName)
{
    mSampler.clearSounds();
    auto file = File(fileName);
    std::unique_ptr<AudioFormatReader> formatReader(mFormatManager.createReaderFor(file));

    ReadSamplesFromFormatReader(formatReader);

}

void SamplerAudioProcessor::ReadSamplesFromFormatReader(std::unique_ptr<AudioFormatReader>& formatReader)
{
    if (formatReader)
    {
        auto sampleLength = static_cast<int>(formatReader->lengthInSamples);
        mWaveForm.setSize(1, sampleLength);
        formatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);
        

        BigInteger midiRange;
        midiRange.setRange(0, 128, true);
        mSampler.addSound(new SamplerSound("Sample", *formatReader, midiRange, 60, 0.0, 0.1, 10));
    }

    updateADSR();
}

void SamplerAudioProcessor::updateADSR()
{
    adsrParams.attack  = apvts.getRawParameterValue(ATTACK_ID)->load();
    adsrParams.decay   = apvts.getRawParameterValue(DECAY_ID)->load();
    adsrParams.sustain = apvts.getRawParameterValue(SUSTAIN_ID)->load();
    adsrParams.release = apvts.getRawParameterValue(RELEASE_ID)->load();

    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(adsrParams);
        }
    }
}

void SamplerAudioProcessor::updateSequencerID(const int id)
{
    std::string tempId = SEQUENCER_SLIDER_PREFIX_ID + std::to_string(id);
    
    sequencerAmplitude[id] = apvts.getRawParameterValue(tempId)->load();
}
void SamplerAudioProcessor::updateSequencerSize()
{
    sequencerSize = static_cast<int>(apvts.getRawParameterValue(COUNT_ID)->load());
}
void SamplerAudioProcessor::updateSequencerDivision()
{
    sequencerDivision = static_cast<Utilities::SubDivision>(apvts.getRawParameterValue(SUBDIVISION_ID)->load());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
