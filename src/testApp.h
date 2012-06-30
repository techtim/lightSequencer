#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofColor.h"
#include "colorPicker.h"
#include "LEDS.h"
#include "ledSequencer.h"
#include "ledPresets.h"
#include "ofSoundStream.h"
#include "ofxButton.h"
#include "bitmapMixer.h"
#include "ofSerial.h"
#include "ofxSlider.h"
#include "ofxMidi.h"

#include "Generator.h"
#include "tcpControl.h"

//#include "fft.h"

#include "LiveAudioAnalysis.h"
#include "ofxPortaudioSoundStream.h"
#include "bpmControl.h"

#define BUFFER_SIZE 512
#define NUM_WINDOWS 80

#define PROJECTROOT "/Users/tim/Programming/openFrameworks/oF62/apps/LED/sequenceLed"


class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed  (int key);
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
	
	// -- Main --
	unsigned int frameRate;
	unsigned int frameCount;
	int controllersNum;
    int controllersWidth, controllersHeight;
	int winWidth, winHigh;

    Generator * Gen;
    tcpControl TCP;

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
	// --Time Sound---
	
	ofxButton setupAudioButton;
    ofxButton Play;
    ofxButton Stop;
    ofxButton Start;
    ofxButton ArdButton;
    ofxButton TcpButton;
    
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
	ledSequencer * Sequence;
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
	struct Region{
		int leftX;
		int leftY;
		int rightX;
		int rightY;
	} * region;
	
	Region mixerRegion;
	//		--- LED ---
	ofSerial Arduino;
	bool enableArduino;
    string serialName;
	
	//		--- MIDI ---
	void newMessage(ofxMidiEventArgs &args);
	void newBeat(ofxMidiEventArgs &args);
	
	ofxMidiIn midiBeat;
	ofxMidiIn midiIn;
	ofxMidiOut midiOut;

	unsigned int midiInPort, midiOutPort;
	int midiId;
	int midiValue;
	double midiTimestamp;
	char msg[255];
	
	ofxSlider * Volume;
	ofxSlider * colorSaturation;
    ofxSlider mixerSaturation;
    unsigned int midiSeqActivationStartCC;
    unsigned int midiSeqBeginCC;
    unsigned int midiHueControlCC;
    unsigned int midiLedMatrixActivationCC;

	//		---BPM
	int beatStartTime;
	int beatTmpTime;
	int beatCounter;
	//BeatDetect beatDetector;

};



#endif
