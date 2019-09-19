/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestAudioProcessor::TestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), window(fftFrameSize, dsp::WindowingFunction<float>::hann), forwardFFT(fftOrder)
#endif
{

	tmp = 0;
	memset(gLastPhase, 0, fftFrameSize * sizeof(float));
	memset(gAnaMagn, 0, fftFrameSize * sizeof(float));
	memset(gAnaFreq, 0, fftFrameSize * sizeof(float));

	memset(gInFIFO, 0, fftFrameSize * sizeof(float));

	memset(gSumPhase, 0, fftFrameSize * sizeof(float));
	memset(gSumPhase2, 0, fftFrameSize * sizeof(float));

	//memset(SumPhase2D, 0, fftFrameSize * sizeof(float));


	memset(gOutputAccum, 0, (2 * fftFrameSize) * sizeof(float));

	memset(gOutFifo, 0, (fftFrameSize) * sizeof(float));


	StepSize = fftFrameSize / osamp; // 2048/8 = 256 samples
	freqPerbin = 44100 / (double)fftFrameSize;  //this will only work for 44100 hz sample rate 
	inFifoLatency = fftFrameSize - StepSize;
	gRover = fftFrameSize - StepSize; // 2048 - 256 = 1792
	excpt = 2. * M_PI * ((double)StepSize) / (double)fftFrameSize;



}

TestAudioProcessor::~TestAudioProcessor()
{
}

//==============================================================================
const String TestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestAudioProcessor::setCurrentProgram (int index)
{
}

const String TestAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	//sin stuff:
	auto level = 0.25f;

	auto* leftbuffer = buffer.getWritePointer(0);
	auto* rightbuffer = buffer.getWritePointer(1);
	auto* inputsound = buffer.getReadPointer(0);

		for (auto sample = 0; sample < buffer.getNumSamples(); sample++) {

			auto currentsample = (float)std::sin(sin_currentangle);
			sin_currentangle += angledelta;

			//input
			//pushNextSampleIntoFifo(currentsample * level);
			pushNextSampleIntoFifo(inputsound[sample]);

			//output
			//leftbuffer[sample] = (currentsample * level);		
			//rightbuffer[sample] = (currentsample * level);

			leftbuffer[sample] = (gOutFifo[gRover - inFifoLatency]);		
			rightbuffer[sample] = (gOutFifo[gRover - inFifoLatency]);
			
		}
}

//==============================================================================
bool TestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

void TestAudioProcessor::pushNextSampleIntoFifo(float sample) {


	gInFIFO[gRover] = sample;
	gRover++;

	if (gRover == fftFrameSize) {
		gRover = inFifoLatency;           

		zeromem(gFFTworksp, sizeof(gFFTworksp));
		memcpy(gFFTworksp, gInFIFO, sizeof(gInFIFO));

		// [1] Window

		for (k = 0; k < fftFrameSize; k++) {

			window2 = -.5 * cos(2. * M_PI * (double)k / (double)fftFrameSize) + .5;
			gFFTworksp[k] = gInFIFO[k] * window2;
		}

		// [2] forward fft

		forwardFFT.performRealOnlyForwardTransform(gFFTworksp, true);

		
		// [3] Analysis 
		Analysis();

		// [3.b] [pitch detection]
		PitchDetection();

		zeromem(gFFTworksp, sizeof(gFFTworksp));
		// [4] Processing / Pitch Shifting 
		//Pitchshift(gSynMagn, gSynFreq, PitchShiftArray[0]);
		//Pitchshift(gSynMagn2, gSynFreq2, PitchShiftArray[1]);


		//new pitchshift function.
		//zero arrays
		//memset(SynthArrayFreq, 0, fftFrameSize * sizeof(float));
		//memset(SynthArrayMag, 0, fftFrameSize * sizeof(float));

		//PitchShift2D();
		PitchShift2D(0);
		PitchShift2D(1);
		PitchShift2D(2);
		PitchShift2D(3);

		// [5] Re-Synthesis
		//ReSynthesis(gSynMagn, gSynFreq, gSumPhase);
		//ReSynthesis(gSynMagn2, gSynFreq2, gSumPhase2);

		//2D Version...
		ReSynthesis2D(0);
		ReSynthesis2D(1);
		ReSynthesis2D(2);
		ReSynthesis2D(3);

		//zero negative frequencies....
		for (k = fftFrameSize + 2; k < 2 * fftFrameSize; k++) gFFTworksp[k] = 0.;

		// do inverse transfrom..
		smbFft(gFFTworksp, fftFrameSize, 1);

		for (k = 0; k < fftFrameSize; k++) {
			window2 = -.5 * cos(2. * M_PI * (double)k / (double)fftFrameSize) + .5;
			gOutputAccum[k] += 2. * window2 * gFFTworksp[2 * k] / (fftFrameSize2 * osamp);// / (fftFrameSize2*osamp);
		}
		for (k = 0; k < StepSize; k++) {
			gOutFifo[k] = gOutputAccum[k];
		}

		//shift accumulator
		memmove(gOutputAccum, gOutputAccum + StepSize, fftFrameSize * sizeof(float));

		//move input, packets of stepsize 256
		for (k = 0; k < inFifoLatency; k++) {
			gInFIFO[k] = gInFIFO[k + StepSize];
		}

	}
}

