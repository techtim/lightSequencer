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
	
    //---MIDI---
    abletonCtrl = ABLETON_CTRL;
    
       //---Buttons---
	Play.setup(winWidth/2 - 50, 0, 100, 30, true);
	Play.setActictiveText("PLAY");
    Play.setInactictiveText("STOP");
    Play.setFontSize(14);
    
    Start.setup(winWidth/2 - 25, 40, 50, 30, false);
    Start.setActictiveText("start");
    Start.setFontSize(12);

//    ArdButton.setup(winWidth/2 - 95, 60, 50, 30, true);
//    ArdButton.setActictiveText("Serial");
//    ArdButton.setFontSize(12);
    OscButton.setup(winWidth/2 + 40, 60, 50, 30, true);
    OscButton.setActictiveText("OSC");
    OscButton.setFontSize(12);
    
//    MidiSelect.setup(winWidth/2 - 150, 0, 50, 30, true);
//	MidiSelect.setActictiveText("Ableton");
//    MidiSelect.setInactictiveText("Kontrol");
//    MidiSelect.setFontSize(10);
    
    LedAddrMode.setup(winWidth/2 - 95, 60, 50, 30, true);
    LedAddrMode.setActictiveText("ADDR ON");
    LedAddrMode.setInactictiveText("ADDR OFF");
    LedAddrMode.setFontSize(10);
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
  

	//---Layout
	controllersNum = 4;
	controllersWidth = 360;
    controllersHeight= 300;
    mixerRegion.leftX = controllersWidth;
	mixerRegion.leftY = 0;
	mixerRegion.rightX = ofGetWidth()-controllersWidth;
	mixerRegion.rightY = winHigh;

	region = new Region[controllersNum];
	region[0].leftX= 10;
	region[0].leftY= 0;
	region[2].leftX = 10;
	region[2].leftY = winHigh-controllersHeight-20;
	region[1].leftX = mixerRegion.rightX;
	region[1].leftY = 0;
	region[3].leftX = mixerRegion.rightX;
	region[3].leftY = winHigh-controllersHeight-20;
	
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
        mixerRegion.rightX - mixerRegion.leftX, mixerRegion.leftY+50
    );

//########################
    //---LED
    serialName = SERIAL_NAME;
//    serialName = "/dev/tty.usbmodem411";
	Arduino.setup(serialName, 115200);
    enableArduino = false;
//	Arduino.setVerbose(true);
	Arduino.flush();
    
	//---Matrix---
    matrixW = MATRIX_W;
    matrixH = MATRIX_H;
	
    matrixSpace = 2;
//    matrixCellSize = controllersWidth/(matrixW+matrixSpace);
    matrixCellSize=50;
    
//    projector.setup(matrixW, matrixH, projector_id);


    int syphonLedSize = // (PROJECTOR_W/matrixW) > (PROJECTOR_H/matrixH) ?
    PROJECTOR_H/matrixH;// : PROJECTOR_W/matrixW;
    
//########################
	//---Color---
    saturSelect = 255;
    valueSelect = 255;
	
	colorSelect = new ofColor[controllersNum];
	for (int i=0; i < controllersNum; i++) colorSelect[i].set(255.0f, 255.0f, 255.0f, 255.0f);

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
    
	// --- SEQ ---
	selectColor.set(0,200,50);
	actColor.set(0, 50, 200);
	inactColor.set(0, 0, 0);
	maxSteps = 16;
	limitSteps = 128;
	seqCellSize = 40;
	seqCellSpace = 5;
    
    midiSeqActivationStartCC = 1;
    midiSeqBeginCC = 10;
	midiPresetBeginCC = 50;
    midiHueControlCC = 74;
	
    midiLedMatrixActivationCC = 45;

    // --- GENERATORS ---
    
	Gen = new Generator[controllersNum];
    
    for (int i=0; i<controllersNum; i++)
	{
		Gen[i].setup(region[i].leftX, region[i].leftY, controllersWidth, controllersHeight, matrixW, matrixH, (i>1?true:false), i);
        Gen[i].setActive(false);
//		colorSaturation[i].setup(region[i].leftX+matrixCellSize/2, region[i].leftY+10, matrixCellSize/2, 50, false, 0, 255);		
    }

