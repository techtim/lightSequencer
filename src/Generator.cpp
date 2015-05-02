//
//  Generator.cpp
//  lightSequencer
//
//  Created by Tim_Tavli on 01.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ofMain.h"
#include "ofEvents.h"
#include "Generator.h"


Generator::Generator(){
    midiLedMatrixActivationCC = 45;

    selectColor.set(0,200,50);
	actColor.set(100, 50, 200);
	inactColor.set(0, 0, 0);
	maxSteps = 16;
	limitSteps = 128;
	seqCellSize = 40;
	seqCellSpace = 5;
    minVolume = 0;
    maxVolume = 0;
    active = false;
}

Generator::~Generator() {
    ofRemoveListener(ofEvents().mousePressed, this, &Generator::mousePressed);
}

void Generator::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, 
                      unsigned int matW, unsigned int matH, bool at_bottom, unsigned int gen_id) {
    id = gen_id;
    // view params
    leftX = x, leftY = y, width = wid, height = hei;
    matrixW = matW, matrixH = matH;
    atBottom = at_bottom;
    matrixSpace = 2;
    matrixCellSize = (width-20)/matrixW - matrixSpace;
//    matrixCellSize = (width-20)/matrixW*0.7 - matrixSpace ;
    matrixCellSize =
        matrixCellSize > ((height/3)/matrixH - matrixSpace) ?
            ((height/3)/matrixH - matrixSpace) : matrixCellSize;
//    matrixCellSize = height/6/matrixH - matrixSpace;
    seqCellSpace = 2;
    seqCellSize = (width*.8 - maxSteps/2*seqCellSpace)/(maxSteps/2+1);
                   
    float seqPixelWidth = maxSteps/2 * (seqCellSize + seqCellSpace) + seqCellSize; // one more cell for quantisation
    float matrixPixelWidth = (matrixSpace+matrixCellSize)*matrixW-matrixSpace;
//    int matrixPixelWidth = width/2-40/matrixW;
//    leftY += atBottom ? (matrixCellSize+matrixSpace)*matrixH : 0;

    hueLine.loadImage("../data/hsv_line.jpeg");
    hueLine.setPosition(leftX+(int)(width-hueLine.width)/2, leftY+5);

    ledMatrix.set(matrixW, matrixH, matrixCellSize,
                  leftX+10 , leftY+70, matrixSpace, true);

    matrixOffset.x = leftX+( width-(ledMatrix.xRight-ledMatrix.xLeft) ) / 2;
    matrixOffset.y = hueLine.getPosition().y+hueLine.getPosition().height+25;
    // place control matrix in needed pos
    ledMatrix.setPosition(matrixOffset.x, matrixOffset.y);
    
    // place processed matrix inside control matrix
    sequencedMatrix.set(matrixW, matrixH, matrixCellSize-matrixCellSize/3,
                        matrixOffset.x+matrixCellSize/6, matrixOffset.y+matrixCellSize/6, matrixSpace+matrixCellSize/3, false);

    outputMatrix.set(matrixW, matrixH, matrixCellSize-matrixCellSize/3,
                     matrixOffset.x+matrixCellSize/6, matrixOffset.y+matrixCellSize/6, matrixSpace+matrixCellSize/3, false);
    
    effects.setup(leftX, ledMatrix.yRight, width, 20, matrixW, matrixH);

    Sequencer.setup(maxSteps, leftX+(width-seqPixelWidth)/2, effects.yLeft+effects.height+5, seqCellSize, seqCellSpace, matrixW*matrixH, selectColor, actColor, inactColor);

    
    gui = new ofxUISuperCanvas("v", leftX+width-10, hueLine.yPos+hueLine.height,15,255);

    gui->setColorBack(ofColor(25));

//    volume = new ofxUIRangeSlider("vol", 0.f, 255.f, &minVolume, &maxVolume, 15, 255,
//                                  leftX+width-10, hueLine.yPos+hueLine.height);

    volume = gui->addRangeSlider("vol", 0.f, 255.f, &minVolume, &maxVolume, 15,255);
//    gui->addWidgetRight(volume);
    gui->autoSizeToFitWidgets();
    gui->enable();
//    gui->setAutoUpdate(true);
    gui->stateChange();

    matrixSequenceMode = false;

    leftY -= atBottom ? (matrixCellSize+matrixSpace)*matrixH : 0;
    
    region = ofRectangle(leftX, leftY, width, height);
    
    sequencedMatrix.setClickedAll();
    sequencedMatrix.setMidiActive(false);
    outputMatrix.setClickedAll();
    outputMatrix.setMidiActive(false);

    ofAddListener(ofEvents().mousePressed, this, &Generator::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &Generator::mouseDragged);

}

void Generator::setupMidi(unsigned int inPort, unsigned int outPort, unsigned int seqActivCC, unsigned int seqBeginCC, unsigned int ledMatrixActivCC, unsigned int hue, unsigned int midiChannel ) {
    midiInPort = inPort;
    midiOutPort = outPort;
    midiInChannel = midiChannel;

    midiSeqBeginCC = seqBeginCC;
    midiHue = hue, midiSeqActivationStartCC = seqActivCC, midiLedMatrixActivationCC = ledMatrixActivCC;
//    midiIn.closePort();
//    midiOut.closePort();
    midiIn.openPort(midiInPort); // opens a connection with the device at port 0 (default)
    midiIn.ignoreTypes(false, false, false);
    midiIn.setVerbose(false);
    midiOut.openPort(midiOutPort);
    midiIn.addListener(this);
    
    hueLine.setupMidi(midiHue, 1, midiInPort, 30+id, 70+id);
    ledMatrix.setupMidi(midiSeqBeginCC, midiInChannel, midiInPort, midiOutPort);
    if (midiLedMatrixActivationCC) ledMatrix.setMidiActivationCC(midiLedMatrixActivationCC);

    Sequencer.setupMidi(midiSeqBeginCC, midiInChannel, midiInPort, midiOutPort);
    if (midiSeqActivationStartCC) Sequencer.setMidiActivationCC(midiSeqActivationStartCC);
}

