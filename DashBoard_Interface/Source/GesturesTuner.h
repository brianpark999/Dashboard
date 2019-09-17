/*
  ==============================================================================

    GesturesTuner.h
    Created: 16 Sep 2019 10:23:51am
    Author:  Enhancia Dev

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DashCommon.h"
#include "DashBoardLookAndFeel.h"
#include "HubConfiguration.h"

#include "Tuner.h"
#include "TwoRangeTuner.h"
#include "OneRangeTuner.h"
#include "VibratoTuner.h"

//==============================================================================
class PitchBendTuner: public TwoRangeTuner
{
public:
    PitchBendTuner (HubConfiguration& config, const int gestureId)
        :   TwoRangeTuner (config, gestureId, config.data[4], 
        				   NormalisableRange<float> (neova_dash::gesture::PITCHBEND_MIN, neova_dash::gesture::PITCHBEND_MAX),
			               Range<float> (neova_dash::ui::PITCHBEND_DISPLAY_MIN, neova_dash::ui::PITCHBEND_DISPLAY_MAX),
                           String (CharPointer_UTF8 ("\xc2\xb0")))
    {}
    
    ~PitchBendTuner()
    {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchBendTuner)
};


//==============================================================================
class TiltTuner: public OneRangeTuner
{
public:
    TiltTuner (HubConfiguration& config, const int gestureId)
        :   OneRangeTuner (config, gestureId, config.data[3], 
        				   NormalisableRange<float> (neova_dash::gesture::TILT_MIN, neova_dash::gesture::TILT_MAX),
			               Range<float> (neova_dash::ui::TILT_DISPLAY_MIN, neova_dash::ui::TILT_DISPLAY_MAX),
                           String (CharPointer_UTF8 ("\xc2\xb0")), OneRangeTuner::tilt)
    {}
    
    ~TiltTuner()
    {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TiltTuner)
};

/*
//==============================================================================
class WaveTuner: public OneRangeTuner
{
public:
    WaveTuner (HubConfiguration& config, const int gestureId)
        :   OneRangeTuner (config, gestureId, config.data[4],
        				   NormalisableRange<float> (neova_dash::gesture::WAVE_MIN, neova_dash::gesture::WAVE_MAX),
			               Range<float> (neova_dash::ui::WAVE_DISPLAY_MIN, neova_dash::ui::WAVE_DISPLAY_MAX),
                           String (CharPointer_UTF8 ("\xc2\xb0")), OneRangeTuner::wave)
    {}
    
    ~WaveTuner()
    {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveTuner)
};
*/

//==============================================================================
class RollTuner: public OneRangeTuner
{
public:
    RollTuner (HubConfiguration& config, const int gestureId)
        :   OneRangeTuner (config, gestureId, config.data[4], 
        				   NormalisableRange<float> (neova_dash::gesture::ROLL_MIN, neova_dash::gesture::ROLL_MAX),
			               Range<float> (neova_dash::ui::ROLL_DISPLAY_MIN, neova_dash::ui::ROLL_DISPLAY_MAX),
                           String (CharPointer_UTF8 ("\xc2\xb0")), OneRangeTuner::roll)
    {}
    
    ~RollTuner()
    {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RollTuner)
};