//    int ctrlLedSize = (mixerRegion.rightX-mixerRegion.leftX-40)/matrixW-matrixSpace;
    int ctrlLedSize = (mixerRegion.rightY-mixerRegion.leftY)/3/matrixH - matrixSpace;
    ctrlLedSize = matrixH < 2 ? (mixerRegion.rightX-mixerRegion.leftX-20)/matrixW-matrixSpace : ctrlLedSize;
//    ctrlLedSize = matrixH < 2 ? (mixerRegion.rightX-mixerRegion.leftX)/matrixW-matrixSpace : ctrlLedSize;
    int mixerWidth = mixerRegion.rightX - mixerRegion.leftX;
//	ledControl.set(matrixW, matrixH, ctrlLedSize, 
//                   mixerRegion.leftX +  matrixSpace*(matrixW-1)/2 - 30, winHigh/2-50, matrixSpace);
//    ledControl.set(<#int col#>, <#int row#>, <#int cell#>, <#int x#>, <#int y#>, <#int spac#>)
	ledControl.set(matrixW, matrixH, ctrlLedSize,
                   mixerRegion.leftX + mixerWidth/2-(ctrlLedSize+matrixSpace)*matrixW/2, winHigh/2.5-ctrlLedSize/2, matrixSpace);
//    ledControl.set(matrixW, matrixH, ctrlLedSize,
//                   mixerRegion.leftX + mixerWidth/2-(ctrlLedSize+matrixSpace)*matrixW/2, winHigh/2, matrixSpace);

    //    ledControl.set(matrixW, matrixH, 200, mixerRegion.leftX + 80, winHigh/2-50, matrixSpace);
	ledControl.setClickedAll();
//    ledControl.setupTexture(DISPLAY_W, DISPLAY_H);

	Mixer = new bitmapMixer;
	Mixer->setup(controllersNum, matrixW, matrixH);
    
//    mixerSaturation.setup(mixerRegion.leftX+(mixerRegion.rightX-mixerRegion.leftX)/2-255/2, 200, 255, 20, true, 0, 255);
    
    gui = new ofxUISuperCanvas("");
    mixerSaturation = new ofxUIMinimalSlider("saturate", 0, 255, &mixSaturation, mixerWidth-35, 10, mixerRegion.leftX+30,200);
    gui->addWidget(mixerSaturation);
    
    mixerBrightness = new ofxUIMinimalSlider("bright", 0, 255, &mixSaturation, mixerWidth-35, 10, mixerRegion.leftX+30,180);
    gui->addWidget(mixerBrightness);
    
//    (string _name, bool _value, float w = 0, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM

    loadColorButton = new ofxUILabelToggle("load color", true, (mixerWidth-35)/2, 10, mixerRegion.leftX+(mixerWidth-35)/3, 160 );
    gui->addWidget(loadColorButton);
    
    gui->setColorBack(ofColor(0,0,0,0));
    gui->setPosition(0, 0);
    //    volume = new ofxUIRangeSlider("vol", 0.f, 255.f, &minVolume, &maxVolume, 15, 255,
    //                                  leftX+width-10, hueLine.yPos+hueLine.height);

    gui->autoSizeToFitWidgets();
    gui->enable();
    gui->setAutoDraw(true);

    
    Presets.setup(mixerRegion.leftX+30, ledControl.yRight+10, mixerRegion.rightX-mixerRegion.leftX-60, winHigh/4, matrixW, matrixH, PRESETS_NUM);
    presetsShift = false;
    
    bMidiPortClosed = true;
    setupMidi();
    
    // DMX
    
    dmx.connect(DMX_PORT, 512);//DMX_MODULES * DMX_CHANNELS);
    dmx.update(true); // black on startup
    
    setupFinished = true;
    
    // OSC
    oscReceiver.setup(OSC_PORT_IN);
    
}

