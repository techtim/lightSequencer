//
//  Header.h
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 19.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lightSequencer_Header_h
#define lightSequencer_Header_h
#include "ofMain.h"
#include "LEDS.h"
#include "ofxButton.h"

class FX : public LEDS {
public:
    FX();
    ~FX();
    
    void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int col, unsigned int row);
    void setupMidi() {;}
    
    void draw();
    bool isClicked(int x, int y);
    void process();
    
    unsigned int width, height, length, columns, rows;
    unsigned char * bitmap;
    bool setupFinished, isOn;
    ofxButton invertButton, mirrorButton, moveLeftButton, moveRightButton;

    ofColor tmpColor;
    bool tmpSelected;
    
    void invert() {
        if (!setupFinished || !invertButton.isOn) return;
//        printf("IVERT");
        for (int i=0; i<length; i++) {
            leds[i].color = leds[i].color.getHex() == 0 ? color : tmpColor.black;
            leds[i].isSelected = !leds[i].isSelected;
        }
    }
    
    void mirror() {
        if (!setupFinished || !mirrorButton.isOn) return;
        for (int i=0; i<length/2; i++) {
            tmpColor = leds[i].color;
            tmpSelected = leds[i].isSelected;
            leds[i].isSelected = leds[length-i-1].isSelected;
            leds[i].color = leds[length-i-1].color;
            leds[length-i-1].color = tmpColor;
            leds[length-i-1].isSelected = tmpSelected;
//            printf("COLOR:%X, SELECT: %i \n", leds[i].color.getHex(), (leds[i].isSelected?1:0) );
//            }
        }
    }
    
    void moveLeft() {
        if (!setupFinished || !moveLeftButton.isOn) return;
        //        printf("IVERT");
        for (int i=0; i<length; i++) {
            leds[i].color = leds[i].color.getHex() == 0 ? color : tmpColor.black;
            leds[i].isSelected = !leds[i].isSelected;
        }
    }
    void moveRight() {
        if (!setupFinished || !moveRightButton.isOn) return;
        //        printf("IVERT");
        for (int i=0; i<length; i++) {
            leds[i].color = leds[i].color.getHex() == 0 ? color : tmpColor.black;
            leds[i].isSelected = !leds[i].isSelected;
        }
    }


};

#endif
