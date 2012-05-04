#ifndef _colorPic
#define _colorPic

#include "ofMain.h"
#include "ofImage.h"
#include "ofxMidi.h"
#include "ofxButton.h"
#include "ofxSlider.h"

// when we exit, we shut down ofxImage
void ofCloseFreeImage();

//----------------------------------------------------
class colorPicker {
	
	public :
		colorPicker();

		void loadImage(string path);

		void setPosition(int x, int y);
		bool isClicked(int x, int y);
		int	 getClickedX(int x, int y);
		int	 getClickedY(int x, int y);

		ofColor HsvToRgb (float hue, float satur, float value );

		void draw();
		
		int x, y;
		int width, height;
		float xPos, yPos;
		unsigned int colorPos, triWidth;
        unsigned int psyPos, evenCounter;
		ofColor color;
		ofImage img;
        ofxButton psyButton;
        ofxSlider hueControl;

        //---MIDI
        void setupMidi(unsigned int ident, unsigned int channel=1, unsigned int port = 0);
        void receiveMidi(ofxMidiEventArgs &args);
        ofxMidiIn midiIn;
        
        int midiChannel; // channel for exact sequence , setuped 
        int midiId;
        int midiValue;
};

#endif