/*
  ==============================================================================

    VibratoTuner.cpp
    Created: 12 Sep 2019 12:05:15pm
    Author:  Enhancia Dev

  ==============================================================================
*/

#include "VibratoTuner.h"

VibratoTuner::VibratoTuner (HubConfiguration& config, const float& val,
	                                                  NormalisableRange<float> gestRange, const int gestureId,
					                                  const float& vibratoIntensity, float maxIntens,
    				                                  const Range<float> gainMax, const Range<float> threshMax)
    : Tuner ("%",  neova_dash::gesture::getHighlightColour (gestureId, config.isGestureActive (gestureId))),
      hubConfig (config),
      value (val), gestureRange (gestRange), id (gestureId),
      intensity (vibratoIntensity), maxIntensity (maxIntens),
      parameterMaxGain (gainMax), parameterMaxThreshold (threshMax)
{
    createSliders();
    createLabels();
}

VibratoTuner::VibratoTuner (HubConfiguration& config, DataReader& reader, const int gestureId)
    : VibratoTuner (config,
    				reader.getFloatValueReference (neova_dash::data::variance),
    				NormalisableRange<float> (-neova_dash::gesture::VIBRATO_RANGE_MAX,
    										  neova_dash::gesture::VIBRATO_RANGE_MAX),
    				gestureId,
    				reader.getFloatValueReference (neova_dash::data::acceleration),
    				100.0f,
    	            Range<float> (0.0f, neova_dash::ui::VIBRATO_DISPLAY_MAX),
    	            Range<float> (0.0f, neova_dash::ui::VIBRATO_THRESH_DISPLAY_MAX))
{
}

VibratoTuner::~VibratoTuner()
{
	setLookAndFeel (nullptr);
	gainSlider = nullptr;
	thresholdSlider = nullptr;
	gainLabel = nullptr;
	thresholdLabel = nullptr;
}
    
//==============================================================================
void VibratoTuner::paint (Graphics& g)
{
	drawValueCursor (g);
	drawIntensityCursor (g);

	g.setColour (neova_dash::colour::subText);
	g.setFont (neova_dash::font::dashFontLight.withHeight (13.0f).withExtraKerningFactor (0.08f));
	g.drawText ("THRESHOLD", thresholdSlider->getBounds().withSizeKeepingCentre (100, 50)
														 .withY (thresholdSlider->getBounds().getBottom()),
							 Justification::centredTop);

	g.drawText ("GAIN", gainSlider->getBounds().withSizeKeepingCentre (100, 50)
											   .withY (thresholdSlider->getBounds().getBottom()),
						Justification::centredTop);
}

void VibratoTuner::resized()
{
	tunerArea = getLocalBounds().reduced (getLocalBounds().getWidth()/5,
										  getLocalBounds().getHeight()/5);

	auto area = tunerArea;

	gainSlider->setBounds (area.removeFromRight (area.getWidth()*2 / 3));
	thresholdSlider->setBounds (area.withSizeKeepingCentre (area.getWidth(), (gainSlider->getHeight() / 2) + 10));

	updateLabelBounds(gainLabel.get());
	updateLabelBounds(thresholdLabel.get());
}
    
void VibratoTuner::updateComponents()
{
	if (gainSlider->getThumbBeingDragged() == -1)
    {
        // Sets slider value
        gainSlider->setValue (static_cast<double>(getGain()), dontSendNotification);
        
        // Sets label text
        if (!(gainLabel->isBeingEdited()))
		{
		    gainLabel->setText (String (static_cast<int>(getGain())) + valueUnit, dontSendNotification);
		}
    }

	if (thresholdSlider->getThumbBeingDragged() == -1)
    {
        // Sets slider value
        thresholdSlider->setValue (static_cast<double>(getThreshold()), dontSendNotification);
        //setThresholdSliderColour();
        
        // Sets label text
        if (!(thresholdLabel->isBeingEdited()))
		{
		    thresholdLabel->setText (String (static_cast<int>(getThreshold())), dontSendNotification);
		}
    }
}

