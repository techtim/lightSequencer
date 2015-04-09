//
//  MotionControl.cpp
//  lightSequencerDmx
//
//  Created by Tim TVL on 07/04/15.
//
//

#include "MotionControl.h"

MotionControl::MotionControl():
slidersLen(127), slidersNum(16), matrixSpace(2), cntr(0)
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
        sliders->addSlider(ofToString(i+1), 0, 255, slidersValues[i], 10, slidersLen);
//        sliders->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_WIDGET_POSITION_UP);
//        sliders->addToggle(ofToString(i+1), false);
    }
    sliders->autoSizeToFitWidgets();
    sliders->setColorBack(ofColor(0,0,0,0));
    sliders->enable();

    matrixCellSize = (wid-20)/cols - matrixSpace;
    //    matrixCellSize = (width-20)/matrixW*0.7 - matrixSpace ;
    matrixCellSize =
        matrixCellSize > ((hei/4)/rows - matrixSpace) ?
        ((hei/4)/rows - matrixSpace) : matrixCellSize;

    LEDS::set(cols, rows, matrixCellSize, posX+20, posY+slidersLen+20, matrixSpace);
    
    
    vector<float> tiltBuffer;
    for(int i = 0; i < 256; i++)
    {
        tiltBuffer.push_back(sin(i/255.f));

    }

    waves = new ofxUISuperCanvas("waves", posX, LEDS::yRight, 15, slidersLen, OFX_UI_FONT_SMALL);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_WIDGET_POSITION_UP);
    waves->addNumberDialer("channel", 1, 20, &tiltChannel, 0);
    waves->addNumberDialer("speed", 1, 32, &tiltSpeed, 0);
    waves->addNumberDialer("wave", 1, 3, &tiltWave, 0);
    
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN, OFX_UI_WIDGET_POSITION_LEFT);
    waves->addRangeSlider("", 0, 255, &tiltFrom, &tiltTo, 10, slidersLen);
    waves->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT, OFX_UI_WIDGET_POSITION_UP);
    waves->addMovingGraph("tilt", tiltBuffer, 256, 0, 255, wid, slidersLen);
//    (string _name, vector<float> _buffer, int _bufferSize, float _min, float _max, float _w, float _h)
    waves->autoSizeToFitWidgets();
    waves->setColorBack(ofColor(0,0,0,0));
    
    ofAddListener(ofEvents().mousePressed, this, &MotionControl::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &MotionControl::mouseDragged);
    ofAddListener(ofEvents().keyReleased, this, &MotionControl::keyPressed)
}

void MotionControl::update(float lenOfQuarterBeatInSamples, int quarterBeatCount) {
    lengthOfQuarterBeatInSamples = lenOfQuarterBeatInSamples;
    quarterBeatCounter = quarterBeatCount;

    float val;
    switch (static_cast<int>(tiltWave)) {
        case 1:
            val = sin(PI*quarterBeatCounter*tiltSpeed/256.f);
            break;
        case 2:
            val = squareWave(PI, quarterBeatCounter*tiltSpeed/256.f);
            break;
        default:
            break;
    }
    
    val=tiltFrom+(val+1)*(tiltTo-tiltFrom)/2;
    if (val>255) val = 255;
    ((ofxUIMovingGraph*)waves->getWidget("tilt"))->addPoint(val);

}

void MotionControl::draw() {
    
    vector<float> buf =((ofxUIMovingGraph*)waves->getWidget("tilt"))->getBuffer();

    sliders->draw();
    LEDS::print();
    waves->draw();
    ((ofxUIMovingGraph*)waves->getWidget("tilt"))->drawFill();

}


void MotionControl::mousePressed(ofMouseEventArgs & args){
    if (!region.inside(args.x, args.y)) return;
    
    if (LEDS::isClicked(args.x, args.y))
    {
        //printf("MATRIX CLICKED\n");
        LEDS::setClicked(args.x, args.y, ofColor(255));
        LEDS::updateColor(ofColor(255));
    }
}

void MotionControl::mouseDragged(ofMouseEventArgs & args){
    if (!region.inside(args.x, args.y)) return;
    
    if (LEDS::isClicked(args.x, args.y))
    {
        //printf("MATRIX CLICKED\n");
        LEDS::setClicked(args.x, args.y, ofColor(255), true);
        LEDS::updateColor(ofColor(255));
    }
}

void MotionControl::guiEvent(ofxUIEventArgs &e){
    
//    if(e.widget == typesList){
//        //        if(typesList->isOpen()){
//        ////			typesList->setVisible(false);
//        //        }
//        //        else {
//        typesList->setVisible(true);
//        if(typesList->getSelected().size() > 0){
//            string selectedTypetName = typesList->getSelected()[typesList->getSelected().size()-1]->getName();
//            if(selectedTypetName == "RGB"){
//                //                shouldCreateNewProject = true;
//                dmxType = LED_DMX_RGB;
//            }
//            else if(selectedTypetName == "RGB+A"){
//                dmxType = LED_DMX_RGBA;
//            }
//            else if(selectedTypetName == "A+RGB"){
//                dmxType = LED_DMX_ARGB;
//            }
//            else if(selectedTypetName == "RGB+W"){
//                dmxType = LED_DMX_RGBW;
//            }
//            else if(selectedTypetName == "W"){
//                dmxType = LED_DMX_W;
//            }
//            typesList->setName(selectedTypetName);
//            typesList->clearSelected();
//            typesList->close();
//        }
//    }
}

float MotionControl::squareWave(float freq, float t) {
    
    return sin(freq*t)+sin(3*freq*t)/3+sin(5*freq*t)/5+sin(7*freq*t)/7 > 0 ? 1 : -1;
    
};

void MotionControl::keyPressed(ofKeyEventArgs & k){
    switch(k.key) {
        case 's':
            sliders->saveSettings("MotionSliders16");
            break;
            
        case OF_KEY_SHIFT:
            
            break;
        case ' ':
            
            break;
    }
}