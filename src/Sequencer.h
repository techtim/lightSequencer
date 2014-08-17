//
//  Sequencer.h
//  dmxSequencer
//
//  Created by Tim TVL on 24/03/14.
//
//

#ifndef __dmxSequencer__Sequencer__
#define __dmxSequencer__Sequencer__

#include "ofMain.h"

#include "ofxUI.h"
#include "ofxMidiIn.h"
#include "ofxMidiOut.h"
#include "LEDS.h"

class Sequencer : public LEDS {
public:
    Sequencer();
//    Sequencer(unsigned int stepsCnt, unsigned int x, unsigned int y, int width, int height,
//              unsigned int cell, unsigned int spac, unsigned int seqBitLen,
//              ofColor selColor, ofColor actColor, ofColor inactColor);
    ~Sequencer();
    void setup(unsigned int steps, unsigned int x, unsigned int y,
               unsigned int cell, unsigned int spac, unsigned int seqBitLen,
               ofColor selColor, ofColor actColor, ofColor inactColor);

    void guiEvent(ofxUIEventArgs &e);
    bool isClicked(int x, int y, bool dragged = false);
    bool isSelectedStep(int step);
    void setStep(int step, ofColor * feedBitmap);
    
    ofColor * getSequencedBitmap();
    unsigned char * getSequencedBitmapChar();
    unsigned int seqBitmapLen;
    unsigned char * sequencedBitmapChar;
    ofColor * sequencedBitmap;
	
    void draw();
    
    void setADSR(float _a, float _d, float _r);
    ofVec3f getADSR();

    ofxUICanvas* gui;
    ofxUICanvas* buttons;
    
    unsigned int quant;
    ofxUILabelToggle * button8;
    ofxUILabelToggle * button16;
    bool bQuant8, bQuant16;
    void setQuant8() {
        quant = 2;
        bQuant16    = false;
        bQuant8     = true;
        button8->setValue(true);
        button16->setValue(false);
        //        setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
        //            printf("A:%f D:%f R:%f", a,d,r);
    }
    
    void setQuant16() {
        quant = 1;
        bQuant16    = true;
        bQuant8     = false;
        button16->setValue(true);
        button8->setValue(false);
        //        setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
        //            printf("A:%f D:%f R:%f", a,d,r);
    }
    
    
    ofxUISuperCanvas * sliders;
	ofxUIMinimalSlider* attSlider;
    ofxUIMinimalSlider* decSlider;
    ofxUIMinimalSlider* relSlider;

//    ofxUISlider* attSlider;
//    ofxUISlider* decSlider;
//    ofxUISlider* relSlider;
    
    float a, d, s, r;
    unsigned int ADSRoffset;
    float ADSRvalue;

    unsigned int steps;
    unsigned int currentStep;
    
    unsigned int attack, decay, release;
    ofColor inactiveColor;
    ofColor activeColor;
    ofColor selectColor;
	
    //---MIDI---
    void setupMidi(unsigned int midiSeqStart, unsigned int channel, unsigned int InPort, unsigned int OutPort);
    void newMessage(ofxMidiEventArgs &args);
    ofxMidiIn	midiIn;
    ofxMidiOut	midiOut;
    unsigned int midiInPort, midiOutPort;
    unsigned int midiChannel; // channel for exact sequence , setuped
    unsigned int midiId;
    unsigned int midiValue;
    unsigned int midiButton8Id, midiButton16Id, midiAttSliderId, midiDecSliderId, midiRelSliderId;
    
    unsigned int midiActivationCC;
    unsigned int midiSeqStartCC;
    bool midiActive;
    void setMidiActive(bool is) {
        midiActive = is;
    }
    
    void setMidiActivationCC(unsigned int cc) {
        midiActivationCC = cc;
    }
    
    float mapNonLinear(float in, float inMin, float inMax, float outMin, float outMax, float shaper){
        // (1) convert to pct (0-1)
        float pct = ofMap (in, inMin, inMax, 0, 1, true);
        // raise this number to a power
        pct = powf(pct, shaper);
        float out = ofMap(pct, 0,1, outMin, outMax, true);
        return out;
    }
    
  private:
    bool setupFinished;
    
    int cellSize;
    unsigned int rows, columns;
    
    int stepTimestamp;
    int slidersOffset;
    ofVec2f buttonsPos;
};


#endif /* defined(__dmxSequencer__Sequencer__) */
