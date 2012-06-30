#ifndef _LEDS
#define _LEDS

#include "ofMain.h"
#include "LED.h"
#include "ofxMidi.h"

class LEDS {

	public:
		LEDS();
		~LEDS();
		void set(int col, int row, int cell, int x, int y, int spac, bool setAllWhite=false);
        
		bool isClicked(int x, int y);
		LED * getClicked(int x, int y);
		void setClicked(int x, int y, ofColor newColor, bool isDragged=false);
		void setClickedAll();
        int numClicked(int x, int y, ofColor newColor);
    
        void parseBitmap(unsigned char *pixels);
		void parseBitmap(ofColor * pixels);
		void parseBitmap();
    
		ofColor * getBitmap();
		unsigned char * getBitmapChar();
		ofColor * bitmap;
		unsigned char * bitmapChar;
	
        void print();
        void setPosition();
        void updateColor(ofColor newColor);
		void updateColor(ofColor newColor, ofColor inactColor);
	
        LED *leds; // array of LED class
        int ledLastClicked;
        
        bool changedBitmap;
		int columns;
        int rows;
        int cellSize;

        int xLeft;
        int yLeft;
        int xRight;
        int yRight;
        int mSpace;

        ofColor color;
        ofImage matrixImg;
        unsigned char * matrixPixels;

        //---MIDI
        void setupMidi(unsigned int ident, unsigned int channel,  unsigned int inPort, unsigned int outPort);
        void receiveMidi(ofxMidiEventArgs &args);
        ofxMidiIn midiIn;
        ofxMidiOut midiOut;
        unsigned int midiInPort, midiOutPort;
        unsigned int midiChannel, midiId, midiValue; // channel for exact sequence , setuped 

        unsigned int midiActivationCC;
        unsigned int midiSeqStartCC;
        bool midiActive;

        void setMidiActive(bool is) {
            midiActive = is;
            is? midiOut.sendControlChange(midiChannel, midiActivationCC, 127):
                midiOut.sendControlChange(midiChannel, midiActivationCC, 0);
        }

        void setMidiActivationCC(unsigned int cc) {
            midiActivationCC = cc;
        }
};

#endif
