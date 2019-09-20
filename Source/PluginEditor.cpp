/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestAudioProcessorEditor::TestAudioProcessorEditor (TestAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// This is where our plugin’s editor size is set.
	setSize(400, 400);

	midiVolume.addListener(this);
	// these define the parameters of our slider object
	midiVolume.setSliderStyle(Slider::LinearBarVertical);
	midiVolume.setRange(0.0, 127.0, 1.0);
	midiVolume.setTextBoxStyle(Slider::NoTextBox, false, 90, 100);
	midiVolume.setPopupDisplayEnabled(true, false, this);
	midiVolume.setTextValueSuffix(" Volume");
	midiVolume.setValue(1.0);
	// this function adds the slider to the editor
	addAndMakeVisible(&midiVolume);

	addAndMakeVisible(midiInputListLabel);
	midiInputListLabel.setText("MIDI Input:", dontSendNotification);
	midiInputListLabel.attachToComponent(&midiInputList, true);

	addAndMakeVisible(midiInputList);
	midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
	auto midiInputs = MidiInput::getDevices();
	midiInputList.addItemList(midiInputs, 1);
	//midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

	//MidiInput::getDefaultDeviceIndex();


}


TestAudioProcessorEditor::~TestAudioProcessorEditor()
{
}

//==============================================================================
void TestAudioProcessorEditor::paint (Graphics& g)
{
	// fill the whole window white
	g.fillAll(Colours::white);
	// set the current drawing colour to black
	g.setColour(Colours::black);
	// set the font size and draw text to the screen
	g.setFont(15.0f);
	g.drawFittedText("Midi Volume", 0, 0, getWidth(), 100, Justification::centred, 1);
}


void TestAudioProcessorEditor::setMidiInput(int index) {

	//auto list = MidiInput::getDevices();



}

void TestAudioProcessorEditor::resized()
{

	midiInputList.setBounds(200, 10, getWidth() - 210, 20);
	// sets the position and size of the slider with arguments (x, y, width, height)
	midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void TestAudioProcessorEditor::sliderValueChanged(Slider* slider) {

	auto volumetmp = midiVolume.getValue() / 130;
	auto temptmp = (String)volumetmp;
	
	DBG(temptmp);

	
	//processor.noteOnVel = midiVolume.getValue() / 130;

	

}
