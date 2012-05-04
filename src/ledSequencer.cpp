/*
 *  Sequencer.cpp
 *  sequenceLed
 *
 *  Created by tim tavli on 05/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "ledSequencer.h"

ledSequencer::ledSequencer()
{
	;;
}

ledSequencer::~ledSequencer()
{
	delete [] leds;
	delete [] bitmap;
	delete [] sequencedBitmap;
	delete [] sequencedBitmapChar;
	delete [] sliders;
}

void ledSequencer::Create(
	unsigned int stepsCnt, unsigned int x, unsigned int y, 
	unsigned int cell, unsigned int spac, unsigned int seqBitLen,
	ofColor selColor, ofColor actColor, ofColor inactColor)
{
//	set(int col, int row, int cell, int x, int y, int spac);
	steps = stepsCnt;
	currentStep = 1000; // big value to work on step == 0 
	LEDS::set(steps/2, 2, cell, x+cell+spac, y, spac);
	seqBitmapLen = seqBitLen;
	sequencedBitmap = new ofColor [seqBitmapLen];
	sequencedBitmapChar = new unsigned char [seqBitmapLen*3];
	color = selColor;
	inactiveColor = inactColor;
	activeColor = actColor;
	
	ADSRoffset = 0;
	ADSRvalue = 0;
    
	//---Controll---
	int slidersLen = 255;
	int slidersOffset = y+2*(cellSize+mSpace);
	sliders = new ofxSlider [3];
    
    sliders[0].setup(x+(cell+spac)*2, slidersOffset, slidersLen, 15, true, 0, 255, true);
	
	sliders[0].setActictiveText("ATT");
	sliders[1].setup(x+(cell+spac)*2, slidersOffset+20, slidersLen, 15, true, 0, 255, true);
	sliders[1].setActictiveText("DEC");
	sliders[2].setup(x+(cell+spac)*2, slidersOffset+40, slidersLen, 15, true, 0, 255, true);
	sliders[2].setActictiveText("REL");
	setADSR(0.0f,0.0f,0.0f);
	
	//--Speed
	button8.setup(x, y, cell, cell, true);
	button8.setActictiveText("1/8");
	button16.setup(x, y+cell+spac, cell, cell, true);
	button16.setActictiveText("1/16");
	button16.isOn = true;
	setQuant16();
}

void ledSequencer::setupMidi(unsigned int midiSeqStart, unsigned int channel, unsigned int inPort, unsigned int outPort) {
    //---MIDI---
	midiChannel = channel;
	midiInPort = inPort;
    midiOutPort = outPort;
    
    midiIn.openPort(midiInPort); // opens a connection with the device at port 0 (default)
    midiOut.openPort(midiOutPort);
    ofAddListener(midiIn.newMessageEvent, this, &ledSequencer::newMessage);
    
    midiActive = false;
    midiSeqStartCC = midiSeqStart;

    sliders[0].setupMidi(100, midiChannel, 100); // set wrong port to controll from active sequencer not global midi
    sliders[1].setupMidi(101, midiChannel, 100);
	sliders[2].setupMidi(102, midiChannel, 100);

    button8.setupMidi(30, 1, midiInPort, midiOutPort);
    button16.setupMidi(31, 1, midiInPort, midiOutPort);
}

bool ledSequencer::isClicked(int x, int y, bool dragged)
{
	bool isclicked = false;
	for (int i = 0; i < 3; i++)
		if (isclicked == sliders[i].isClicked(x, y))
				setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
	if (dragged) return isclicked;

	if (button8.isClicked(x, y)) { setQuant8(); }
	if (button16.isClicked(x, y)){ setQuant16(); }

	return LEDS::isClicked(x, y);
}

bool ledSequencer::isSelectedStep(int step)
{
	if (!leds[step-1].isSelected)
		(step == 0 ? leds[steps-1].color = inactiveColor : leds[step-1].color = inactiveColor);

	leds[step].color = color;
	//leds[step].isSelected = true;
	//printf("leds[%i].color=%f", step, leds[step].color.r);
	if (leds[step].isSelected && currentStep != step) 
	{
		setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
		currentStep = step;
		return true;
	}
	currentStep = step;
	return false;
}

void ledSequencer::setStep(int step, ofColor * feedBitmap)
{
	const int stp = (step/quant)%steps;
	if (isSelectedStep(stp))
	{
		ADSRoffset = 0;
//		ADSRvalue = 0;
	}
	memcpy(sequencedBitmap, feedBitmap, seqBitmapLen*sizeof(ofColor));
	setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
}

ofColor * ledSequencer::getSequencedBitmap()
{
	if (a == 0 && d == 0 && r ==0 ) {
        ADSRvalue = 0;
    } else if (ADSRoffset<=a){ // attack
		ADSRvalue += 255.0/a;
        if (ADSRvalue > 255) ADSRvalue = 255.0;
	} else if (ADSRoffset>a && ADSRoffset<=a+d) { // decay
 		ADSRvalue = 255;
		ADSRoffset++;
	} else if (ADSRoffset<=r+a+d){ // release
		(ADSRvalue == 0.0 ? ADSRvalue = 255.0 : (unsigned int)(ADSRvalue -= 255.0/r));
		if (ADSRvalue < 0) ADSRvalue = 0;
	} else {
		ADSRvalue = 0;
	}
	ADSRoffset++;
//	if (midiChannel ==1) printf("\nOFFSET=%i, ADSRVAL=%f \n", ADSRoffset, ADSRvalue);
	
	for (int i=0; i<seqBitmapLen; i++)
	{
        if (sequencedBitmap[i].getHex() != 0) {
			sequencedBitmap[i].setBrightness(ADSRvalue);
		}
	}
	return sequencedBitmap;
}

unsigned char * ledSequencer::getSequencedBitmapChar()
{
    sequencedBitmap = getSequencedBitmap();

	int count=0;
	for (int i=0; i<seqBitmapLen; i++)
    {
        sequencedBitmapChar[count++] = sequencedBitmap[i].getR();
        sequencedBitmapChar[count++] = sequencedBitmap[i].getG();
        sequencedBitmapChar[count++] = sequencedBitmap[i].getB();
    }
	
	return sequencedBitmapChar;
}

void ledSequencer::print()
{
    int x = xLeft;
    int y = yLeft;
    int counter = 0;
    if (midiActive) {
        ofSetColor(150, 150, 150);
        ofRect(x-mSpace, y-mSpace, (cellSize + mSpace) * columns+mSpace, (cellSize + mSpace) * rows+mSpace);
        
        for (int i=0; i < rows * columns; i++) {
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, leds[i].isSelected ? 127 : 0);
        }
        midiOut.sendControlChange(midiChannel, midiActivationCC, 127);
        
        midiOut.sendControlChange(midiChannel, midiSeqStartCC+currentStep, 127);
        currentStep == 0 ?
            midiOut.sendControlChange(midiChannel, midiSeqStartCC+steps-1, leds[steps-1].isSelected ? 127 : 0) :
            midiOut.sendControlChange(midiChannel, midiSeqStartCC+currentStep-1, leds[currentStep-1].isSelected ? 127 : 0) ;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
//            if (leds[counter].isSelected == true)
//            {
				ofSetColor(leds[counter].color.r, leds[counter].color.g, leds[counter].color.b);
//			}
//			else
//            {
//                ofSetColor(0,0,0);
//            }
            ofRect(x, y, cellSize, cellSize);
            x += cellSize + mSpace;
            counter++;
        }
        y += cellSize + mSpace;
        x = xLeft;
    }
	updateColor(activeColor, inactiveColor);

    for (int i = 0; i < 3; i++) sliders[i].draw();

    button8.draw();
	button16.draw();
	//ofDrawBitmapString("COUNTER:"+ofToString(counter),30,20);
    //printf("COUNTER:%i \n", counter);
};

//---MIDI---
void ledSequencer::newMessage(ofxMidiEventArgs &args){
	if (midiActivationCC == args.byteOne && args.byteTwo == 127 && args.channel == midiChannel) midiActive = !midiActive;
    
    if (midiActive) {
//        for (int i=0; i<3; i++) sliders[i].receiveMidi(args); // send command right to slider
        button8.setMidiActive(true);
        button16.setMidiActive(true);
        if (button8.midiId == args.byteOne) setQuant8();
        if (button16.midiId == args.byteOne) setQuant16();
    } else {
        midiOut.sendControlChange(midiChannel, midiActivationCC, 0); // turn off activation button
        button8.setMidiActive(false);
        button16.setMidiActive(false);
    }

	if (midiActive && args.channel == midiChannel && 
        args.byteOne >= midiSeqStartCC && 
        args.byteOne<midiSeqStartCC+steps && args.byteTwo == 127
    )
		leds[args.byteOne-midiSeqStartCC].isSelected = (leds[args.byteOne-midiSeqStartCC].isSelected == true ? false : true);
}