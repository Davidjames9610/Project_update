/*
  ==============================================================================

    SynthSound.h
    Created: 19 Sep 2019 3:26:10pm
    Author:  David

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound :public SynthesiserSound {

public:

	bool appliesToNote(int midiNoteNumber) {
		// return true if this sound should be played when a given midi note is pressed.
		return true;
	}
	bool appliesToChannel(int midiChannel) {
		//
		return true;
	}


};