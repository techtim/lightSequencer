#include "testApp.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib> 

//--------------------------------------------------------------
void testApp::setup(){

    setupFinished = false;
    
    frameRate = 60;
    ofSetFrameRate(frameRate);

	ofBackground(40,40,40);
//    ofBackground(255,255,255);
    ofSetCircleResolution(40);
	
	winWidth = ofGetWidth();
	winHigh = ofGetHeight();
	printf("W = %i, H =%i",winWidth,winHigh);
	
    //---MIDI---
	midiIn.listPorts();
    midiOut.listPorts();
    
    midiPort = midiIn.getPortByName("nanoKONTROL2");
	if (midiPort != 100) {
        midiIn.openPort(midiPort); // opens a connection with the device at port 0 (default)
        midiOut.openPort(midiPort);
    }
    printf("OPENED PORT: %i", midiPort);
    if (midiPort != 100) ofAddListener(midiIn.newMessageEvent, this, &testApp::newMessage);

    //---Buttons---
	Play.setup(winWidth/2 - 50, 0, 100, 30, true);
	Play.setActictiveText("PLAY");
    Play.setInactictiveText("STOP");
    Play.setFontSize(14);
    Play.setupMidi(41, 1, midiPort);
    Play.setMidiActive(true);
    
    Start.setup(winWidth/2 - 25, 40, 50, 30, false);
    Start.setActictiveText("start");
    Start.setFontSize(12);
    Start.setupMidi(43, 1, midiPort);
//    Start.setMidiActive(true);
    ArdButton.setup(winWidth/2 - 75, 60, 50, 30, true);
    ArdButton.setActictiveText("Arduino");
    ArdButton.setFontSize(12);
    TcpButton.setup(winWidth/2 + 40, 60, 50, 30, true);
    TcpButton.setActictiveText("TCP");
    TcpButton.setFontSize(12);

	//---BPM
    leftAudioIn.assign(BUFFER_SIZE, 0.0);
	rightAudioIn.assign(BUFFER_SIZE, 0.0);
    
    inputDeviceID = 0;//the mic(0)/line input(1) of a mac book pro
	outputDeviceID = 2;//the headphones out of a macbook pro
	setInputDevice(inputDeviceID);//includes calling the setup routine etc
	ofAddListener(inputSoundStream.audioReceivedEvent, this, &testApp::audioInputListener);

    verdana14.loadFont("../../data/verdana.ttf", 14, true, true);
	verdana14.setLineHeight(14.0f);
	verdana14.setLetterSpacing(1.035);
    
    verdana30.loadFont("../../data/verdana.ttf", 14, true, true);
	verdana30.setLineHeight(14.0f);
	verdana30.setLetterSpacing(1.035);
//    
    
	//---Layout
	controllersNum = 4;
	controllersWidth = 440;
    controllersHeight= winHigh/2;
    mixerRegion.leftX = controllersWidth;
	mixerRegion.leftY = 0;
	mixerRegion.rightX = winWidth-controllersWidth;
	mixerRegion.rightY = winHigh;
	
	region = new Region[controllersNum];
	region[0].leftX= 0;
	region[0].leftY= 0; 
	region[2].leftX = 0;
	region[2].leftY = winHigh-controllersHeight;
	region[1].leftX = mixerRegion.rightX;
	region[1].leftY = 0;
	region[3].leftX = mixerRegion.rightX;
	region[3].leftY = winHigh-controllersHeight;;
	
	region[0].rightX = mixerRegion.leftX;
	region[0].rightY = 0;
	region[2].rightX = mixerRegion.leftX;
	region[2].rightY = winHigh;
	region[1].rightX = controllersWidth;
	region[1].rightY = controllersHeight;
	region[3].rightX = winWidth;
	region[3].rightY = winHigh;
	
    bpmCtrl.setup(
        mixerRegion.leftX, mixerRegion.leftY + 100,
        mixerRegion.rightX - mixerRegion.leftX, mixerRegion.rightY - mixerRegion.leftY -400
    );
    bpmCtrl.setupMidi(0, 1, midiPort);
//########################
    //---LED
	Arduino.setup("/dev/tty.usbmodem411", 19200);
	enableArduino = false;
	Arduino.setVerbose(false);
	Arduino.flush();
    
	//---Matrix---
    matrixW = 8;
    matrixH = 1;
	
    matrixSpace = 2;
//    matrixCellSize = controllersWidth/(matrixW+matrixSpace);
    matrixCellSize=50;
    float matrixPixelWidth = (matrixSpace+matrixCellSize)*matrixW-matrixSpace;
    

//########################
	//---Color---
    saturSelect = 255;
    valueSelect = 255;
	
	colorSelect = new ofColor[controllersNum];
	for (int i=0; i < controllersNum; i++) colorSelect[i].set(255.0f, 255.0f, 255.0f, 255.0f);

    midiHueControlCC = 74;
//	colorSaturation = new ofxSlider[controllersNum];
	
    midiLedMatrixActivationCC = 45;

	//---Time Sound---
    BPM = 120;
    //BPMS = BPM / 60 * 1000;
    //MSEC = 0;
	
    SAMPLE_RATE = 44100;
	
	soundInited = false;
	
    setBPM(BPM);
	beatCounter = 0;
	quarterBeatCounter = 0;
    pos = 0; // running sample count
    
	//--- SEQ ---
	selectColor.set(0,200,50);
	actColor.set(0, 50, 200);
	inactColor.set(0, 0, 0);
	maxSteps = 16;
	limitSteps = 128;
	seqCellSize = 40;
	seqCellSpace = 5;
    
    float seqPixelWidth = maxSteps/2 * (seqCellSize + seqCellSpace) + seqCellSize; // one more cell for quantisation
    
    midiSeqActivationStartCC = 34;
    Sequence = new ledSequencer[controllersNum];
	
	Volume = new ofxSlider[controllersNum];
	Gen = new Generator[controllersNum];
    
    for (int i=0; i<controllersNum; i++)
	{
		Gen[i].setup(region[i].leftX, region[i].leftY, controllersWidth, controllersHeight, matrixW, matrixH, (i>1?true:false), i);
        Gen[i].setupMidi(midiPort, midiSeqActivationStartCC+i, midiLedMatrixActivationCC, midiHueControlCC+i);
//		colorSaturation[i].setup(region[i].leftX+matrixCellSize/2, region[i].leftY+10, matrixCellSize/2, 50, false, 0, 255);		
    }

	Volume[0].setup(mixerRegion.leftX,  50, 20, 255, false, 0, 255);
	Volume[1].setup(mixerRegion.rightX-10, 50, 20, 255, false, 0, 255);
	Volume[2].setup(mixerRegion.leftX,  winHigh-controllersHeight+50, 20, 255, false, 0, 255);
	Volume[3].setup(mixerRegion.rightX-10, winHigh-controllersHeight+50, 20, 255, false, 0, 255);

    Volume[0].setupMidi(120, 1, midiPort, false);
    Volume[1].setupMidi(121, 1, midiPort, false);
    Volume[2].setupMidi(122, 1, midiPort, false);
    Volume[3].setupMidi(123, 1, midiPort, false);
    
    mixerSaturation.setup(mixerRegion.leftX+(mixerRegion.rightX-mixerRegion.leftX)/2-255/2, mixerRegion.rightY/2 - 100, 255, 20, true, 0, 255);
//    mixerSaturation.setActictiveText("SAT");
    mixerSaturation.setupMidi(70, 1, midiPort, false);

	ledControl.set(matrixW, matrixH, (mixerRegion.rightX-mixerRegion.leftX+60)/matrixW, mixerRegion.leftX - matrixSpace*(matrixW-1)/2 - 30, winHigh/2-50, matrixSpace);
//    ledControl.set(matrixW, matrixH, 200, mixerRegion.leftX + 80, winHigh/2-50, matrixSpace);
	ledControl.setClickedAll(); 
	Mixer = new bitmapMixer;
	Mixer->setup(controllersNum, matrixW, matrixH);
    
    presetsNum = 32;
    Presets.setup(mixerRegion.leftX+30, mixerRegion.rightY-winHigh/3, mixerRegion.rightX-mixerRegion.leftX-60, winHigh/3, matrixW, matrixH, presetsNum);
    Sequence[0].setMidiActive(true);

    setupFinished = true;
}

