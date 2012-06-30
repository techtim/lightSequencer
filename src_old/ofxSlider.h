/*
 *  ofxSlider.h
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ofxSldr
#define _ofxSldr

#include "ofMain.h"
#include "ofxMidi.h"
#include <math.h>

class ofxSlider {
public:
	ofxSlider();
	ofxSlider(int x, int y, int wid, int hei);

	void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, bool typ, int minimum = 0, int maximum = 255, bool is_exp = false);
	void setupMidi(unsigned int ident, unsigned int channel=1, unsigned int port = 0, bool fixValue = true);
	void print();
	void draw();

	void setActictiveText(string const & text);
	bool isClicked(int x, int y);
	
    void setValue(int x, int y);
	void setValue(unsigned int val);
	void setValueReal(unsigned int val);
    
	int getValue(){
        return value;
    }
	int getValueMapped(int minimum, int maximum);
	
	bool getState()
	{
		return isOn;
	}
	bool setState(bool turn)
	{
		isOn = turn ? true : false;
	}
	

	ofTrueTypeFont myFont;
	bool type; // 0 - vertical, 1 - horizontal;
    bool isOn;
	bool is_exponent;
	unsigned int radius;
	unsigned int textOffset;
	int leftX, leftY, rightX, rightY;
	int width, height;
	int min, max;
	unsigned int value;

	string actictiveText;
	
	//---MIDI
	void receiveMidi(ofxMidiEventArgs &args);
    ofxMidiIn midiIn;

	unsigned int midiChannel; // channel for exact sequence , setuped 
	unsigned int midiId;
	unsigned int midiValue;
    bool midiFix;
};

#endif