//--------------------------------------------------------------

void testApp::update(){
	frameCount++;
    if (OscButton.isOn) TCP.update();
	if (frameRate/30 == frameCount) frameCount = 0;
    
//    if (bpmCtrl.Sync.isOn &&
//    (bpmCtrl.liveAudioAnalysis.aubioOnsetDetectionVector.size() - bpmCtrl.liveAudioAnalysis.lastBeatTime) < 40){
//        midiTapOut.sendControlChange(1, 2, 127);
//        midiTapOut.sendControlChange(1, 2, 0 );
//    }
//    if (){};
    midiInPort = midiIn.getPortByName( abletonCtrl ? "IAC Driver Bus 1" : "nanoKONTROL" );
    midiOutPort = midiIn.getPortByName( abletonCtrl ? "IAC Driver Bus 1" : "nanoKONTROL" );
	if (midiInPort == 100 || midiOutPort == 100 || bMidiPortClosed)
        setupMidi();
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        vector<string> addr = ofSplitString(m.getAddress(), "/");
        
        if (addr[0] == "wii" ) {
            
            if (addr[3] == "B") {;}
                
            if (addr[3] == "A") mixerSaturation->setValue(m.getArgAsInt32(0));
            
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(0, 0, 0);
    ofNoFill();
    // BUTTONS
	Play.draw();
    Start.draw();
    OscButton.draw();
    ArdButton.draw();
    MidiSelect.draw();
    LedAddrMode.draw();

    gui->setPosition(0, 0);
    
	bpmCtrl.draw();
    BPMstring = ofToString(bpmCtrl.BPM, 1);
    verdana30.drawString(BPMstring, bpmCtrl.leftX + bpmCtrl.width/2 - verdana30.stringWidth(BPMstring)/2, bpmCtrl.leftY+30);
    setBPM(bpmCtrl.BPM);
//    printf(BPMstring.data());

    Presets.draw();

    Mixer->setSaturation(255-mixerSaturation->getValue());
	for (int i=0; i<controllersNum; i++)
	{
		ofSetColor(0, 200, 55);
		
        Gen[i].draw(quarterBeatCounter);
        
//        Volume[i].draw();

        ofColor * seqBitmap;
		seqBitmap = Gen[i].getSequencedBitmap();
		Mixer->feedBitmap(seqBitmap, i, 255);
	}

	ledControl.parseBitmap(Mixer->outputMixedChar());
    ledControl.setSaturation(255-mixerSaturation->getValue());

	ledControl.print();

    if (ofGetFrameNum()%2==0) {
        ledControl.getDmx(dmx);
        if(dmx.isConnected()) {
            dmx.update();
        } else {
            ofSetColor(255);
            ofDrawBitmapString("COuld not connect to port " + ofToString(DMX_PORT), 400, 10);
        }
    }
    ofColor * colorPix = ledControl.getBitmap();

	if ((OscButton.isOn || ArdButton.isOn) && frameCount == 0) // && enableArduino)
	{

        if (OscButton.isOn) {
            if (OscButton.isOn) {
                unsigned char * matrixPixels = new unsigned char [ledControl.ledsInChain*3];
                ledControl.getChainBitmapChar(matrixPixels);

                ofxOscMessage m;
                //	m.setAddress( "/mouse/position" );
                m.setAddress( "/led/aaa" );
                for (unsigned int i=0; i < ledControl.ledsInChain*3;i+=3) {
                    m.addIntArg( matrixPixels[i] );
                    m.addIntArg( matrixPixels[i+1] );
                    m.addIntArg( matrixPixels[i+2] );
                }
//                oscSender.sendMessage( m );
            }
		}


	}
    ofColor col = ofColor(0);
    col;
    col.setBrightness(255);
    col;
	ofSetWindowTitle("FPS: " + ofToString((int)ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='a') {
        ;;
    }
}

void testApp::keyReleased(int key) {
    //cout << (0x0400) << endl;
    //cout << (101 | OF_KEY_MODIFIER) << " " << key << endl;
//    ofxFensterManager::getWindowById  ofxFenster* proj_win=
//    ofxDisplayList displays = ofxDisplayManager::get()->getDisplays();
//	cout << "NUMBER OF DISPLAYS FOUND " << displays.size()<< endl;
//	ofxDisplay* display = displays[0];
//    if(key=='f') {
//        if (projector_id == win->id) {
//            win->windowResized(PROJECTOR_W, PROJECTOR_H);
//        } else {
//            win->windowResized(DISPLAY_W, DISPLAY_H);
//        }
//        win->toggleFullscreen();
//    }
    if(key=='f') {
        ofToggleFullscreen();  
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    if (Presets.isClicked(x,y) && Presets.isLedsClicked) {
        if (Presets.isRec) {
//            printf("PRESETS SAVE = %i \n", Presets.ledLastClicked);
            for (int i=0; i<4; i++) Presets.save(Presets.ledLastClicked, i, Gen[i]);
        } else {
//            printf("PRESETS LOAD = %i \n", Presets.ledLastClicked);
            for (int i=0; i<4; i++) Presets.load(Presets.ledLastClicked, i, Gen[i], loadColorButton->getValue());
        }
    };
    Play.isClicked(x,y);
    if (MidiSelect.isClicked(x,y)) {
        abletonCtrl = MidiSelect.isOn ? 1 : 0;
        setupMidi();
    }
    bpmCtrl.isClicked(x,y);
    if (ArdButton.isClicked(x,y)) {
        if (ArdButton.isOn) {
            Arduino.setup(serialName, 115200);
//            Arduino.setVerbose(true);
            Arduino.flush();
        } else {
//            Arduino.close();
        }
    }
    
    if (LedAddrMode.isClicked(x, y)) {
//        ledControl.setMidiActive(LedAddrMode.isOn);

        ledControl.setAddrMode(LedAddrMode.isOn);
//        LedAddrMode.isOn ?
//            ledControl.setChainState(Presets.getChainState()) :
//            Presets.setChainState(ledControl.getChainState());
    }
    if (LedAddrMode.isOn && ledControl.isClicked(x, y)) {
        ledControl.setClicked(x,y, ofColor(200,200,0));
        ledControl.updateColor(ofColor(200,200,0));
    }

    if (OscButton.isClicked(x,y)) {
        if (!TCP.weConnected) TCP.setup();
    };
}

void testApp::exit()
{
	//if (!setupAudioButton.getState() && soundInited) ofSoundStreamStop();
	ofRemoveListener(inputSoundStream.audioReceivedEvent, this, &testApp::audioInputListener);
	Arduino.flush();
    if (ArdButton.isOn) Arduino.close();
    
	leftAudioIn.clear();
	rightAudioIn.clear();

	delete [] region;
	delete [] colorSelect;

//	delete [] Gen; # don't know why "pointer being freed was not allocated"
    
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
    printf("inputSoundStream.close() \n");
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

void testApp::setupMidi() {
    //---MIDI---
//    abletonCtrl = ABLETON_CTRL;
	midiIn.listPorts();
    midiOut.listPorts();
    
    midiInPort = midiIn.getPortByName( abletonCtrl ? "IAC Driver IAC_Bus_1" : "nanoKONTROL" );
    midiOutPort = midiOut.getPortByName("nanoKONTROL");
    midiTapPort = 100; // midiOut.getPortByName("IAC Driver IAC_Bus_2");
    
	if (midiInPort != 100 && midiOutPort != 100) {
        if (bMidiPortClosed) {
//            midiIn.closePort();
//            midiOut.closePort();
            midiIn.openPort(midiInPort); // opens a connection with the device at port 0 (default)
            midiOut.openPort(midiOutPort);
            bMidiPortClosed = false;
            
            Play.setupMidi(41, 1, midiInPort, midiOutPort);
            Play.setMidiActive(true);
            Start.setupMidi(43, 1, midiInPort, midiOutPort);
            //    Start.setMidiActive(true);
            bpmCtrl.setupMidi(0, 1, midiInPort, midiOutPort);
            
            int midiChannel;
            for (int i=0; i<controllersNum; i++)
            {
                //        midiChannel = i+1;
                midiChannel = 1;
                Gen[i].setupMidi(midiInPort, midiOutPort,
                                 (abletonCtrl? 0 : midiSeqActivationStartCC+i), midiSeqBeginCC,
                                 (abletonCtrl? 0 : midiLedMatrixActivationCC), (midiHueControlCC+i),
                                 midiChannel);
                Gen[i].setActive(abletonCtrl ? true : false);
            }
            
            printf("\nOPENED PORT -> in: %i -> out: %i \n", midiInPort, midiOutPort);
            if (midiInPort != 100) ofAddListener(midiIn.newMessageEvent, this, &testApp::newMessage);
        }
    } else {
        bMidiPortClosed = true;
    }
    
    if (midiTapPort != 100) {
        midiTapOut.openPort(midiTapPort);
    }
    
//    mixerSaturation.setupMidi(MIDI_MIX_SATURATION, 1, midiInPort, false);

}

void testApp::newMessage(ofxMidiEventArgs &args){
	midiValue 		= args.byteTwo;
	midiId 			= args.byteOne;
//	midiPort 		= args.port;
	midiTimestamp 	= args.timestamp;
    
    if (midiId == Start.midiId && midiValue == 127) quarterBeatCounter = 0, Play.receiveMidi(args);
    // Turn Off other Sequencers except now selected
//    if (!abletonCtrl) {
    if (midiId >= midiSeqActivationStartCC && midiId < midiSeqActivationStartCC+controllersNum && midiValue == 127)
        {
//            Gen[midiId-midiSeqActivationStartCC];
//            bool act = Gen[midiId-midiSeqActivationStartCC].isActive();
//            unsigned int num = midiId-midiSeqActivationStartCC;
//            if (act) {
//                Gen[num].Sequencer.setMidiActive(false), Gen[num].ledMatrix.setMidiActive(false);
//                Gen[num].setActive(false);
//            } else {
//                Gen[num].setActive(true);
//            };

        }
//            Gen[i].isActive();
    
//            for (int i = 0; i < controllersNum; i++) {
//                if (midiId != midiSeqActivationStartCC + i) { 
//                  Gen[i].Sequencer.setMidiActive(false), Gen[i].ledMatrix.setMidiActive(false);
//                    Gen[i].setActive(false);
//                    Gen[i].isActive();
//                } else {
//                    Gen[i].setActive(true);
//                };
//                midiOut.sendControlChange(midiPort, midiSeqActivationStartCC+i, 127);
//            Gen[midiId-midiSeqActivationStartCC].setActive(true);
//            }

    if (midiId >= midiPresetBeginCC && midiId < midiPresetBeginCC+20) {
//            printf("PRESETS LOAD = %i \n", midiId - 10 + (presetsShift ? 16 : 0));
        for (int i=0; i<4; i++) Presets.load(midiId - midiPresetBeginCC, i, Gen[i]);
    };

    
    if (midiId == MIDI_MIX_SATURATION) mixerSaturation->setValue(midiValue ? ofMap(midiValue, 1, 127, 1, 255) : 0);
    
//    if (midiValue) printf("APP CHAN: %i ID: %i VALUE: %i \n", args.channel, midiId, midiValue);
}
