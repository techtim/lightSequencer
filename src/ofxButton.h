/*
 *  ofxButton.h
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ofxBut
#define _ofxBut

#include "ofMain.h"
#include "ofxMidi.h"

class ofxButton {
public:
	ofxButton();
	ofxButton(int x, int y, int wid, int hei);

	void setup(int x, int y, int wid, int hei, bool isToggle);
	void print();
	void draw();

	void setActictiveText(string text);
	void setInactictiveText(string text);
	void setFontSize(unsigned int size);
	bool isClicked(int x, int y);
	bool getState();

	ofTrueTypeFont myFont;

	bool isOn;
	bool type; // 0 - clicker, 1 - switcher;
	unsigned int fontSize;
	int leftX, leftY, rightX, rightY;
	int width, height;
	string actictiveText;
	string inactictiveText;
    
    //---MIDI
    void setupMidi(unsigned int ident, unsigned int channel, unsigned int inPort, unsigned int outPort);
    void receiveMidi(ofxMidiEventArgs &args);
    void setMidiActive(bool is) { midiActive = is; }
    ofxMidiIn midiIn;
    ofxMidiOut midiOut;
    unsigned int midiInPort, midiOutPort;
    unsigned int midiChannel, midiId, midiValue; // channel for exact sequence , setuped 
    bool midiActive;

};

#endif