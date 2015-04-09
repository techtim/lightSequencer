//
//  MotionControl.h
//  lightSequencerDmx
//
//  Created by Tim TVL on 07/04/15.
//
//

#ifndef __lightSequencerDmx__MotionControl__
#define __lightSequencerDmx__MotionControl__

#include <stdio.h>
#include "ofxUI.h"
#include "LEDS.h"
#include "Utils.h"

class MotionControl: public LEDS {
public:
    MotionControl();
    ~MotionControl();
    
    void setup(const int & posX, const int & posY, const int & wid, const int & hei, const int &cols, const int &rows);
    void update(float lengthOfQuarterBeatInSamples, int quarterBeatCounter);
    void draw();
    
    void mousePressed(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void keyPressed(ofKeyEventArgs & k);

    void guiEvent(ofxUIEventArgs &e);
    void getDmx(ofxDmx &dmx);
    void setupLedDmx(vector<LED> &_leds);

    float squareWave(float freq, float t);
    
private:
    int slidersNum, slidersLen;
    int matrixCellSize, matrixSpace;
    float *slidersValues;

    float lengthOfQuarterBeatInSamples;
    int quarterBeatCounter;
//    std::shared_ptr<unsigned int> cntr;

    float cntr;
    float panChannel, panOffset, panSpeed, panWave, panFrom, panTo;
    float tiltChannel, tiltOffeset, tiltSpeed, tiltWave, tiltFrom, tiltTo;
    
    // --- UI ---
    ofxUIDropDownList* quantizeList;
    ofxUISuperCanvas * sliders;
    ofxUISuperCanvas * waves;
};


#endif /* defined(__lightSequencerDmx__MotionControl__) */