void VibratoTuner::updateDisplay()
{
	computeSmoothIntensity (1.5f);

	if (!std::isnan(smoothIntensity) && (smoothIntensity > intensity
									     || rint (intensity) != rint (lastIntensity)
									     || intensity > getThreshold()))
	{
		repaint (tunerArea);
	}
}

void VibratoTuner::updateColour()
{
	setColour (neova_dash::gesture::getHighlightColour (hubConfig.getGestureData (id).type,
    												    hubConfig.isGestureActive (id)));
}

//==============================================================================
void VibratoTuner::labelTextChanged (Label* lbl)
{
	if (lbl == gainLabel.get())
	{ 
		// checks that the string is numbers only
	    if (lbl->getText().containsOnly ("-0123456789"+valueUnit) == false)
	    {
	        lbl->setText (String (static_cast<int>(getGain())) + valueUnit, dontSendNotification);
	        return;
	    }

	    // Gets the float value of the text 
	    float val = lbl->getText().getFloatValue();

	    if (val < 0.0f) val = 0.0f;
	    else if (val > parameterMaxGain.getEnd()) val = parameterMaxGain.getEnd();
	    
	    setGain (val);
	    lbl->setText (String (val) + valueUnit, dontSendNotification);
		gainSlider->setValue (val, dontSendNotification);
	}

	else if (lbl == thresholdLabel.get())
	{ 
		// checks that the string is numbers only
	    if (lbl->getText().containsOnly ("-0123456789") == false)
	    {
	        lbl->setText (String (static_cast<int>(getThreshold())), dontSendNotification);
	        return;
	    }

	    // Gets the float value of the text 
	    float val = lbl->getText().getFloatValue();

	    if (val < 0.0f) val = 0.0f;
	    else if (val > parameterMaxThreshold.getEnd()) val = parameterMaxThreshold.getEnd();
	    
	    setThreshold (val);
	    lbl->setText (String (val), dontSendNotification);
		thresholdSlider->setValue (val, dontSendNotification);
		//setThresholdSliderColour();
	}

	updateLabelBounds (lbl);
}

void VibratoTuner::editorShown (Label*, TextEditor& ted)
{
    ted.setJustification (Justification::centred);
}

void VibratoTuner::editorHidden (Label* lbl, TextEditor&)
{
    lbl->setVisible (false);
}

void VibratoTuner::sliderValueChanged (Slider* sldr)
{
	if (sldr == gainSlider.get())
	{
		updateLabelBounds (gainLabel.get());
		gainLabel->setText (String (static_cast<int>(gainSlider->getValue())) + valueUnit, dontSendNotification);
	}
	else if (sldr == thresholdSlider.get())
	{    
		updateLabelBounds (thresholdLabel.get());
		thresholdLabel->setText (String (static_cast<int>(thresholdSlider->getValue())), dontSendNotification);
		//setThresholdSliderColour();
	}
}

void VibratoTuner::mouseDown (const MouseEvent& e)
{
	if (e.mods.isLeftButtonDown())
	{
		if (e.getNumberOfClicks() == 1)
		{
			if (e.eventComponent == gainSlider.get())
			{
				gainLabel->setVisible (true);
			}
			else if (e.eventComponent == thresholdSlider.get())
			{
				thresholdLabel->setVisible (true);
			}
		}
		else // Double Click
		{
			if (e.eventComponent == gainSlider.get())
			{
				gainLabel->setVisible (true);
				gainLabel->showEditor();
			}
			else if (e.eventComponent == thresholdSlider.get())
			{
				thresholdLabel->setVisible (true);
				thresholdLabel->showEditor();
			}
		}
	}
}

void VibratoTuner::mouseUp (const MouseEvent& e)
{
	if (e.mods.isLeftButtonDown() && e.getNumberOfClicks() == 1)
	{
		if (e.eventComponent == gainSlider.get())
		{
			setGain (static_cast<float>(gainSlider->getValue()));
			gainLabel->setVisible (false);
		}
		else if (e.eventComponent == thresholdSlider.get())
		{
			setThreshold (static_cast<float>(thresholdSlider->getValue()));
			thresholdLabel->setVisible (false);
		}
	}
}

