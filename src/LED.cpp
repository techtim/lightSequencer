#include "LED.h"

LED::LED()
{
    color.set(0, 0, 0, 0);
    numInChain = 0; // don't sent to LED chain
//    ADSRvalue = 0;
//    ADSRoffset = 0;
    bShowGui = false;
    minValue = 0;
    maxValue = 255;
    ADSRoffset=0;
}

LED::LED(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
    
    dmxStartAddress = 1 + numInChain * DMX_CHANNELS_IN_PAR;
    gui = new ofxUISuperCanvas("led-"+ofToString(dmxStartAddress));
//    gui->setGlobalSliderHeight(5);
//
//    gui->addNumberDialer("DMX CHANNEL", 1, 512, dmxStartAddress, 3)->setDrawOutline(true);
//    gui->loadSettings("led-"+ofToString(dmxStartAddress));
//    gui->autoSizeToFitWidgets();
////    gui->disableAppDrawCallback();
////    gui->setAutoUpdate(true);
//    gui->setPosition(ofGetWidth()/2-gui->getRect()->width/2, ofGetHeight()/2);
    gui->disable();
};

void LED::position(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
};

void LED::trigADSR() {
    ADSRoffset = 0;
}

void LED::setADSR(ofVec3f adsr){
    setADSR(adsr.x, adsr.y, adsr.z);
}

void LED::setADSR(unsigned int _att, unsigned int _dec, unsigned int _rel, unsigned int _min, unsigned int _max){
    att = _att, dec = _dec, rel = _rel, sus = 0, minValue = _min, maxValue = _max;
}

void LED::update(){
    if (att == 0 && dec == 0 && rel ==0 ) {
        ADSRvalue = minValue;
    } else if (ADSRoffset<=att){ // attack
		ADSRvalue += maxValue/att;
        if (ADSRvalue > maxValue) ADSRvalue = maxValue;
	} else if (ADSRoffset>att && ADSRoffset<=att+dec) { // decay
 		ADSRvalue = maxValue;
//		ADSRoffset++;
	} else if (ADSRoffset<=rel+att+dec){ // release
//		(ADSRvalue == 0.0 || ADSRvalue == minValue ? ADSRvalue = maxValue : (unsigned int)(ADSRvalue -= maxValue/rel));
        ADSRvalue -= maxValue/rel;
        if (ADSRvalue < minValue) ADSRvalue = minValue;
	} else {
		ADSRvalue = minValue;
	}

//    if (ADSRvalue != 0) ofLog(OF_LOG_NOTICE, "ADSRoffset="+ofToString(ADSRoffset)+"  ADSRvalue="+ofToString(ADSRvalue));

//    color.setBrightness(ADSRvalue);
    color.a = ADSRvalue;
    
	if (ADSRoffset < 1000) ADSRoffset++;
}

bool LED::isClicked(int x, int y) {
    if (y <= yRight && y >= yLeft && x <= xRight && x >= xLeft) {
        isSelected = (isSelected ? false : true);
        printf("isSelected=%i; num=%i",isSelected, num);
        //        printf("X=%i; Y=%i",x, y);
        return true;
    }
    return false;
};

void LED::showGui(bool bShow) {
    bShowGui = bShow;
    bShowGui ? gui->enable() : gui->disable();
}


void LED::getDmx(ofxDmx &dmx) {
    
    if (dmxColor != color) {

        dmx.setLevel(dmxStartAddress+0, color.r);
        dmx.setLevel(dmxStartAddress+1, color.g);
        dmx.setLevel(dmxStartAddress+2, color.b);
        dmx.setLevel(dmxStartAddress+3, 255);
            //            ofLog(OF_LOG_VERBOSE, paramNames[i]+" = "+ofToString(dmxValues[i]));
        dmxColor = color;
    }
}