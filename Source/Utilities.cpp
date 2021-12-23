/*
  ==============================================================================

    Utilities.cpp
    Created: 9 Dec 2021 8:50:58pm
    Author:  Gareth

  ==============================================================================
*/
#include "Utilities.h"

int Utilities::getNumSamplesToNextNote(float sampleRate, int BPM, SubDivision subDivision)
{
    float subDivisionInDecimals = ConvertSubDivisionToDecimals(subDivision);
    return static_cast<int>(std::ceil((60.0f / BPM) * sampleRate * subDivisionInDecimals));
}

float Utilities::ConvertSubDivisionToDecimals(SubDivision subDivision)
{
    double ret;
    double tripletModifier = 2.0 / 3.0;
    switch (subDivision) {
    case SubDivision::QUARTER:
        ret = 1.0;
        break;
    case SubDivision::QUARTER_TRIPLET:
        ret = 1.0 * tripletModifier;
        break;
    case SubDivision::EIGTH:
        ret = 0.5;
        break;
    case SubDivision::EIGTH_TRIPLET:
        ret = 0.5 * tripletModifier;
        break;
    case SubDivision::SIXTEENTH:
        ret = 0.25;
        break;
    case SubDivision::SIXTEENTH_TRIPLET:
        ret = 0.25 * tripletModifier;
        break;
    case SubDivision::THIRTY_SECOND:
        ret = 0.125;
        break;
    default:
        ret = 1.0;
        break;
    }

    return ret;
}