void VibratoTuner::setColour (const Colour newColour)
{
	Tuner::setColour (newColour);

    gainSlider->setColour (Slider::rotarySliderFillColourId, tunerColour);
    thresholdSlider->setColour (Slider::trackColourId, tunerColour);

    auto setLabelColours = [this] (Label& label)
    {
        label.setColour (Label::textColourId, tunerColour);
        label.setColour (Label::textWhenEditingColourId, tunerColour);
        label.setColour (TextEditor::textColourId, tunerColour);
        label.setColour (TextEditor::highlightColourId, tunerColour.withAlpha (0.2f));
        label.setColour (TextEditor::highlightedTextColourId, tunerColour);
        label.setColour (CaretComponent::caretColourId, tunerColour.withAlpha (0.2f));
    };

    setLabelColours (*gainLabel);
    setLabelColours (*thresholdLabel);
}

void VibratoTuner::createSliders()
{
    addAndMakeVisible (*(gainSlider = std::make_unique<Slider> ("Gain Slider")));
    addAndMakeVisible (*(thresholdSlider = std::make_unique<Slider> ("Threshold Slider")));

    // Gain Slider parameters
	gainSlider->setSliderStyle(Slider::Rotary);
    //gainSlider->setRotaryParameters (MathConstants<float>::pi*5/3, MathConstants<float>::pi*7/3, true);
    gainSlider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    gainSlider->setColour (Slider::rotarySliderFillColourId, tunerColour);
    gainSlider->setColour (Slider::rotarySliderOutlineColourId, neova_dash::colour::tunerSliderBackground);
    gainSlider->setScrollWheelEnabled (false);
    gainSlider->setRange (static_cast<double>(parameterMaxGain.getStart()), static_cast<double>(parameterMaxGain.getEnd()), 0.01);
    gainSlider->setValue (static_cast<double>(getGain()));
    gainSlider->addListener (this);
    gainSlider->addMouseListener (this, false);

    // ThreshSliderParameters
	thresholdSlider->setSliderStyle(Slider::LinearVertical);
    thresholdSlider->setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
    thresholdSlider->setColour (Slider::backgroundColourId, neova_dash::colour::tunerSliderBackground);
    thresholdSlider->setColour (Slider::trackColourId, tunerColour);
    thresholdSlider->setScrollWheelEnabled (false);
    thresholdSlider->setRange (static_cast<double>(parameterMaxThreshold.getStart()),
							   static_cast<double>(parameterMaxThreshold.getEnd()), 1.0);
    thresholdSlider->setValue (static_cast<double>(getThreshold()));
    thresholdSlider->addListener (this);
    thresholdSlider->addMouseListener (this, false);
}
    
void VibratoTuner::createLabels()
{
    addAndMakeVisible (*(gainLabel = std::make_unique<Label> ("Gain Label",
    	 									  TRANS (String(static_cast<int>(getGain())) + valueUnit))));
    addAndMakeVisible (*(thresholdLabel = std::make_unique<Label> ("Threshold Label",
    									           TRANS (String(static_cast<int>(getThreshold()))))));

    auto setLabelSettings = [this] (Label& label)
    {
        label.setEditable (false, false, false);
        label.setFont (neova_dash::font::dashFont.withHeight (13.0f));
        label.setJustificationType (Justification::centred);
        label.setColour (Label::textColourId, tunerColour);
        label.setColour (Label::textWhenEditingColourId, tunerColour);
        label.setColour (TextEditor::textColourId, tunerColour);
        label.setColour (TextEditor::highlightColourId, tunerColour.withAlpha (0.2f));
        label.setColour (TextEditor::highlightedTextColourId, tunerColour);
        label.setColour (CaretComponent::caretColourId, tunerColour.withAlpha (0.2f));
        label.setSize (30, 20);
        label.addListener (this);
        label.setVisible (false);
    };

    setLabelSettings (*gainLabel);
    setLabelSettings (*thresholdLabel);
}

