#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

//#include "MainMenuController.h"

#include "ofColor.h"
#include "colorPicker.h"
#include "LEDS.h"
#include "Sequencer.h"
#include "ledPresets.h"
#include "ofSoundStream.h"
#include "ofxButton.h"
#include "bitmapMixer.h"
#include "ofSerial.h"
#include "ofxMidi.h"
#include "ofTexture.h"
#include "ofxUi.h"
#include "ofxOsc.h"
#include "ofxDmx.h"

#include "Generator.h"
#include "tcpControl.h"

//#include "fft.h"

#include "LiveAudioAnalysis.h"
#include "ofxPortaudioSoundStream.h"
#include "bpmControl.h"

#define BUFFER_SIZE 512
#define NUM_WINDOWS 80

// GUI and CONF

#define PRESETS_NUM 32

//#define SERIAL_NAME "/dev/tty.usbmodemfd121"; //   "/dev/tty.usbmodem411";
#define SERIAL_NAME "/dev/tty.usbserial-A600clK6";

#define OSCHOST "192.168.0.177"
#define OSCPORT 10000

//#define PROJECTROOT "/Users/tim/Programming/openFrameworks/oF62/apps/LED/sequenceLed"

class Projector: public ofBaseApp{
public:
	~Projector(){
		cout << "Projector destroyed" << endl;
	}
	void setup(int matrixW, int matrixH, int _id){
        int winWidth = PROJECTOR_W;
        int winHeight = PROJECTOR_H;
        win_id = _id;
        int ctrlLedSize = (winWidth/matrixW) > (winHeight/matrixH) ?
            winHeight/matrixH : winWidth/matrixW;
		
//        ledControl.set(matrixW, matrixH, ctrlLedSize,
//                       0 + (winWidth-ctrlLedSize*matrixW)/2, 0 + (winHeight-ctrlLedSize*matrixH)/2, 0);
        //    ledControl.set(matrixW, matrixH, ctrlLedSize,
        //                   mixerRegion.leftX + mixerWidth/2-(ctrlLedSize+matrixSpace)*matrixW/2, winHigh/2, matrixSpace);
        
        //    ledControl.set(matrixW, matrixH, 200, mixerRegion.leftX + 80, winHigh/2-50, matrixSpace);
        ledControl.setClickedAll();
//        ledControl.print();
        		cout << "SETUP:" << ctrlLedSize << endl;
	}
    
    void keyReleased(int key) {
        //cout << (0x0400) << endl;
        //cout << (101 | OF_KEY_MODIFIER) << " " << key << endl;
        if(key=='f') {
//            if (win_id == win->id) win->windowResized(PROJECTOR_W, PROJECTOR_H);
//            win->toggleFullscreen();
        }
    }

    void setBitmap(ofColor * bitmap) {
       ledControl.parseBitmap(bitmap);
    }
	void draw(){
//		cout << "CL" << bitmapColor[0].a << endl;
//        glClearColor(0.0, 0.0, 0.0, 0.0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ledControl.print();
	}
	
    LEDS ledControl;
    int win_id;
};

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed  (int key);
   	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void windowResized(int w, int h);
	//void mouseReleased(int x, int y, int button);
	
	void setBPM(float targetBPM);
//	void audioRequested(float *output, int bufferSize, int numChannels);
//	void audioReceived(float * input, int bufferSize, int nChannels);
    
    void showDeviceListWindow(ofxPortaudioSoundStream& paSoundStream);
    void audioInputListener (ofxAudioEventArgs &args);
//  void audioOutputListener (ofxAudioEventArgs &args);
    void setInputDevice(int i);
//	void setOutputDevice(int i);
	
    void toggleAddressMode();
    
//    MainMenuController* mainMenuController;
    
	// -- Main --
	unsigned int frameRate;
	unsigned int frameCount;
	int controllersNum;
    int controllersWidth, controllersHeight;
	int winWidth, winHigh;

    Generator * Gen;
    tcpControl TCP;
    ofxOscSender oscSender;

	// -- Color --
	int hueSelect;
	int saturSelect;
	int valueSelect;
	
	ofColor * colorSelect;
	ofColor * hueColor;
	colorPicker * hueLine;
	
	// -- Matrix --
	int matrixH, matrixW;
	int matrixPosX, matrixPosY;
	int matrixCellSize;
	int matrixSpace;
	
	LEDS * ledMatrix;
	LEDS * sequencedMatrix;
    
