#include "LED.h"

LED::LED()
{
    color.set(0, 0, 0, 0);
    numInChain = 0; // don't sent to LED chain
//    ADSRvalue = 0;
//    ADSRoffset = 0;
    bShowGui = false;
    bDmxSetup = false;
    minValue = 0;
    maxValue = 255;
    ADSRoffset = 0;
    dmxStartAddress = 0;
}

LED::LED(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
    
//    dmxStartAddress = 1 + numInChain * DMX_CHANNELS_IN_PAR;

};

void LED::position(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
};

void LED::trigADSR() {
    ADSRoffset = 0;
}

void LED::setADSR(const ofVec3f & adsr){
    setADSR(adsr.x, adsr.y, adsr.z);
}

void LED::setADSR(const unsigned int & _att, const unsigned int & _dec,const unsigned int & _rel, const unsigned int & _min, const unsigned int & _max){
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
//        printf("isSelected=%i; num=%i",isSelected, num);
        //        printf("X=%i; Y=%i",x, y);
        return true;
    }
    return false;
};

void LED::setEdit(bool _bEdit){
    bEdit = _bEdit;
}

bool LED::isEdit() {
    return bEdit;
}

void LED::setDmxAddress(unsigned int num) {
    
    dmxStartAddress = num;
    setupDmxGui();
}

void LED::setupDmxGui() {
    gui = new ofxUISuperCanvas("led-"+ofToString(dmxStartAddress),OFX_UI_FONT_SMALL);
    //    gui->setGlobalSliderHeight(5);

    fDmxAddress = static_cast<float>(dmxStartAddress);
    dmxAddr = new ofxUINumberDialer(0, 512, &fDmxAddress, 0, "addr", OFX_UI_FONT_SMALL);
    dmxType = LED_DMX_RGB;
//    gui->addNumberDialer("addr", 1, 512, dmxStartAddress, 3)->setDrawOutline(true);
    gui->addWidgetDown(dmxAddr);

    typesList = new ofxUIDropDownList(70, "RGB", dmxTypes, OFX_UI_FONT_SMALL);

    typesList->activateToggle("RGB");
//    typesList->setShowCurrentSelected(true);
    gui->addWidgetDown(typesList);
    
//    gui->loadSettings("led-"+ofToString(dmxStartAddress));
    gui->autoSizeToFitWidgets();
    gui->setWidth(75);
//    gui->setAutoUpdate(true);
    gui->setPosition(ofGetWidth()/2-gui->getRect()->width/2, ofGetHeight()/2);
    gui->disable();

    ofAddListener(gui->newGUIEvent, this, &LED::guiEvent);
    bDmxSetup = true;
}

void LED::showGui(bool bShow) {
    if (!bDmxSetup) return;
    bShowGui = bShow;
    bShowGui ? gui->enable() : gui->disable();
}

void LED::setDmxType(ledDmxType t) {
    dmxType = t;
    typesList->setName(dmxTypes[(int)dmxType]);
    typesList->activateToggle(dmxTypes[(int)dmxType]);
}

void LED::setDmxType(int t) {
    switch (t) {
        case 0:
            setDmxType(LED_DMX_RGB);
            break;
        case 1:
            setDmxType(LED_DMX_RGBA);
            break;
        case 2:
            setDmxType(LED_DMX_ARGB);
            break;
        case 3:
            setDmxType(LED_DMX_RGBW);
            break;
        case 4:
            setDmxType(LED_DMX_W);
            break;
        default:
            break;
    }
}

void LED::getDmx(ofxDmx &dmx) {
    if (dmxStartAddress == 0) return;

    if (dmxColor != color) {
        switch (dmxType) {
            case LED_DMX_RGB:
                dmx.setLevel(dmxStartAddress+0, color.r);
                dmx.setLevel(dmxStartAddress+1, color.g);
                dmx.setLevel(dmxStartAddress+2, color.b);
                break;
            case LED_DMX_RGBA:
                dmx.setLevel(dmxStartAddress+0, color.r);
                dmx.setLevel(dmxStartAddress+1, color.g);
                dmx.setLevel(dmxStartAddress+2, color.b);
                dmx.setLevel(dmxStartAddress+3, color.a);
                break;
            case LED_DMX_ARGB:
                dmx.setLevel(dmxStartAddress+0, color.a);
                dmx.setLevel(dmxStartAddress+1, color.r);
                dmx.setLevel(dmxStartAddress+2, color.g);
                dmx.setLevel(dmxStartAddress+3, color.b);
                break;
            case LED_DMX_RGBW:
                dmx.setLevel(dmxStartAddress+0, color.r);
                dmx.setLevel(dmxStartAddress+1, color.g);
                dmx.setLevel(dmxStartAddress+2, color.b);
                dmx.setLevel(dmxStartAddress+3, 0);
                break;
            case LED_DMX_W:
                dmx.setLevel(dmxStartAddress+0, ADSRvalue);
                break;

            default:
                break;
        }
            //            ofLog(OF_LOG_VERBOSE, paramNames[i]+" = "+ofToString(dmxValues[i]));
        dmxColor = color;
    }
}

void LED::guiEvent(ofxUIEventArgs &e){

    if(e.widget == typesList){
//        if(typesList->isOpen()){
////			typesList->setVisible(false);
//        }
//        else {
            typesList->setVisible(true);
            if(typesList->getSelected().size() > 0){
                string selectedTypetName = typesList->getSelected()[typesList->getSelected().size()-1]->getName();
                if(selectedTypetName == "RGB"){
    //                shouldCreateNewProject = true;
                    dmxType = LED_DMX_RGB;
                }
                else if(selectedTypetName == "RGB+A"){
                    dmxType = LED_DMX_RGBA;
                }
                else if(selectedTypetName == "A+RGB"){
                    dmxType = LED_DMX_ARGB;
                }
                else if(selectedTypetName == "RGB+W"){
                    dmxType = LED_DMX_RGBW;
                }
                else if(selectedTypetName == "W"){
                    dmxType = LED_DMX_W;
                }
                typesList->setName(selectedTypetName);
                typesList->clearSelected();
                typesList->close();
            }
//        }
    }
    else if(e.widget == dmxAddr) {
        dmxStartAddress = dmxAddr->getValue();
//        gui->set"led-"+ofToString(dmxStartAddress
    }
}