#ifndef _colorPic
#define _colorPic

#include "ofMain.h"
#include "ofImage.h"
#include "ofxMidi.h"
#include "ofxUI.h"

// when we exit, we shut down ofxImage
void ofCloseFreeImage();

//----------------------------------------------------
class colorPicker {
	
	public :
    colorPicker();
    
    void loadImage(string path);
    
    void setPosition(int x, int y);
    bool isClicked(int x, int y, bool isDragged = false);
    int	 getClickedX(int x, int y);
    int	 getClickedY(int x, int y);
    int  getColorPos();
    
    int getHeight() { return height; };
    ofRectangle getPosition() { return ofRectangle(xPos, yPos, width, height); };
    
    ofColor HsvToRgb (float hue, float satur, float value );
    
    void draw();
    
    int x, y;
    int width, height;
    float xPos, yPos;
    unsigned int colorPos, triWidth;
    unsigned int psyPos, evenCounter;
    int colorPosStart, colorPosEnd;
    ofColor color;
    ofImage img;
//    ofxUIButton* psyButton;
    ofxUISuperCanvas* gui;
    ofxUILabelToggle* psyButton;
    ofxUIMinimalSlider* hueControl;
    
    float hueSpeed;
    //---MIDI
    void setupMidi(unsigned int ident, unsigned int channel=1, unsigned int port = 0, unsigned int psy_bottom = 32, unsigned int psy_speed = 71);
    void receiveMidi(ofxMidiEventArgs &args);
    ofxMidiIn midiIn;
    
    int midiChannel; // channel for exact sequence , setuped
    int midiId;
    int midiValue;
};

#endif