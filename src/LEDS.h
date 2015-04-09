#ifndef _LEDS
#define _LEDS

#include "ofMain.h"
#include "LED.h"
#include "ofxMidi.h"

class LEDS: public ofxMidiListener {

	public:
		LEDS();
		~LEDS();
		void set(int col, int row, int cell, int x, int y, int spac, bool setAllWhite=false);
        void clear();
    
		bool isClicked(int x, int y);
		LED * getClicked(int x, int y);
		void setClicked(int x, int y, ofColor newColor, bool isDragged=false);
		void setClickedAll();
        int numClicked(int x, int y, ofColor newColor);
    
        void setADSR(ofVec3f adsr, unsigned int _min = 0, unsigned int _max = 255);
        void setADSR(unsigned int _att, unsigned int _dec, unsigned int _rel, unsigned int _min=0, unsigned int _max=255);
        void trigADSR();
        void update();
    
        void parseBitmap(unsigned char *pixels, bool onlyState = false);
		void parseBitmap(ofColor * pixels, bool onlyState = false);
		void parseBitmap();
    
        LED * getByRowCol(int r, int c);
    
		ofColor * getBitmap();
		unsigned char * getBitmapChar();
		ofColor * bitmap;
		unsigned char * bitmapChar;

        void setSaturation(float sat);
        void setBrightness(float bright);

        void getPixelsState (string & pixelsState);
        void setPixelsState (const string pixelsState);
    
        string getChainState();
        void setChainState (const string & chainState);
        void getChainBitmapChar(unsigned char * chainMap);

        void print(bool with_alpha = true);
        void setPosition(int x, int y);
        void updateColor(ofColor newColor, bool updAll = false);
		void updateColor(ofColor newColor, ofColor inactColor);
    
        int getWidth() { return xRight - xLeft; }
        int getHeight() { return yRight - yLeft; }
        ofRectangle getRegion() {return ofRectangle(xLeft, yLeft, xRight - xLeft, yRight - yLeft);}
    
        void setupDmx();
        void getDmx(ofxDmx &dmx);
        void saveDmxConfig();
        void loadDmxConfig();
    
        bool inAddrMode, bChangedAddrMode;
        void setAddrMode(bool is_on) {
            inAddrMode = is_on;
            bChangedAddrMode = true;
        }

        unsigned int ledsInChain;
    
        void setupTexture(int w, int h);
        ofTexture * getTexture(int shape = 0);
        ofTexture * tex;
        unsigned char * texPixels;
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
        unsigned int ledsNumber;
    
        bool bGuiActive;
    
        ofRectangle region;

        ofColor color;
        ofImage matrixImg;
        unsigned char * matrixPixels;

        //---MIDI
        void setupMidi(unsigned int ident, unsigned int channel,  unsigned int inPort, unsigned int outPort);
//        void receiveMidi(ofxMidiEventArgs &args);
        void newMidiMessage(ofxMidiMessage& eventArgs);

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
