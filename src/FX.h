//
//  Header.h
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 19.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef dmxSequencer_FX_h
#define dmxSequencer_FX_h
#include "ofMain.h"
#include "LEDS.h"
#include "ofxButton.h"
#include "ofxUI.h"

class FX : public LEDS {
public:
    FX();
    ~FX();
    
    void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int col, unsigned int row);
    void setupMidi() {;}
    
    void draw();
    bool isClicked(int x, int y);
    void process();
    
    void guiEvent(ofxUIEventArgs &e);
    
    unsigned int width, height, length;
    unsigned char * bitmap;
    bool setupFinished, isOn;
    unsigned int numEffects;
    unsigned int moveStep;
    bool isEvenStep;
    
    //    ofxUICanvas* gui;
    ofxUISuperCanvas* gui;
    ofxUILabelToggle* invertButton;
    ofxUILabelToggle* mirrorButton;
    ofxUILabelToggle* moveLeftButton;
    ofxUILabelToggle* moveRightButton;
    ofxUILabelToggle* moveUpButton;
    ofxUILabelToggle* moveDownButton;
    
    ofColor tmpColor;
    bool tmpSelected;
    
    void invert();
    void mirror();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void retrigger() { bRetriger = true; } ;
    
private:
    unsigned int columns, rows;
    unsigned int ledsNumber;
    bool bRetriger;
};

#endif
