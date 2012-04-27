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
	actColor.set(0, 50, 200);
	inactColor.set(0, 0, 0);
	maxSteps = 16;
	limitSteps = 128;
	seqCellSize = 40;
	seqCellSpace = 5;
}

Generator::~Generator() {
    ofRemoveListener(ofEvents.mousePressed, this, &Generator::mousePressed);;;
}

void Generator::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, 
                      unsigned int matW, unsigned int matH, bool at_bottom, unsigned int id) {
    // view params
    leftX = x, leftY = y, width = wid, height = hei;
    matrixW = matW, matrixH = matH;
    atBottom = at_bottom;
    matrixSpace = 2;
    //    matrixCellSize = controllersWidth/(matrixW+matrixSpace);
//    matrixCellSize=50;
    matrixCellSize = (width/matrixW > height/4/matrixH ? height/4/matrixH : width/matrixW ) - matrixSpace;
    seqCellSize = 40;
	seqCellSpace = 5;
    float seqPixelWidth = maxSteps/2 * (seqCellSize + seqCellSpace) + seqCellSize; // one more cell for quantisation
    float matrixPixelWidth = (matrixSpace+matrixCellSize)*matrixW-matrixSpace;
    
    leftY += atBottom ? (matrixCellSize+matrixSpace)*matrixH : 0;
    
    hueLine.loadImage("../../data/hsv_line.jpeg");
    hueLine.setPosition(leftX+(int)(width-hueLine.width)/2, leftY+10);

    //    hueLine.xPos+hueLine.height;
  
    //		colorSaturation.setup(leftX+matrixCellSize/2, leftY+10, matrixCellSize/2, 50, false, 0, 255);
    
    ledMatrix.set(matrixW, matrixH, matrixCellSize, 
                     leftX+(width-matrixPixelWidth)/2 , leftY+90, matrixSpace, true);
    
    effects.setup(leftX, ledMatrix.yRight, width, 40, matrixW, matrixH);
    
    Sequencer.Create(maxSteps, leftX+(width-seqPixelWidth)/2, leftY+170, seqCellSize, seqCellSpace, matrixW*matrixH, selectColor, actColor, inactColor, 1, midiPort);
    
    leftY -= atBottom ? (matrixCellSize+matrixSpace)*matrixH : 0;
//    sequencedMatrix.set(matrixW, matrixH, matrixCellSize,
//                           leftX+(width-matrixPixelWidth)/2, leftY+(atBottom ? -matrixH * matrixCellSize : height-matrixCellSize), matrixSpace, true);
    sequencedMatrix.set(matrixW, matrixH, matrixCellSize,
                        leftX+(width-matrixPixelWidth)/2, leftY+(atBottom ? 0 : height-(matrixCellSize+matrixSpace)*matrixH), matrixSpace, true);
    sequencedMatrix.setClickedAll();
    
    ofAddListener(ofEvents.mousePressed, this, &Generator::mousePressed);
    ofAddListener(ofEvents.mouseDragged, this, &Generator::mouseDragged);

}

void Generator::setupMidi(unsigned int midiPort, unsigned int seqActivCC, unsigned int ledMatrixActivCC, unsigned int hue ) {
    midiIn.openPort(midiPort); // opens a connection with the device at port 0 (default)
    midiOut.openPort(midiPort);
    ofAddListener(midiIn.newMessageEvent, this, &Generator::receiveMidi);
    
    hueLine.setupMidi(midiHue, 1, midiPort);
    ledMatrix.setupMidi(seqActivCC, 1, midiPort);
    ledMatrix.setMidiActivationCC(ledMatrixActivCC);
    Sequencer.setMidiActivationCC(seqActivCC);
}

void Generator::draw(unsigned int quarterBeatCounter) {
    ofEnableAlphaBlending();
    glPushMatrix();
//    if (atBottom) {
//        glTranslatef(0, (matrixCellSize+matrixSpace)*matrixH, 0);
//    }
    ofSetColor(255, 255, 255);
    hueLine.draw();
    //		ofSetColor(255, 255, 255);
    //		colorSaturation.draw();
    
    ofSetColor(255, 255, 255);
    ledMatrix.updateColor(hueLine.color);
    ledMatrix.print();
    ofSetColor(255, 255, 255);
    bool act;
    effects.draw();

    ofColor * seqBitmap;

    if (Sequencer.ADSRoffset == 0) effects.process(); // ADSRoffset == 0 means sequencer triggered new selected step;
    Sequencer.setStep(quarterBeatCounter, effects.getBitmap());

    seqBitmap = Sequencer.getSequencedBitmap();
    sequencedMatrix.parseBitmap(seqBitmap);

    Sequencer.print();
    ofSetColor(255, 255, 255);
    sequencedMatrix.print();
    
    glPopMatrix();
	
	ofDisableAlphaBlending();
}

void Generator::mouseDragged(ofMouseEventArgs & args){
    Sequencer.isClicked(args.x, args.y, true);
    if (hueLine.isClicked(args.x, args.y)) {
        ledMatrix.updateColor(hueLine.color);
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

void Generator::receiveMidi(ofxMidiEventArgs &args) {
    if (!active) return;
    

}