void TestAudioProcessor::PitchDetection(void) {


	float tmpmgn = 0;
	int tmpindex = 0;
	float largestpitch;

	for (k = 0; k < fftFrameSize2; k++) {
		if (gAnaMagn[k] > tmpmgn) {
			tmpmgn = gAnaMagn[k];
			tmpindex = k;
		}
	}
	

	//when only change the pitch if the new frequency is within our rang...
	if ((gAnaFreq[tmpindex] > 100) && (gAnaFreq[tmpindex] < 1050)) {
		largestpitch = gAnaFreq[tmpindex];
		for (k = 0; k < 6; k++) {
			PitchShiftArray[k] = roundf((MidiArray[k] / largestpitch) * 100) / 100;
			//pitchshift2 = roundf((midi_array[1] / largestpitch) * 100) / 100;
		}
	}
}


void TestAudioProcessor::Pitchshift(float gSynMagnf[], float gSynFreqf[], float pitchshiftf) {

	memset(gSynMagnf, 0, fftFrameSize * sizeof(float));
	memset(gSynFreqf, 0, fftFrameSize * sizeof(float));

	for (k = 0; k < fftFrameSize2; k++) {
		index = k * pitchshiftf;
		if (index < fftFrameSize2) {
			gSynMagnf[index] += gAnaMagn[k];
			gSynFreqf[index] = gAnaFreq[k] * pitchshiftf;
		}

	}

}

void TestAudioProcessor::PitchShift2D(int MidiNo) {

	//SynthArrayFreq, SynthArrayMag, 0, PitchShiftArray[0]

	//zero array quick
	for (k = 0; k < fftFrameSize; k++) {
		SynthArrayMag[MidiNo][k] = 0.;
		SynthArrayFreq[MidiNo][k] = 0.;
	}

	for (k = 0; k < fftFrameSize2; k++) {
		index = k * PitchShiftArray[MidiNo];
		if (index < fftFrameSize2) {
			SynthArrayMag[MidiNo][index] += gAnaMagn[k];
			SynthArrayFreq[MidiNo][index] = gAnaFreq[k] * PitchShiftArray[MidiNo];
		}

	}

}

void TestAudioProcessor::ReSynthesis2D(int MidiNo) {

	
	for (k = 0; k < fftFrameSize2; k++) {

		//we get our magnitude and true frequnecy
		magn = SynthArrayMag[MidiNo][k];
		tmp = SynthArrayFreq[MidiNo][k];

		//subtract bin mid frequency
		tmp -= (double)k * freqPerbin;

		//get bin deviation from freq deviation
		tmp /= freqPerbin;

		//osamp into acount
		tmp = 2.0 * M_PI * tmp / osamp;

		//add the overlap phase advance back in 
		tmp += (double)k * excpt;

		//accumulate delta phase to get bin phase
		SumPhase2D[MidiNo][k] += tmp;
		phase = SumPhase2D[MidiNo][k];

		//get real and imag parts and re-interleave;
		gFFTworksp[2 * k] += (magn * cos(phase)); //* 0.5);
		gFFTworksp[2 * k + 1] += (magn * sin(phase));// * 0.5);
	}
}


