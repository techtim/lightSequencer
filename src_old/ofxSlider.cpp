/*
 *  ofxSlider.cpp
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSlider.h"

ofxSlider::ofxSlider()
{
	value = 0;
	myFont.loadFont("../../data/mono.ttf", 10);
}

void ofxSlider::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, bool typ, int minimum, int maximum, bool is_exp)
{
	type = typ;
	is_exponent = is_exp;
    isOn = false;
	leftX = x;
	leftY = y;
	width = wid;
	height = hei;
	value = 0;
	radius = type ? height/2 : width/2; 
	rightX = x + width;
	rightY = y + height;
	min = minimum;
	max = maximum;
	textOffset = type ? leftX+10 : leftY+10;
    
    actictiveText = "";
    
    midiId = 0;
}

void ofxSlider::setupMidi(unsigned int ident, unsigned int channel, unsigned int port, bool fixValue)
{
	midiChannel = channel;
	midiId = ident;
    midiFix = fixValue;
    if (port != 100) {
        midiIn.openPort(port);
        ofAddListener(midiIn.newMessageEvent, this, &ofxSlider::receiveMidi);
    }
}

void ofxSlider::setActictiveText(string const & text)
{
	actictiveText = text;
	textOffset = (type ? myFont.stringWidth(actictiveText) : myFont.stringHeight(actictiveText))+10;
}

bool ofxSlider::isClicked(int x, int y)
{
	if (y >= leftY && y <= rightY && x >= leftX && x <= rightX)
	{
		printf("ofxSlider::isClicked");
		setValue(x, y);
		isOn = !isOn;
		return true;
	}
	return false;
}


void ofxSlider::setValue(int x, int y)
{
//	printf("ofxSlider ST VALUE=%i\n", value);
    value = type? ofMap(x - leftX, 0, width, min, max, false)
	: ofMap(rightY - y, 0, height, min, max, false); // 1-vert 0-horiz
    if (is_exponent) value = (exp((float)value/max*2)-1)/(exp(2)-1)*max;
//	printf("ofxSlider VALUE=%i\n\n", value);
}

int ofxSlider::getValueMapped(int minimum, int maximum)
{
	int mappedValue = ofMap(value, 0, max, minimum, maximum, false);
	return mappedValue;
}

void ofxSlider::setValue(unsigned int val)
{
	value = ofMap(val, 0, 127, min, max, false);

    if (is_exponent) value = (exp((float)value/max*2)-1)/(exp(2)-1)*max;
}

void ofxSlider::setValueReal(unsigned int val)
{
    value = val;
}

void ofxSlider::receiveMidi(ofxMidiEventArgs &args)
{
	if (midiChannel == args.channel)
		if (midiId == args.byteOne)
		{
//            if (midiValue) printf("SLIDER CHAN: %i ID: %i VALUE: %i \n", args.channel, midiId, midiValue) ;
            int valueMapped = getValueMapped(0,127);
            int midiVal = is_exponent ? (exp(((float)args.byteTwo/127*2))-1)/(exp(2)-1)*127 : args.byteTwo;
			if (!midiFix) {
                setValue(args.byteTwo);
            }
            else if (midiFix && ( valueMapped - midiVal) < 10 && (valueMapped - midiVal) >= -10) {
				setValue(args.byteTwo);
			}
		}
}

void ofxSlider::draw() {

	glPushMatrix();
//	type? glTranslatef(leftX, leftY, 0)
//		: glTranslatef(leftX, leftY, 0);
	glTranslatef(leftX, leftY, 0);	
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(50, 50, 50);

	// if a toggle
	if (isOn)
	{ ofSetColor(150, 150, 150);}
	else
	{
		ofSetColor(100, 100, 100);
	}
	
	ofNoFill();
	ofSetColor(255,255,255);
	ofRect(0, 0, width, height);
	ofFill();
	
	ofSetColor(255,255,255,255);
	type? ofCircle(value, height/2, radius)
	: ofCircle(width/2, height-value, radius);
	glPopMatrix();
	//---Label part---
	glPushMatrix();
	type? glTranslatef(leftX - textOffset, leftY, 0)
		: glTranslatef(leftX, leftY - textOffset, 0);

	ofSetColor(255,255,255);
	if (actictiveText.size() > 0) myFont.drawString(actictiveText, 0, (int)radius*1.5); 
	glPopMatrix();
	
	ofDisableAlphaBlending();

}

