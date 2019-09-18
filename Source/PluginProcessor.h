/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

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



	enum {

		//these value may need to be updated!

		fftOrder = 11,
		fftFrameSize = 1 << fftOrder,
		fftFrameSize2 = fftFrameSize / 2,

		scopeSize = 512,							 
	};

	double angledelta = (440 * 2.0 * M_PI) / 44100;

	//440 = a5, 880 = a6
	float pitchshift = 0.5;
	float pitchshift2 = 0.8;


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

};
