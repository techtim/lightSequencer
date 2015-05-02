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
    
    void setEnable(bool bEnabled);
    
    void mousePressed(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void keyPressed(ofKeyEventArgs & k);

    void guiEvent(ofxUIEventArgs &e);
    void getDmx(ofxDmx &dmx);
    void setupDmxConfig(vector<LED> &_leds);

    float squareWave(float freq, float t);
    float triWave(float freq, float t);
private:
    int slidersNum, slidersLen;
    int matrixCellSize, matrixSpace;
    float *slidersValues;

    float lengthOfQuarterBeatInSamples;
    int quarterBeatCounter;
//    std::shared_ptr<unsigned int> cntr;

    float cntr, triangleDelta;
    float panChannel, panPhase, panSpeed, panWave, panFrom, panTo;
    float tiltChannel, tiltPhase, tiltSpeed, tiltWave, tiltFrom, tiltTo;
    
    bool bSavePreset;
    bool bEnable;

    LEDS tiltLeds;
    LEDS panLeds;
    
    // --- UI ---
    ofxUIDropDownList* quantizeList;
    ofxUISuperCanvas * sliders;
    ofxUISuperCanvas * waves;

    ofxUISuperCanvas * radios;
    ofxUILabelToggle * buttonSavePres;
    
    ofxUIMovingGraph* tiltGraph;
    ofxUIMovingGraph* panGraph;
};


#endif /* defined(__lightSequencerDmx__MotionControl__) */
