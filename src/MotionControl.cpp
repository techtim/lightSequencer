//
//  MotionControl.cpp
//  lightSequencerDmx
//
//  Created by Tim TVL on 07/04/15.
//
//

#include "MotionControl.h"

MotionControl::MotionControl():
slidersLen(85), slidersNum(16), matrixSpace(2), cntr(0), triangleDelta(0), bSavePreset(false), bEnable(true)
{
    slidersValues = new float[slidersNum]();
}

MotionControl::~MotionControl(){
    delete [] slidersValues;
}

void MotionControl::setup(const int &posX, const int &posY, const int &wid, const int &hei, const int &cols, const int &rows) {

    //    attSlider = new ofxUIMinimalSlider(_name, _min, _max, _value, rect->getWidth()-widgetSpacing*2, globalSliderHeight, 0, 0, size);
    sliders = new ofxUISuperCanvas("", posX, posY, 15, slidersLen, OFX_UI_FONT_SMALL);
    for (int i=0; i<slidersNum; i++) {
        sliders->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_WIDGET_POSITION_UP);
        sliders->addSlider(ofToString(i+1), 0, 255, &slidersValues[i], 10, slidersLen);
    }
    sliders->autoSizeToFitWidgets();
    sliders->setColorBack(ofColor(0,0,0,0));
    sliders->enable();

    matrixCellSize = 10;
//    matrixCellSize = (wid-20)/cols - matrixSpace;
    //    matrixCellSize = (width-20)/matrixW*0.7 - matrixSpace ;
//    matrixCellSize =
//        matrixCellSize > ((hei/4)/rows - matrixSpace) ?
//        ((hei/4)/rows - matrixSpace) : matrixCellSize;

    LEDS::set(cols, rows, matrixCellSize, posX+20, posY+slidersLen+20, matrixSpace);
    tiltLeds.set(cols, rows, matrixCellSize, posX+225, posY+slidersLen*2+30, matrixSpace);
    
    panLeds.set(cols, rows, matrixCellSize, posX+225, posY+slidersLen*3+60, matrixSpace);
    vector<float> tiltBuffer;
    for(int i = 0; i < 256; i++)
    {
        tiltBuffer.push_back(0.f);

    }

    waves = new ofxUISuperCanvas("WAVES", posX, LEDS::yRight, 15, slidersLen, OFX_UI_FONT_SMALL);
    waves->addSpacer(5);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_ALIGN_CENTER);
    waves->addLabel("TILT");
    waves->addNumberDialer("chan", 1, 20, &tiltChannel, 0);
    waves->addNumberDialer("speed", 1, 32, &tiltSpeed, 0);
    waves->addNumberDialer("wave", 1, 4, &tiltWave, 0);
    waves->addNumberDialer("phase", 0, 256/rows, &tiltPhase, 0);
    
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_ALIGN_LEFT);
    waves->addRangeSlider("", 0, 255, &tiltFrom, &tiltTo, 10, slidersLen);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_ALIGN_FREE);
    tiltGraph = waves->addMovingGraph("tilt", tiltBuffer, 256, 0, 255, wid, slidersLen);
//    (string _name, vector<float> _buffer, int _bufferSize, float _min, float _max, float _w, float _h)

    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_ALIGN_FREE);
    waves->addSpacer(10);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_ALIGN_CENTER);
    waves->addLabel("PAN");
    waves->addNumberDialer("chan", 1, 20, &panChannel, 0);
    waves->addNumberDialer("speed", 1, 32, &panSpeed, 0);
    waves->addNumberDialer("wave", 1, 4, &panWave, 0);
    waves->addNumberDialer("phase", 0, 256/rows, &panPhase, 0);

    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_ALIGN_LEFT);
    waves->addRangeSlider("", 0, 255, &panFrom, &panTo, 10, slidersLen);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_ALIGN_FREE);
    panGraph = waves->addMovingGraph("pan", tiltBuffer, 256, 0, 255, wid, slidersLen);
    
    waves->autoSizeToFitWidgets();
    waves->setColorBack(ofColor(0,0,0,0));
    
    radios = new ofxUISuperCanvas("SCENES", posX, ofGetHeight()-70, 15, wid, OFX_UI_FONT_SMALL);
    vector<string> names;
    for (int i=0; i<10; i++) names.push_back(ofToString(i+1));
    radios->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_ALIGN_FREE);
    radios->addRadio("scenes", names, OFX_UI_ORIENTATION_HORIZONTAL);
    radios->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_ALIGN_FREE);
    buttonSavePres = radios->addLabelToggle("SAVE", &bSavePreset);
    radios->autoSizeToFitWidgets();
    radios->setColorBack(ofColor(0,0,0,0));
    
    sliders->loadSettings("MotionSliders16");
    waves->loadSettings("MotionWaves");
    
    ofAddListener(radios->newGUIEvent,this,&MotionControl::guiEvent);
    
    ofAddListener(ofEvents().mousePressed, this, &MotionControl::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &MotionControl::mouseDragged);
    ofAddListener(ofEvents().keyReleased, this, &MotionControl::keyPressed);
}

void MotionControl::setEnable(bool bEnabled) {
    bEnable = bEnabled;
    if (bEnable) {
        sliders->enable();
        waves->enable();
        radios->enable();
    } else {
        sliders->disable();
        waves->disable();
        radios->disable();
    }
        
}

