#ifndef _LED
#define _LED

#include "Config.h"
#include "ofMain.h"
#include "ofColor.h"
#include "ofTexture.h"
#include "ofxUI.h"
#include "ofxDmx.h"


class LED {
    
public:
    LED();
    LED(int xL, int yL, int xR, int yR);
    
    void set(int xLeft, int yLeft, int xRight, int yRight);
    void setColor(ofColor col) {
        color = col;
    };
    
    void setADSR(ofVec3f adsr);
    void setADSR(unsigned int _att, unsigned int _dec, unsigned int _rel, unsigned int _min=0, unsigned int _max=255);
    void trigADSR();
    
    void setNumInChain(unsigned int num) {
        numInChain = num;
    }
    
    void setDmxAddress(unsigned int num) {
        dmxStartAddress = num;
    }
    
    void getNumInChain() {
        return numInChain;
    }
    
    void getDmx(ofxDmx &dmx);
    
    void update();
    
    void position(int xL, int yL, int xR, int yR);
    bool isClicked(int x, int y);
    
    LED operator=(const LED led) {
        isSelected = led.isSelected;
        numInChain = led.numInChain;
        color.set(led.color);
        //        color.setBrightness(0);
    }
    
    bool isSelected;
    int xLeft;
    int yLeft;
    int xRight;
    int yRight;
    int num;
    int numColumn;
    int numRow;
    int numInChain;
    float att, dec, sus, rel;
    
    unsigned int minValue, maxValue;
    
    float ADSRvalue;
    unsigned int ADSRoffset;
    
    ofColor color, dmxColor, alphaColor;
    
    ofxUISuperCanvas *gui;
	void guiEvent(ofxUIEventArgs &e);

    void showGui(bool bShow);
    bool bShowGui;
    unsigned int dmxStartAddress;
};

#endif
