/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "SynthVoice.h"

#define M_PI 3.14159265358979323846
#define MAX_FRAME_LENGTH 8192  //(2^13)



//==============================================================================
/**
*/
class TestAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TestAudioProcessor();
    ~TestAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//my functions:
	
	void pushNextSampleIntoFifo(float sample);
	void smbFft(float* fftBuffer, long fftFrameSize, long sign);
	float princarg(float phase);
	void Analysis(void);
	void Pitchshift(float gSynMagnf[], float gSynFreqf[], float pitchshiftf);
	void ReSynthesis(float gSynMagnf[], float gSynFreqf[], float gSumPhasef[]);
	void PitchDetection(void);
	void PopulatePitchArray(void);



	enum {

		//these value may need to be updated!

		fftOrder = 11,
		fftFrameSize = 1 << fftOrder,
		fftFrameSize2 = fftFrameSize / 2,

		scopeSize = 512,							 
	};


	void PitchShift2D(int MidiNo);
	void ReSynthesis2D(int MidiNo);

	double angledelta = (220 * 2.0 * M_PI) / 44100;

	//440 = a5, 880 = a6
	float pitchshift = 1.2;
	float pitchshift2 = 1.3;

	float input_note1 = 440;
	float input_note2 = 523;

	//Aray
	float MidiArray[6] = {220, 277.18, 329.63, 440, 554.37, 659.25};
	float PitchShiftArray[6] = {0,0,0,0,0,0};

	//Synthesis Arrays
	float SynthArrayFreq[6][fftFrameSize]; 
	float SynthArrayMag[6][fftFrameSize];

	float SumPhase2D[6][fftFrameSize] = {0.};

	// midi array 

	float Midi2Hz_Array[109] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27.5,29.1,30.9,32.7,34.6,36.7,38.9,41.2,43.7,46.2,49,51.9,55,58.3,61.7,65.4,69.3,73.4,77.8,82.4,87.3,92.5,98,103.8,110,116.5,123.5,130.8,138.6,146.8,155.6,164.8,174.6,185,196,207.7,220,233.1,246.9,261.626,277.2,293.7,311.1,329.6,349.2,370,392,415.3,440,466.2,493.9,523.3,554.4,587.3,622.3,659.3,698.5,740,784,830.6,880,932.3,987.8,1046.5,1108.7,1174.7,1244.5,1318.5,1396.9,1480,1568,1661.2,1760,1864.7,1975.5,2093,2217.5,2349.3,2489,2637,2793.8,2960,3136,3322.4,3520,3729.3,3951.1, 4186};

	float noteOnVel = 0;
	// Synth pointer

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestAudioProcessor)

	dsp::WindowingFunction<float> window;
	dsp::FFT forwardFFT;

	float gInFIFO[fftFrameSize];
	float gOutFifo[fftFrameSize];

	float gLastPhase[fftFrameSize];
	float gchangePhase[fftFrameSize];
	float gMag[fftFrameSize];

	float gAnaMagn[fftFrameSize];
	float gAnaFreq[fftFrameSize];

	float gSynMagn[fftFrameSize];
	float gSynFreq[fftFrameSize];

	float gSynMagn2[fftFrameSize];
	float gSynFreq2[fftFrameSize];

	long index;
	long index2;

	float gSumPhase[fftFrameSize];
	float gSumPhase2[fftFrameSize];
	float gOutputAccum[2 * fftFrameSize];


	float tmp;
	float gFFTworksp[2 * fftFrameSize];
	long k;
	//float gFFTworksp2[2 * MAX_FRAME_LENGTH];

	int fifoIndex = 0;
	int inFifoLatency;
	int gRover;
	double excpt;
	int osamp = 8;
	int StepSize;
	long qpd;
	double freqPerbin;
	//double window = 0;

	double real = 0;
	double imag = 0;
	double magn = 0;
	double phase = 0;

	double window2;


	//sine wave generation!

	double sin_currentsamplerate = 44100;
	double sin_currentangle = 0;

	Synthesiser my_synth;
	SynthVoice* myVoice;
	

};