void MotionControl::update(float lenOfQuarterBeatInSamples, int quarterBeatCount) {
    lengthOfQuarterBeatInSamples = lenOfQuarterBeatInSamples;
    quarterBeatCounter = quarterBeatCount;

    if (!bEnable) return;

    float val;
    switch (static_cast<int>(tiltWave)) {
        case 1:
            val = sin(PI*quarterBeatCounter*tiltSpeed/256.f);
            break;
        case 2:
            val = cos(PI*quarterBeatCounter*tiltSpeed/256.f);
            break;
        case 3:
            val = squareWave(PI, quarterBeatCounter*tiltSpeed/256.f);
            break;
        case 4:
            val = triWave(PI, quarterBeatCounter*tiltSpeed/256.f);
            break;
        default:
            break;
    }
    val=tiltFrom+(val+1)*(tiltTo-tiltFrom)/2;
    if (val>255) val = 255;
    ((ofxUIMovingGraph*)waves->getWidget("tilt"))->addPoint(val);
    
    switch (static_cast<int>(panWave)) {
        case 1:
            val = sin(PI*quarterBeatCounter*panSpeed/256.f);
            break;
        case 2:
            val = cos(PI*quarterBeatCounter*panSpeed/256.f);
            break;
        case 3:
            val = squareWave(PI, quarterBeatCounter*panSpeed/256.f);
            break;
        case 4:
            val = triWave(PI, quarterBeatCounter*panSpeed/256.f);
            break;
        default:
            break;
    }
    val=panFrom+(val+1)*(panTo-panFrom)/2;
    if (val>255) val = 255;
    ((ofxUIMovingGraph*)waves->getWidget("pan"))->addPoint(val);

}

void MotionControl::draw() {

    if (!bEnable) return;
//    vector<float> buf =((ofxUIMovingGraph*)waves->getWidget("tilt"))->getBuffer();

    sliders->draw();
    LEDS::print();
    waves->draw();
    tiltLeds.print();
    panLeds.print();
    radios->draw();
    ((ofxUIMovingGraph*)waves->getWidget("tilt"))->drawFill();

}


void MotionControl::mousePressed(ofMouseEventArgs & args){
//    if (!region.inside(args.x, args.y)) return;
    
    if (LEDS::isClicked(args.x, args.y))
    {
        LEDS::setClicked(args.x, args.y, ofColor(255));
        LEDS::updateColor(ofColor(255));
    }
    if (tiltLeds.isClicked(args.x, args.y))
    {
        tiltLeds.setClicked(args.x, args.y, ofColor(255));
        tiltLeds.updateColor(ofColor(255));
    }
    if (panLeds.isClicked(args.x, args.y))
    {
        panLeds.setClicked(args.x, args.y, ofColor(255));
        panLeds.updateColor(ofColor(255));
    }
    
}

void MotionControl::mouseDragged(ofMouseEventArgs & args){
//    if (!region.inside(args.x, args.y)) return;
    
    if (LEDS::isClicked(args.x, args.y))
    {
        LEDS::setClicked(args.x, args.y, ofColor(255), true);
        LEDS::updateColor(ofColor(255));
    }
    if (tiltLeds.isClicked(args.x, args.y))
    {
        tiltLeds.setClicked(args.x, args.y, ofColor(255), true);
        tiltLeds.updateColor(ofColor(255));
    }
    if (panLeds.isClicked(args.x, args.y))
    {
        panLeds.setClicked(args.x, args.y, ofColor(255), true);
        panLeds.updateColor(ofColor(255));
    }

}

void MotionControl::guiEvent(ofxUIEventArgs &e){
    
    if(e.widget->getName() == "SCENES"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
    }
}


void MotionControl::setupDmxConfig(vector<LED> &_leds) {
    leds = _leds;
    tiltLeds.leds = _leds;
    panLeds.leds = _leds;
}

void MotionControl::getDmx(ofxDmx &dmx) {
    for (int i=0; i<rows*columns; i++) {
        if (leds[i].isSelected) {
            for (int j=0; j<slidersNum; j++) {
                if (slidersValues[j] > 0.f){
                    dmx.setLevel( leds[i].dmxStartAddress+j, static_cast<unsigned char>(slidersValues[j]));
                }
            }
        }
    }
    for (int i=0; i<rows*columns; i++) {
        if(tiltLeds.leds[i].isSelected) {
            dmx.setLevel( tiltLeds.leds[i].dmxStartAddress+tiltChannel-1, static_cast<unsigned char>(tiltGraph->getBuffer()[tiltPhase*tiltLeds.leds[i].numColumn]) );
        }
    }
    for (int i=0; i<rows*columns; i++) {
        if(panLeds.leds[i].isSelected) {
            dmx.setLevel( panLeds.leds[i].dmxStartAddress+panChannel-1, static_cast<unsigned char>(panGraph->getBuffer()[panPhase*panLeds.leds[i].numColumn]) );
        }
    }
}

// --------------------

float MotionControl::squareWave(float freq, float t) {
    
    return sin(freq*t)+sin(3*freq*t)/3+sin(5*freq*t)/5+sin(7*freq*t)/7 >= 0 ? 1 : -1;
    
};

float MotionControl::triWave(float freq, float t) {
    return -1+t*2;
};

void MotionControl::keyPressed(ofKeyEventArgs & k){
    switch(k.key) {
        case 's':
            sliders->saveSettings("MotionSliders16");
            waves->saveSettings("MotionWaves");
            break;
            
        case OF_KEY_SHIFT:
            
            break;
        case ' ':
            
            break;
    }
}