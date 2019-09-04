/*
  ==============================================================================

    PresetSelectorComponent.h
    Created: 3 Sep 2019 11:37:45am
    Author:  Enhancia Dev

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DashCommon.h"

//==============================================================================
/*
*/
class PresetSelectorComponent    : public Component,
                                   private Button::Listener
{
public:
    //==============================================================================
    PresetSelectorComponent();
    ~PresetSelectorComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    void buttonClicked (Button* bttn) override;

private:
    //==============================================================================
    class PresetToggle : public Button
    {
    public:
        //==========================================================================
        PresetToggle (const int toggleNum);
        ~PresetToggle();

        //==========================================================================
        void resized() override;

    private:
        //==========================================================================
        void paintButton (Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

        //==========================================================================
        const int id;

        //==========================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetToggle)
    };

    class GestureLED : public Component
    {
    public:
        //==========================================================================
        GestureLED (const int ledNum);
        ~GestureLED();

        //==========================================================================
        void paint (Graphics&) override;
        void resized() override;

    private:
        //==========================================================================
        const int id;

        //==========================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GestureLED)
    };

    //==============================================================================
    OwnedArray<PresetToggle> toggles;
    OwnedArray<GestureLED> leds;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetSelectorComponent)
};