//    Projector projector;
    int projector_id;

    int display_id;
    
	// --Time Sound---
    ofxPortaudioSoundStream inputSoundStream;
//    ofxPortaudioSoundStream outputSoundStream;    
	
	vector <float> leftAudioIn;
	vector <float> rightAudioIn;
    
    float 	volume;
    int screenToDraw;
	
//    LiveAudioAnalysis liveAudioAnalysis;
    bpmControl bpmCtrl;
    
    int clickIndex;
	
    float width, height;

	int bufferCounter;
	int bufferSize;

	int inputDeviceID;
	int outputDeviceID;
	bool setupFinished;
	
	ofTrueTypeFont verdana30;
	ofTrueTypeFont verdana14;
	string BPMstring;
	
	string inputDeviceName, outputDeviceName;
//	void printInputSampleRate();

	int mouseX, mouseY;
	bool showingInputDevices, showingOutputDevices;
	int deviceListHeightDistance, deviceListHeightOffset;
	
//	void checkPressOnDeviceList(int x, int y);

	//---SEQ---
	string stringSetup;
	Sequencer * Sequence;
	ofColor selectColor, actColor, inactColor; //params for sequencer
	
	unsigned int quarterBeatCounter;
	unsigned int maxSteps;
	unsigned int limitSteps;
	unsigned int seqCellSize, seqCellSpace;
	//unsigned int BPMS;
	//unsigned int MSEC;
	int pos;
	float BPM;
	int SAMPLE_RATE;
	float lengthOfOneBeatInSamples;
	float lengthOfQuarterBeatInSamples;
	bool soundInited;
	
	bitmapMixer * Mixer;
	ofColor * mixedBitmap;
	LEDS ledControl;
	
    ledPresets Presets;
    unsigned int presetsNum;
    bool presetsShift;

	struct Region{
		int leftX;
		int leftY;
		int rightX;
		int rightY;
	} * region;
	
	Region mixerRegion;
	//		--- LED ---
//	ofSerial Arduino;
//	bool enableArduino;
//    string serialName;
	
	//		--- MIDI ---
    void setupMidi();
	void newMessage(ofxMidiEventArgs &args);
	void newBeat(ofxMidiEventArgs &args);
	
	ofxMidiIn midiBeat;
	ofxMidiIn midiIn;
	ofxMidiOut midiOut;
    ofxMidiOut midiTapOut;
    
	unsigned int midiInPort, midiOutPort, midiTapPort;
	int midiId;
	int midiValue;
	double midiTimestamp;
	char msg[255];
    bool bMidiPortClosed;
    
    bool abletonCtrl;

    unsigned int midiSeqActivationStartCC;
    unsigned int midiSeqBeginCC;
    unsigned int midiPresetBeginCC;
    unsigned int midiHueControlCC;
    unsigned int midiLedMatrixActivationCC;
    
    // --- GUI ---

    void guiEvent(ofxUIEventArgs &e);
    
    ofxUISuperCanvas* gui;
    ofxUIMinimalSlider* mixerSaturation;
    ofxUIMinimalSlider* mixerBrightness;
    ofxUILabelToggle* loadColorButton;
    float mixSaturation;
    float mixBrightness;
    
    ofxUIMultiImageToggle* Play;
    ofxUIMultiImageButton* Start;

    ofxUILabelToggle* OscButton;
    ofxUILabelToggle* LedAddrMode;
    ofxUILabelToggle* MidiSelect;

	//	--- BPM ---
	int beatStartTime;
	int beatTmpTime;
	int beatCounter;
    
    bool bPlaying;
	//BeatDetect beatDetector;
    
    //      ---DMX
    ofxDmx dmx;

    
    //      ---OSC
    ofxOscSender oscSenser;
    ofxOscReceiver oscReceiver;
};



#endif