//--------------------------------------------------------------
void testApp::update(){
	frameCount++;
    if (TcpButton.isOn) TCP.update();
	if (frameRate/30 == frameCount) frameCount = 0;
}

//--------------------------------------------------------------
void testApp::draw(){
	
	Play.draw();
    Start.draw();
    TcpButton.draw();
    ArdButton.draw();
	bpmCtrl.draw();
    BPMstring = ofToString(bpmCtrl.BPM, 1)+"\n\n BPM";
    verdana30.drawString(BPMstring, bpmCtrl.leftX + bpmCtrl.width/2 - verdana30.stringWidth(BPMstring)/2, bpmCtrl.leftY+30);
    setBPM(bpmCtrl.BPM);
//    printf(BPMstring.data());
    mixerSaturation.draw();
    Presets.draw();
    Mixer->setSaturation(255-mixerSaturation.getValue());
	for (int i=0; i<controllersNum; i++)
	{
		ofSetColor(0, 200, 55);
		
        Gen[i].draw(quarterBeatCounter);
        
        Volume[i].draw();
        
        ofColor * seqBitmap;
		seqBitmap = Gen[i].getSequencedBitmap();
		Mixer->feedBitmap(seqBitmap, i, Volume[i].getValue());
	}

	ledControl.parseBitmap(Mixer->outputMixed());
	ledControl.print();
	unsigned char * matrixPixels = ledControl.getBitmapChar();
//###
//    if (frameCount == 0) TCP.write((char*)matrixPixels, ledControl.columns*ledControl.rows*3);
//###
	if ((TcpButton.isOn || ArdButton.isOn) && frameCount == 0) // && enableArduino)
	{
//		Arduino.flush(1, 1);
		//printf("\n ******* \n");
		//for (int a = 0; a < serialSyncCount; )
		//			if (Arduino.writeByte(1)) a++;
		
		//unsigned char stat = Arduino.readByte();
		//		printf("STAT: %C \n", stat);
		if (ArdButton.isOn) {
            unsigned char * flag = new unsigned char;
            *flag = 'R';
            Arduino.writeByte(*flag);
            delete flag;
        }
        if (TcpButton.isOn) {
            unsigned char flag = 'R';
            TCP.write(flag);
            TCP.writeRaw(matrixPixels, matrixW * matrixH * 3);
		}

		int counter = 0;
		
		//printf("FOR: %i \n", matrixW * matrixH * 3);
		
		for (int i=0; i < matrixW * matrixH * 3; i++)
		{
			unsigned char pix = matrixPixels[i];// >> 2;
//					if (pix == "0") pix = "00";
			//		sscanf(str,"%x",&b);
//			printf("Write \"%02X\" \n", pix);
//			if (TcpButton.isOn) TCP.write(pix, true);
            if (ArdButton.isOn)printf("Write \"%02X\" \n", pix), Arduino.writeByte(pix);
//			{
//				counter++;
//			}
		}
//        std::cout<< "\n";
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    for (int i=0; i<controllersNum; i++)
	{
		Volume[i].isClicked(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	for (int i=0; i<controllersNum; i++)
	{
		Volume[i].isClicked(x, y);
    }

    if (Presets.isClicked(x,y) && Presets.isLedsClicked) {
        if (Presets.isRec) {
            printf("PRESETS SAVE = %i \n", Presets.ledLastClicked);
            for (int i=0; i<4; i++) Presets.save(Presets.ledLastClicked, i, Gen[i]);
        } else {
            printf("PRESETS LOAD = %i \n", Presets.ledLastClicked);
            for (int i=0; i<4; i++) Presets.load(Presets.ledLastClicked, i, Gen[i]);
        }
    };
    Play.isClicked(x,y);
    bpmCtrl.isClicked(x,y);
    ArdButton.isClicked(x,y);
    if (TcpButton.isClicked(x,y)) {
        if (!TCP.weConnected) TCP.setup();
    };
}

void testApp::exit()
{
	//if (!setupAudioButton.getState() && soundInited) ofSoundStreamStop();
	ofRemoveListener(inputSoundStream.audioReceivedEvent, this, &testApp::audioInputListener);
	Arduino.flush();
   	Arduino.close();
    
	leftAudioIn.clear();
	rightAudioIn.clear();

	delete [] region;
	delete [] colorSelect;
	delete [] Volume;
	delete [] Gen;
    
	delete Mixer;
}

void testApp::windowResized(int w, int h) {
    
}

//void testApp::audioRequested(float *output, int bufferSize, int numChannels) 
//{
//    ;;
//}

void testApp::audioInputListener(ofxAudioEventArgs &args){	
	// samples are "interleaved"

	if(setupFinished && args.deviceID == inputDeviceID){
		for (int i = 0; i < args.bufferSize; i++){
			leftAudioIn[i] = args.buffer[i*2];
			rightAudioIn[i] = args.buffer[i*2+1];
            
            pos++; // this gets incremented with every sample
            // when lengthOfOneBeatInSamples goes into
            // pos a whole number of times, we've entered a new quarter beat

            if(fmod(pos,lengthOfQuarterBeatInSamples)==0){
                if (Play.isOn) quarterBeatCounter++;
                if (quarterBeatCounter == limitSteps) quarterBeatCounter = 0;
                pos = 0;
            }

		}
		
		bpmCtrl.processFrame(&leftAudioIn[0], BUFFER_SIZE);
		
//		bufferCounter++;
	}//end if we have finished set up the floats to hold the audio
}

void testApp::setInputDevice(int i){
	setupFinished = false;
	inputSoundStream.close();
	inputDeviceID = i;
	inputSoundStream.setDeviceId(inputDeviceID);	//this now uses the audio input rather than mic input for mac 	
	inputSoundStream.setup(0, 2, this, 44100, BUFFER_SIZE, 4);
    
	long latency;
    //	latency = inputSoundStream.audio->getStreamLatency();
    //	cout << "stream latency is " << latency << endl;
    
	bpmCtrl.liveAudioAnalysis.clearVectors();
	setupFinished = true;
	inputSoundStream.deviceName = inputSoundStream.getDeviceNameFromID(inputSoundStream.deviceID);
}

void testApp::setBPM(float targetBPM){
    // NB. Currently the target BPM might not actually be achieved,
    // because permitted BPMs are limited to divisible by a whole number of samples.
    lengthOfOneBeatInSamples = (int)((SAMPLE_RATE*60.0f)/targetBPM);
	lengthOfQuarterBeatInSamples = (int)((SAMPLE_RATE*60.0f)/targetBPM/4);
    BPM=(SAMPLE_RATE*60.0f)/lengthOfOneBeatInSamples;
}

void testApp::newMessage(ofxMidiEventArgs &args){
	midiValue 		= args.byteTwo;
	midiId 			= args.byteOne;
//	midiPort 		= args.port;
	midiTimestamp 	= args.timestamp;
    
    // Turn Off other Sequencers except now selected
    if (midiId >= midiSeqActivationStartCC && midiId < midiSeqActivationStartCC+controllersNum && midiValue == 127)
        for (int i = 0; i < controllersNum; i++) {
            if (midiId != midiSeqActivationStartCC + i) { 
                Gen[i].Sequencer.setMidiActive(false), Gen[i].ledMatrix.setMidiActive(false); 
            };
//                midiOut.sendControlChange(midiPort, midiSeqActivationStartCC+i, 127);
        }

    // If switch to control ledMatrixe, turn of midi on active Sequencer
    if (midiId == midiLedMatrixActivationCC)
        for (int i = 0; i < controllersNum; i++) {
            if (midiValue == 127 && Gen[i].Sequencer.midiActive) {
//                Sequence[i].midiActive ? ledMatrix[i].setMidiActive(true) : ledMatrix[i].setMidiActive(false);
                Gen[i].ledMatrix.setMidiActive(true);
                Gen[i].Sequencer.setMidiActive(false);
            } else if (midiValue == 127 && Gen[i].ledMatrix.midiActive) {
//                ledMatrix[i].midiActive ? Sequence[i].setMidiActive(true) : Sequence[i].setMidiActive(false);
                Gen[i].Sequencer.setMidiActive(true);
                Gen[i].ledMatrix.setMidiActive(false);
            }
        }

    if (midiId == Start.midiId && midiValue == 127) quarterBeatCounter = 0, Play.receiveMidi(args);

    if (midiValue) printf("APP CHAN: %i ID: %i VALUE: %i \n", args.channel, midiId, midiValue);
}