void Generator::draw(unsigned int quarterBeatCounter) {
    ofEnableAlphaBlending();
    glPushMatrix();
//    if (atBottom) {
//        glTranslatef(0, (matrixCellSize+matrixSpace)*matrixH, 0);
//    }
    id%2 == 0 ?
        gui->setPosition(leftX+width-gui->getRect()->width+15, hueLine.yPos+hueLine.height) :
        gui->setPosition(leftX-15, hueLine.yPos+hueLine.height);
    
    ofSetColor(255, 255, 255);
    hueLine.draw();

    ledMatrix.updateColor(hueLine.color);
    ledMatrix.print();

    effects.updateColor(hueLine.color);
//    effects.parseBitmap(ledMatrix.getBitmapChar());
    if (ledMatrix.changedBitmap) effects.parseBitmap(ledMatrix.getBitmapChar()), ledMatrix.changedBitmap = false, effects.retrigger();
    ofSetColor(255, 255, 255);
    bool act;
    effects.draw();
//    effects.print();
    
    ofColor * seqBitmap;
    
    Sequencer.setStep(quarterBeatCounter, effects.getBitmap());
//    Sequencer.getSequencedBitmap();

    if (Sequencer.ADSRoffset == 0) {
        effects.process(); // ADSRoffset == 0 means sequencer triggered new selected step;
        Sequencer.setStep(quarterBeatCounter, effects.getBitmap());
        sequencedMatrix.parseBitmap(effects.getBitmap(), true);
        sequencedMatrix.trigADSR();
        outputMatrix.parseBitmap(effects.getBitmap(), true);
        outputMatrix.trigADSR();
    }
    
    sequencedMatrix.setADSR(Sequencer.getADSR(), minVolume, 255);
    sequencedMatrix.updateColor(hueLine.color, true);
    sequencedMatrix.update();
    sequencedMatrix.setClickedAll();
    sequencedMatrix.print();
    
    outputMatrix.setADSR(Sequencer.getADSR(), minVolume, maxVolume);
    outputMatrix.updateColor(hueLine.color, true);
    outputMatrix.update();
    outputMatrix.setClickedAll();
    
    Sequencer.draw();
    ofSetColor(255, 255, 255);
//    seqBitmap = Sequencer.getSequencedBitmap();
    
    
    gui->draw();

    glPopMatrix();
	
//	ofDisableAlphaBlending();
//    ofNoFill();
//    ofSetColor(255, 0 ,0 );
//    ofRect(region);
//    ofFill();
}

void Generator::mouseDragged(ofMouseEventArgs & args){
    if (!region.inside(args.x, args.y)) return;
    
    Sequencer.isClicked(args.x, args.y, true);
    if (hueLine.isClicked(args.x, args.y, true)) {
        ledMatrix.updateColor(hueLine.color);
    }
    if (ledMatrix.isClicked(args.x, args.y))
    {
        //printf("MATRIX CLICKED\n");
        ledMatrix.setClicked(args.x, args.y, hueLine.color, true);
        ledMatrix.updateColor(hueLine.color);
        effects.parseBitmap(ledMatrix.getBitmapChar());
    }
}

void Generator::mousePressed(ofMouseEventArgs & args){
    int x = args.x, y = args.y;
    
    if (ledMatrix.isClicked(x,y))
    {
        //printf("MATRIX CLICKED\n");
        ledMatrix.setClicked(x,y, hueLine.color);
        ledMatrix.updateColor(hueLine.color);
        effects.parseBitmap(ledMatrix.getBitmapChar());
    }
    // -- Color --
    if (hueLine.isClicked(x, y))
    {
        //colorSelect = HsvToRgb(hueLine.getClickedX(x,y), saturSelect, valueSelect);
        ledMatrix.updateColor(hueLine.color);
        effects.parseBitmap(ledMatrix.getBitmapChar());
        effects.color = hueLine.color;
    }
    // -- SEQ --
    if (Sequencer.isClicked(x, y))
    {
        Sequencer.setClicked(x,y, selectColor);
    }
    if (effects.isClicked(x, y)) { 
        if (!effects.isOn)
            effects.parseBitmap(ledMatrix.getBitmapChar());
    }
}

void Generator::newMidiMessage(ofxMidiMessage &args) {
    midiValue 		= args.value;
	midiId 			= args.control;
    if (midiLedMatrixActivationCC && midiId == midiLedMatrixActivationCC && midiValue == 127)
        matrixSequenceMode = matrixSequenceMode ? false : true;

    if (midiId == MIDI_VOLUME_START_CC+id) {
        volume->setValueHigh(ofMap(midiValue, 0,128,0,255));
    }

    if (midiId == MIDI_SEQ_START_CC && midiValue == 127) effects.parseBitmap(ledMatrix.getBitmapChar()), effects.retrigger();

    if (!active) return;
//    printf("active: %i, matrMode: %i",midiSeqActivationStartCC, matrixSequenceMode? 1 : 0);
//    for (int i=0; i<3; i++) Sequencer.newMidiMessage(args);
    if (matrixSequenceMode) {
            printf("SEQ START: %i\n",midiLedMatrixActivationCC);
        ledMatrix.setMidiActive(true); Sequencer.setMidiActive(false);
    } else {
        ledMatrix.setMidiActive(false); Sequencer.setMidiActive(true);
    }

//    midiOut.sendControlChange(1, midiLedMatrixActivationCC, 127); 
}

