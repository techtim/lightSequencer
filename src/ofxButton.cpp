/*
 *  ofxButton.cpp
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxButton.h"

ofxButton::ofxButton()
{
	;
}

ofxButton::ofxButton(int x, int y, int wid, int hei)
{
	isOn = false;
	actictiveText = "";
	leftX = x;
	leftY = y;
	width = wid;
	height = hei;
	rightX = x + width;
	rightY = y + height;
    midiActive = false;
}

void ofxButton::setup(int x, int y, int wid, int hei, bool isToggle)
{
	type = isToggle;
	isOn = false;
	actictiveText = "";
    inactictiveText = "";
	leftX = x;
	leftY = y;
	width = wid;
	height = hei;
	rightX = x + width;
	rightY = y + height;
	fontSize = 12;
	myFont.loadFont("verdana.ttf", fontSize);
//    myFont.setLineHeight(height);
}


void ofxButton::setActictiveText(string text)
{
	actictiveText = text;
   	if (inactictiveText == "") inactictiveText = text;
}

void ofxButton::setInactictiveText(string text)
{
	inactictiveText = text;
}

void ofxButton::setFontSize(unsigned int size)
{
	fontSize = size;
	myFont.loadFont("verdana.ttf", fontSize);
//    myFont.setLineHeight(height);
//	myFont.setLetterSpacing(1.035);
}

bool ofxButton::isClicked(int x, int y)
{
	if (y >= leftY && y <= rightY && x >= leftX && x <= rightX)
	{
		if (type) isOn = !isOn;
        printf("isClicked: %s", actictiveText.c_str());
		return true;
	}
	return false;
}

bool ofxButton::getState()
{
	return isOn;
}

void ofxButton::print()
{
	ofSetColor(60, 60, 60);	
	ofRect(leftX, leftY, width, height);
	ofSetColor(0, 0, 0);
	ofDrawBitmapString( (isOn ? actictiveText : inactictiveText),leftX,leftY);
}

void ofxButton::draw() {
	
	glPushMatrix();
	glTranslatef(leftX, leftY, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(50, 50, 50);
    
    // Send MIDI data
    if (midiActive) midiOut.sendControlChange(midiChannel, midiId, isOn ? 127 : 0); 
    
	// if a toggle
	if (isOn)
	{ 
        ofSetColor(150, 150, 150);
        ofRect(0, 0, width, height);
        ofSetColor(255, 255, 255);
        myFont.drawString(
            actictiveText, int(width - myFont.stringWidth(actictiveText))/2, int(height + myFont.stringHeight(actictiveText))/2
        );
        if (type == false) isOn = false; // blink once on midi
    } else {	
		ofSetColor(75, 75, 75);
        ofRect(0, 0, width, height);
		ofSetColor(255, 255, 255);
        if (inactictiveText != "")
            myFont.drawString(
                inactictiveText, int(width - myFont.stringWidth(inactictiveText))/2, int(height + myFont.stringHeight(inactictiveText))/2
            );
	}

	ofDisableAlphaBlending();
	
	glPopMatrix();
}

void ofxButton::setupMidi(unsigned int ident, unsigned int channel, unsigned int inPort, unsigned int outPort){ 
    midiChannel = channel;
	midiId = ident;
    midiInPort = inPort, midiOutPort = outPort;
    if (midiInPort != 100) { // do not turn on trick
//        midiIn.closePort();
        midiIn.openPort(midiInPort);
        if (midiOutPort != 100) {
//            midiOut.closePort();
            midiOut.openPort(midiOutPort);
        }
        ofAddListener(midiIn.newMessageEvent, this, &ofxButton::receiveMidi);
    }
}

void ofxButton::receiveMidi(ofxMidiEventArgs &args){
    if (midiActive && midiId == args.byteOne && args.channel == midiChannel && args.byteTwo == 127) isOn = !isOn;
}