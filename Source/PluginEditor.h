/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TestAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    TestAudioProcessorEditor (TestAudioProcessor&);
    ~TestAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void setMidiInput(int index);

private:
	void sliderValueChanged(Slider* slider) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestAudioProcessor& processor;

	Slider midiVolume;

	ComboBox midiInputList;
	Label midiInputListLabel;
	int lastInputIndex = 0;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessorEditor)
};