void VibratoTuner::updateLabelBounds (Label* labelToUpdate)
{
	if (labelToUpdate == gainLabel.get())
	{
		float angle = static_cast<float>(gainSlider->getRotaryParameters().startAngleRadians
		                  + ((gainSlider->getValue() - gainSlider->getMinimum())
							     / gainSlider->getRange().getLength())
                             *std::abs (gainSlider->getRotaryParameters().endAngleRadians
                             	- gainSlider->getRotaryParameters().startAngleRadians));

        // Slider radius with a 10-pixels offset
        float radius = (jmin (gainSlider->getWidth(), gainSlider->getHeight()) - 10.0f) / 2.0f
        				   + 10.0f;

		labelToUpdate->setCentrePosition (static_cast<int>(gainSlider->getBounds().getCentreX()
											 + radius * std::cos (angle - MathConstants<float>::halfPi)),
                                          static_cast<int>(gainSlider->getBounds().getCentreY()
                                          	 + radius * std::sin (angle - MathConstants<float>::halfPi)));
	}
	else if (labelToUpdate == thresholdLabel.get())
	{
		labelToUpdate->setCentrePosition (thresholdSlider->getBounds().getCentreX() - 25,
										  static_cast<int>(thresholdSlider->getBottom() - 10
			                                  - thresholdSlider->valueToProportionOfLength(thresholdSlider->getValue())
                                              * (thresholdSlider->getHeight() - 20)));
	}
}

void VibratoTuner::setGain (float val)
{
	hubConfig.setFloatValue (id, HubConfiguration::gestureParam0, val);
}

void VibratoTuner::setThreshold (float val)
{
	hubConfig.setFloatValue (id, HubConfiguration::gestureParam1, val);
}

float VibratoTuner::getGain()
{
    return hubConfig.getGestureData (id).gestureParam0;
}

float VibratoTuner::getThreshold()
{
    return hubConfig.getGestureData (id).gestureParam1;
}

void VibratoTuner::drawValueCursor (Graphics& g)
{
	lastValue = value;

	float convertedValue = gestureRange.convertTo0to1 (value);

	int offset = (intensity < getThreshold()) ? 0
	                                          : static_cast<int>((convertedValue - 0.5f) * (gainSlider->getWidth() - 30)
	                                                                    * (getGain())/50);

	juce::Point<int> cursorPoint = {gainSlider->getBounds().getCentreX() + offset,
							  gainSlider->getBounds().getCentreY()};

    g.setColour ((intensity < getThreshold()) ? neova_dash::colour::tunerSliderBackground : tunerColour);
    g.fillEllipse (juce::Rectangle<float> (5.0f, 5.0f).withCentre (cursorPoint.toFloat()));
}

void VibratoTuner::drawIntensityCursor (Graphics& g)
{
	lastIntensity = intensity;

    juce::Point<float> cursorPoint (
        static_cast<float>(thresholdSlider->getBounds ().getCentreX () - 10),
        static_cast<float>(jmax (thresholdSlider->getBottom () - 10 - (thresholdSlider->getHeight () - 20) * smoothIntensity / maxIntensity,
            static_cast<float>(thresholdSlider->getY () + 10)))
    );

	Path cursorPath;
    cursorPath.addTriangle ({cursorPoint.x - 3.0f, cursorPoint.y - 3.0f},
                            {cursorPoint.x + 3.0f, cursorPoint.y       },
                            {cursorPoint.x - 3.0f, cursorPoint.y + 3.0f});

    g.setColour ((intensity < getThreshold()) ? neova_dash::colour::tunerSliderBackground : tunerColour);
    g.fillPath (cursorPath);
}

void VibratoTuner::setThresholdSliderColour()
{
	thresholdSlider->setColour (Slider::trackColourId,
							    (intensity < getThreshold()) ? neova_dash::colour::tunerSliderBackground
			                       							 : tunerColour);
}

void VibratoTuner::computeSmoothIntensity (float smoothnessRamp)
{
	float decrement = std::pow (smoothnessRamp, incrementalSmoothFactor);

	if (intensity > smoothIntensity - decrement)
	{
		smoothIntensity = intensity;
		incrementalSmoothFactor = 1.0f;
	}
	else
	{
		smoothIntensity -= decrement;
		incrementalSmoothFactor += 0.5f;
	}
}