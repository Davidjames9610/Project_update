/*
  ==============================================================================

    SynthVoice.h
    Created: 19 Sep 2019 3:26:24pm
    Author:  David

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "PluginProcessor.h"


class SynthVoice :public SynthesiserVoice {

public:

	bool canPlaySound(SynthesiserSound* sound)override {
		//dynamic cast
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber,
		float velocity,
		SynthesiserSound* sound,
		int currentPitchWheelPosition)override {

		synth_frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		//synth_angle = (synth_frequency * 2.0 * 2.0 * MathConstants<double>::pi) / 44100;
		//synth_level = velocity * 0.15;


		auto string = (String)synth_frequency;
		DBG(string);
	}

	void stopNote(float velocity, bool allowTailOff)override {
	
		//allowTailOff = true;

		//PluginProcessor.getpointer();

		//auto string = (String)synth_frequency;
		//DBG(string);
		//synth_angle = 0;
		clearCurrentNote();
	}

	void pitchWheelMoved(int newPitchWheelValue)override {
		return;
	}


	void controllerMoved(int controllerNumber,int newControllerValue)override {
	
		return;
	}


	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {


		/*
		while (--numSamples >= 0) // [6]
		{
			auto currentSample = (float)(std::sin(synth_currentangle) * synth_level);

			for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
				outputBuffer.addSample(i, startSample, currentSample);
			synth_currentangle += synth_angle;
			++startSample;
		}
		*/
	}






private:

	float synth_frequency = 0;
	double synth_angle = 0;
	float synth_level = 0;
	double synth_currentangle = 0;

	//TestAudioProcessor& processor;
};

