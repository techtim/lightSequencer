//
//  bpmControl.h
//  lightSequencer
//
//  Created by Tim Tavlintsev on 12.01.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.


#ifndef bpmControl_h
#define bpmControl_h

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxButton.h"
#include "LiveAudioAnalysis.h"

class bpmControl : public ofxMidiListener {
    public:
	
    bpmControl();
    ~bpmControl();

    void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei);
    void setupMidi(unsigned int ident, unsigned int channel, unsigned int inPort, unsigned int outPort);

	void draw();

	bool isClicked(int x, int y);

    int leftX, leftY, rightX, rightY;
	unsigned int width, height;
    float BPM; 
    bool autoSync;
    

    LiveAudioAnalysis liveAudioAnalysis;
    void processFrame(float* frame, int framesize){
        liveAudioAnalysis.processFrame(frame, framesize);
        if (liveAudioAnalysis.btrack.playbeat == 1){
			liveAudioAnalysis.lastBeatTime = liveAudioAnalysis.aubioOnsetDetectionVector.size()-1;
		}
    }

    ofTrueTypeFont verdana;
	ofTrueTypeFont verdana2;
	string BPMstring;
    
    ofxButton Sync;
    ofxButton UpBpm;
    ofxButton DownBpm;
    ofxButton Reset;
    
    
    
    //---MIDI
    void newMidiMessage(ofxMidiMessage &args);
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;
    unsigned int midiInPort, midiOutPort;
	unsigned int midiChannel; // channel for exact sequence , setuped 
	unsigned int midiSyncCC, midiResetCC;
	unsigned int midiUpBpmCC, midiDownBpmCC;

};

#endif