void TestAudioProcessor::ReSynthesis(float gSynMagnf[], float gSynFreqf[], float gSumPhasef[]) {

	for (k = 0; k < fftFrameSize2; k++) {

		//we get our magnitude and true frequnecy
		magn = gSynMagnf[k];
		tmp = gSynFreqf[k];

		//subtract bin mid frequency
		tmp -= (double)k * freqPerbin;

		//get bin deviation from freq deviation
		tmp /= freqPerbin;

		//osamp into acount
		tmp = 2.0 * M_PI * tmp / osamp;

		//add the overlap phase advance back in 
		tmp += (double)k * excpt;

		//accumulate delta phase to get bin phase
		gSumPhasef[k] += tmp;
		phase = gSumPhasef[k];

		//get real and imag parts and re-interleave;
		gFFTworksp[2 * k] += (magn * cos(phase)); //* 0.5);
		gFFTworksp[2 * k + 1] += (magn * sin(phase));// * 0.5);
	}
}



void  TestAudioProcessor::Analysis(void) {

	for (int k = 0; k < fftFrameSize; k++) {

		//de-interlace FFT buffer

		real = gFFTworksp[2 * k];
		imag = gFFTworksp[(2 * k) + 1];

		//compute magnitude and phase
		magn = 2. * sqrt(real * real + imag * imag);
		phase = atan2(imag, real);

		//compute the phase difference
		tmp = phase - gLastPhase[k];
		gLastPhase[k] = phase;

		//subtract the expected phase differance
		tmp -= (double)k * excpt;

		//map phase back onto +- pie interval.
		tmp = princarg(tmp);

		tmp = ((double)osamp * tmp) / (2. * M_PI);

		tmp = ((double)k * freqPerbin) + (tmp * freqPerbin);

		//store magnitude and true frequnecy in analysis arrays
		gAnaFreq[k] = tmp;
		gAnaMagn[k] = magn;

	}
}


float TestAudioProcessor::princarg(float phase) {

	//unwrap the phase function to within the +- pi interval...

	// Working!? 

	//float newphase = 16.49;   //gives -2.35 :)
	//float newphase = M_PI * 6.25; //should give 0.78..


	float a_temp;
	int k_temp;

	a_temp = phase / (float)(2 * M_PI);
	k_temp = round(a_temp);
	a_temp = phase - (float)(k_temp * 2 * M_PI);
	//DBG(a_temp);
	return a_temp;

}

void TestAudioProcessor::smbFft(float* fftBuffer, long fftFrameSize, long sign)
/*
	FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the
	time domain data in fftBuffer[0...2*fftFrameSize-1]. The FFT array takes
	and returns the cosine and sine parts in an interleaved manner, ie.
	fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. fftFrameSize
	must be a power of 2. It expects a complex input signal (see footnote 2),
	ie. when working with 'common' audio signals our input signal has to be
	passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. In that case, the transform
	of the frequencies of interest is in fftBuffer[0...fftFrameSize].
*/
{
	float wr, wi, arg, * p1, * p2, temp;
	float tr, ti, ur, ui, * p1r, * p1i, * p2r, * p2i;
	long i, bitm, j, le, le2, k;

	for (i = 2; i < 2 * fftFrameSize - 2; i += 2) {
		for (bitm = 2, j = 0; bitm < 2 * fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		if (i < j) {
			p1 = fftBuffer + i; p2 = fftBuffer + j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
	}
	for (k = 0, le = 2; k < (long)(log(fftFrameSize) / log(2.) + .5); k++) {
		le <<= 1;
		le2 = le >> 1;
		ur = 1.0;
		ui = 0.0;
		arg = M_PI / (le2 >> 1);
		wr = cos(arg);
		wi = sign * sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer + j; p1i = p1r + 1;
			p2r = p1r + le2; p2i = p2r + 1;
			for (i = j; i < 2 * fftFrameSize; i += le) {
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			tr = ur * wr - ui * wi;
			ui = ur * wi + ui * wr;
			ur = tr;
		}
	}
}



AudioProcessorEditor* TestAudioProcessor::createEditor()
{
    return new TestAudioProcessorEditor (*this);
}

//==============================================================================
void TestAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestAudioProcessor();
}
