/*
  ==============================================================================

    Utilities.h
    Created: 28 Nov 2021 10:49:10am
    Author:  Gareth

  ==============================================================================
*/
#include <string>
#include <map>
#include <cmath>

#pragma once
namespace Utilities
{
    enum SubDivision {
        QUARTER,
        QUARTER_TRIPLET,
        EIGTH,
        EIGTH_TRIPLET,
        SIXTEENTH,
        SIXTEENTH_TRIPLET,
        THIRTY_SECOND,
        SIZE
    };

    const std::string SubDivisionString[] = {
        "1/4", "1/4T", "1/8", "1/8T", "1/16", "1/16T", "1/32"
    };

    const std::map<std::string, SubDivision> subDivisionStringToEnum = {
        { SubDivisionString[SubDivision::QUARTER],           SubDivision::QUARTER },
        { SubDivisionString[SubDivision::QUARTER_TRIPLET],   SubDivision::QUARTER_TRIPLET },
        { SubDivisionString[SubDivision::EIGTH],             SubDivision::EIGTH },
        { SubDivisionString[SubDivision::EIGTH_TRIPLET],     SubDivision::EIGTH_TRIPLET },
        { SubDivisionString[SubDivision::SIXTEENTH],         SubDivision::SIXTEENTH },
        { SubDivisionString[SubDivision::SIXTEENTH_TRIPLET], SubDivision::SIXTEENTH_TRIPLET },
        { SubDivisionString[SubDivision::THIRTY_SECOND],     SubDivision::THIRTY_SECOND }
    };

    float ConvertSubDivisionToDecimals(SubDivision subDivision);
    int getNumSamplesToNextNote(float sampleRate, int BPM, SubDivision subDivision);
};


