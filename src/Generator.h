//
//  Generator.h
//  lightSequencer
//
//  Created by Tim_Tavli on 01.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lightSequencer_Generator_h
#define lightSequencer_Generator_h

#include "ofMain.h"
#include "colorPicker.h"
#include "LEDS.h"
#include "Sequencer.h"	
#include "ofxMidi.h"
#include "FX.h"

class Generator {
    ofColor hueColor, selectColor, actColor, inactColor;

    // sequencer
	unsigned int maxSteps, limitSteps, seqCellSize, seqCellSpace;
    // view
   	unsigned int width, height, leftX, leftY, atBottom;
   	unsigned int matrixW, matrixH, matrixCellSize, matrixSpace;
    ofRectangle region;

    ofVec2f matrixOffset;
    // midi
    unsigned int midiHue, midiSeqActivationStartCC, midiSeqBeginCC, midiLedMatrixActivationCC;
    unsigned int midiInPort, midiOutPort, midiInChannel;
    unsigned int midiValue, midiId;
    bool active;
    bool matrixSequenceMode; // true if Matrix midi read active mode
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;

    ofxUISuperCanvas* gui;
    ofxUIRangeSlider* volume;
    float minVolume, maxVolume;

public:
    Generator();
    ~Generator();
        
    void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, 
               unsigned int matW, unsigned int matH, bool at_bottom, unsigned int id);
    void setupMidi(unsigned int inPort, unsigned int outPort, unsigned int seqActivCC, unsigned int seqBeginCC, unsigned int ledMatrixActivCC, unsigned int hue, unsigned int midiChannel = 1);
    void receiveMidi(ofxMidiEventArgs &args);

    void setActive(bool to) {
        active = to;
        if (!to) ledMatrix.setMidiActive(to);
        Sequencer.setMidiActive(to);
    }
    
    const bool isActive() {
        return active;
    }
    
    void draw(unsigned int quarterBeatCounter);

    void mousePressed(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);

    ofColor * getSequencedBitmap() {
        return outputMatrix.getBitmap();
    }
    
    ofxUISuperCanvas* getGui() { return gui; };
    colorPicker hueLine;
    Sequencer Sequencer;
    LEDS ledMatrix, sequencedMatrix, outputMatrix;
    FX effects;
    unsigned int id;
    
    ofEvent<ofMouseEventArgs> mousePress;
    ofEvent<ofMouseEventArgs> mouseDragg;

};

#endif
