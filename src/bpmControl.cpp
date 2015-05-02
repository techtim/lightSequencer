//
//  bpmControl.cpp
//  lightSequencer
//
//  Created by Tim TVL on 12.01.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "bpmControl.h"

bpmControl::bpmControl() {
    
    BPM = 120.0f;

    midiSyncCC  = 46;
	midiResetCC = 60;
    midiUpBpmCC = 61;
    midiDownBpmCC = 62;
    
    verdana.loadFont("../data/verdana.ttf", 14, true, true);
	verdana.setLineHeight(14.0f);
	verdana.setLetterSpacing(1.035);
}

bpmControl::~bpmControl() {
    midiIn.closePort();
    midiOut.closePort();
};

void bpmControl::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei){

    leftX = x; leftY = y; width = wid; height = hei;
    rightX = leftX + width; rightY = leftY + height;

    Sync.setup(leftX+width/4-50/2, leftY + 10 , 50, 30, true);
	Sync.setActictiveText("SYNC");

    Reset.setup(rightX-width/4-50/2, leftY+10, 50, 30, false);
	Reset.setActictiveText("reset");
    
    UpBpm.setup(rightX-width/4-30/2, leftY + 10 , 30, 20, false);
//    UpBpm.setActictiveText("˰");
    DownBpm.setup(rightX-width/4-30/2, leftY + 40 , 30, 20, false);
//   	DownBpm.setActictiveText("˯");
}

void bpmControl::processFrame(float* frame, int framesize){
    liveAudioAnalysis.processFrame(frame, framesize);
    if (liveAudioAnalysis.btrack.playbeat == 1){
        liveAudioAnalysis.lastBeatTime = liveAudioAnalysis.aubioOnsetDetectionVector.size()-1;
    }
}

void bpmControl::draw(){

    Sync.draw();
    Reset.draw();
//    UpBpm.draw();
//    DownBpm.draw();
    if (Sync.isOn) BPM = liveAudioAnalysis.bpmEstimate;

    if (Sync.isOn && (liveAudioAnalysis.aubioOnsetDetectionVector.size() - liveAudioAnalysis.lastBeatTime) < 10){
		ofSetHexColor(0xFFFFFF);
		ofCircle(leftX+width/4+50, leftY + 22, 10);
	}
    liveAudioAnalysis.drawBeatTimesSeconds(liveAudioAnalysis.aubioOnsetDetectionVector);
}

bool bpmControl::isClicked(int x, int y){
//    printf("isClicked");
    Sync.isClicked(x, y);
    if (Reset.isClicked(x, y)) liveAudioAnalysis.clearVectors() ;
}

void bpmControl::setupMidi(unsigned int ident, unsigned int channel, unsigned int inPort, unsigned int outPort){ 
	midiChannel = channel;
    midiInPort = inPort, midiOutPort = outPort;
    if (midiInPort != 100) { // do not turn on trick
        midiIn.openPort(midiInPort);
        midiOut.openPort(midiOutPort);
        midiIn.addListener(this);
        Sync.setupMidi(midiSyncCC, 1, midiInPort, midiOutPort);
        Sync.setMidiActive(true);
        Reset.setupMidi(midiResetCC, 1, midiInPort, midiOutPort); 
        Reset.setMidiActive(true);
    }
}

void bpmControl::newMidiMessage(ofxMidiMessage &args){
	if (midiChannel == args.channel){

//            if (args.byteOne) printf("BPM CHAN: %i ID: %i VALUE: %i \n", args.channel, args.byteOne, args.byteTwo) ;

    }
}



