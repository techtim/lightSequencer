#ifndef _LED
#define _LED

#include "Config.h"
#include "ofMain.h"
#include "ofColor.h"
#include "ofTexture.h"
#include "ofxUI.h"
#include "ofxDmx.h"

const std::string dt[5] = {"RGB","RGB+A","A+RGB","RGB+W","W"};
const std::vector<std::string> dmxTypes(dt, dt+5);


class LED {
    
public:
    LED();
    LED(int xL, int yL, int xR, int yR);
    
    void set(int xLeft, int yLeft, int xRight, int yRight);
    void setColor(ofColor col) {
        color = col;
    };
    
    void setADSR(const ofVec3f & adsr);
    void setADSR(const unsigned int & _att, const unsigned int & _dec,const unsigned int & _rel, const unsigned int & _min=0, const unsigned int & _max=255);
    void trigADSR();
    
    void setNumInChain(unsigned int num) {
        numInChain = num;
    }

    void setDmxAddress(unsigned int num);
    void setDmxColorOffset(unsigned int offset);
    void setupDmxGui();
    void setEdit(bool _bedit);
    bool isEdit();

    void setDmxType(ledDmxType t);
    void setDmxType(int t);
    void getDmx(ofxDmx &dmx);
    
    void update();

    void getNumInChain() {
        return numInChain;
    }
    void position(int xL, int yL, int xR, int yR);
    bool isClicked(int x, int y);
    
    LED operator=(const LED led) {
        isSelected = led.isSelected;
        numInChain = led.numInChain;
        color.set(led.color);
        dmxStartAddress = led.dmxStartAddress;
        dmxColorOffset = led.dmxColorOffset;
        numRow = led.numRow;
        numColumn = led.numColumn;
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
    
    ofxUISuperCanvas* gui;
    ofxUINumberDialer* dmxAddr;
    ofxUINumberDialer* dmxColorOff;
    ofxUIDropDownList* typesList;
    
	void guiEvent(ofxUIEventArgs &e);

    void showGui(bool bShow);
    bool bShowGui;
    bool bDmxSetup;
    bool bEdit;

    unsigned int dmxStartAddress, dmxColorOffset;
    
    ledDmxType dmxType;
    float fDmxAddress, fDmxColorOffset;
};

#endif
