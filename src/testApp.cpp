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
    ofSetDataPathRoot("../Resources/data/");
    
	ofBackground(40,40,40);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetCircleResolution(40);    

    ofSetEscapeQuitsApp( false );
    
//    mainMenuController = nil;
//    
//    mainMenuController = [[MainMenuController alloc] init];
    
	winWidth = ofGetWidth();
	winHigh = ofGetHeight();
	
    //---MIDI---
    abletonCtrl = ABLETON_CTRL;
    

	//---BPM
    leftAudioIn.assign(BUFFER_SIZE, 0.0);
	rightAudioIn.assign(BUFFER_SIZE, 0.0);
    
    inputDeviceID = 0;//the mic(0)/line input(1) of a mac book pro
	outputDeviceID = 2;//the headphones out of a macbook pro
#ifdef USE_AUDIO 
	setInputDevice(inputDeviceID);//includes calling the setup routine etc
	ofAddListener(inputSoundStream.audioReceivedEvent, this, &testApp::audioInputListener);
#endif

    verdana14.loadFont("../data/verdana.ttf", 14, true, true);
	verdana14.setLineHeight(14.0f);
	verdana14.setLetterSpacing(1.035);
    
    verdana30.loadFont("../data/verdana.ttf", 14, true, true);
	verdana30.setLineHeight(14.0f);
	verdana30.setLetterSpacing(1.035);
  
    

	//---Layout
	controllersNum = 4;
	controllersWidth = 360;
    controllersHeight= 300;
    unsigned int cntrlOffsetX = 5;
    
    mixerRegion.leftX = controllersWidth+cntrlOffsetX+5;
	mixerRegion.leftY = 0;
	mixerRegion.rightX = ofGetWidth()-controllersWidth-5;
	mixerRegion.rightY = winHigh;

	region = new Region[controllersNum];
	region[0].leftX= cntrlOffsetX;
	region[0].leftY= 0;
	region[2].leftX = cntrlOffsetX;
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
	
    
	//---Matrix---
    matrixW = MATRIX_W;
    matrixH = MATRIX_H;
	
    matrixSpace = 2;
//    matrixCellSize = controllersWidth/(matrixW+matrixSpace);
    matrixCellSize=50;
    
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
    }

    bpmCtrl.setup(
                  mixerRegion.leftX, mixerRegion.leftY + 50,
                  mixerRegion.rightX - mixerRegion.leftX, mixerRegion.leftY+50
                  );

    int mixerWidth = mixerRegion.rightX - mixerRegion.leftX;

    int ctrlLedSize = (mixerRegion.rightY-mixerRegion.leftY)/3/matrixH - matrixSpace;
    
    ctrlLedSize = ctrlLedSize > mixerWidth/matrixW - matrixSpace ?
         mixerWidth/matrixW-matrixSpace : ctrlLedSize;

//	ledControl.set(matrixW, matrixH, ctrlLedSize,
//                   mixerRegion.leftX +  matrixSpace*(matrixW-1)/2 - 30, winHigh/2-50, matrixSpace);
//    ledControl.set(<#int col#>, <#int row#>, <#int cell#>, <#int x#>, <#int y#>, <#int spac#>)
	ledControl.set(matrixW, matrixH, ctrlLedSize,
                   mixerRegion.leftX + mixerWidth/2-(ctrlLedSize+matrixSpace)*matrixW/2, winHigh/2.5-ctrlLedSize/2, matrixSpace);
//    ledControl.set(matrixW, matrixH, ctrlLedSize,
//                   mixerRegion.leftX + mixerWidth/2-(ctrlLedSize+matrixSpace)*matrixW/2, winHigh/2, matrixSpace);

    //    ledControl.set(matrixW, matrixH, 200, mixerRegion.leftX + 80, winHigh/2-50, matrixSpace);
	ledControl.setClickedAll();
    ledControl.setupDmx();
//    ledControl.setupTexture(DISPLAY_W, DISPLAY_H);

	Mixer = new bitmapMixer;
	Mixer->setup(controllersNum, matrixW, matrixH);
    
    //---Buttons---
    
    gui = new ofxUISuperCanvas("");
    
    bPlaying = false;
    Play = new ofxUIMultiImageToggle(mixerRegion.leftX+mixerWidth/2-32, 10, 32, 32, &bPlaying, "GUI/play_.png", "PLAYPAUSE");
    Play->setLabelVisible(false);
    gui->addWidget(Play);
    
    Start = new ofxUIMultiImageButton(mixerRegion.leftX+mixerWidth/2, 10, 32, 32, false, "GUI/start_.png", "START");
//    Start->setLabelVisible(false);
    gui->addWidget(Start);
    
    //    ArdButton.setup(winWidth/2 - 95, 60, 50, 30, true);
    //    ArdButton.setActictiveText("Serial");
    //    ArdButton.setFontSize(12);
    
    //    MidiSelect.setup(winWidth/2 - 150, 0, 50, 30, true);
    //	MidiSelect.setActictiveText("Ableton");
    //    MidiSelect.setInactictiveText("Kontrol");
    //    MidiSelect.setFontSize(10);
    
    LedAddrMode = new ofxUILabelToggle("Addr Mode", false, 70, 10, Start->getRect()->x+Start->getRect()->width+10, 20, OFX_UI_FONT_SMALL );

    gui->addWidget(LedAddrMode);
    
    OscButton = new ofxUILabelToggle("OSC", false, 30, 10, mixerRegion.leftX+60, 60 );
//    gui->addWidget(OscButton);
    
    mixSaturation = 0.f;
    mixerSaturation = new ofxUIMinimalSlider("saturate", 0, 255, &mixSaturation, mixerWidth-30, 10, mixerRegion.leftX+15,110);
    gui->addWidget(mixerSaturation);
    
    mixBrightness = 255.f;
    mixerBrightness = new ofxUIMinimalSlider("bright", 0, 255, &mixBrightness, mixerWidth-30, 10, mixerRegion.leftX+15,140);
    gui->addWidget(mixerBrightness);
    
//    (string _name, bool _value, float w = 0, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM

    loadColorButton = new ofxUILabelToggle("load color", true, (mixerWidth-35)/2, 10, mixerRegion.leftX+(mixerWidth-35)/3, ledControl.yRight+10, OFX_UI_FONT_SMALL);
    gui->addWidget(loadColorButton);
    
    gui->setColorBack(ofColor(0,0,0,0));
    gui->setPosition(0, 0);

    gui->autoSizeToFitWidgets();
    gui->enable();
    gui->setAutoDraw(true);

    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    
    Presets.setup(mixerRegion.leftX+30, ledControl.yRight+40, mixerRegion.rightX-mixerRegion.leftX-60, winHigh/4, matrixW, matrixH, PRESETS_NUM);
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
    if (OscButton->getValue()) TCP.update();
	if (frameRate/30 == frameCount) frameCount = 0;
    
    ledControl.setAddrMode(LedAddrMode->getValue());
//    if (bpmCtrl.Sync.isOn &&
//    (bpmCtrl.liveAudioAnalysis.aubioOnsetDetectionVector.size() - bpmCtrl.liveAudioAnalysis.lastBeatTime) < 40){
//        midiTapOut.sendControlChange(1, 2, 127);
//        midiTapOut.sendControlChange(1, 2, 0 );
//    }
//    if (){};

//    midiInPort = midiIn.getPortByName( abletonCtrl ? "IAC Driver Bus 1" : "nanoKONTROL" );
//    midiOutPort = midiIn.getPortByName( abletonCtrl ? "IAC Driver Bus 1" : "nanoKONTROL" );
//	if (midiInPort == 100 || midiOutPort == 100 || bMidiPortClosed)
//        setupMidi();
    
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
    // BUTTONS

    gui->setPosition(0, 0);
    
	bpmCtrl.draw();
    BPMstring = ofToString(bpmCtrl.BPM, 1);
    verdana30.drawString(BPMstring, bpmCtrl.leftX + bpmCtrl.width/2 - verdana30.stringWidth(BPMstring)/2, bpmCtrl.leftY+30);
    setBPM(bpmCtrl.BPM);
//    printf(BPMstring.data());

    Presets.draw();

    Mixer->setSaturation(255-mixerSaturation->getValue());
    Mixer->setBrightness(mixerBrightness->getValue());

	for (int i=0; i<controllersNum; i++)
	{
		ofSetColor(0, 200, 55);
		
        Gen[i].draw(quarterBeatCounter);

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
        }
    }
    
    if(!dmx.isConnected()) {
        ofSetColor(255,0,0);
        ofDrawBitmapString("NO USB->DMX", ofGetWidth()/2-45, 10);
    }

    ofColor * colorPix = ledControl.getBitmap();

	if (OscButton->getValue() && frameCount == 0) // && enableArduino)
	{


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

	ofSetWindowTitle("FPS: " + ofToString((int)ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='a') {
        ;;
    }
    if(key == ' ') {
        mixerBrightness->setValue(0.f);
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
    if(key == ' ') {
        mixerBrightness->setValue(255.f);
    }
    if(key == 's') {
        ledControl.saveDmxConfig();
    }
    if(key == 'l') {
        ledControl.loadDmxConfig();
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

//    if (MidiSelect.isClicked(x,y)) {
//        abletonCtrl = MidiSelect.isOn ? 1 : 0;
//        setupMidi();
//    }
    bpmCtrl.isClicked(x,y);

    if (LedAddrMode->getValue() && ledControl.isClicked(x, y)) {
        ledControl.setClicked(x,y, ofColor(200,200,0));
//        ledControl.updateColor(ofColor(200,200,0));
    }

}

void testApp::guiEvent(ofxUIEventArgs &e){
    
    if (e.widget == Start) {
        quarterBeatCounter = 0;
    }
    else if (e.widget == LedAddrMode) {
        
    }
    else if (e.widget == OscButton) {
        if (!TCP.weConnected) TCP.setup();
    }
}

void testApp::exit()
{
	//if (!setupAudioButton.getState() && soundInited) ofSoundStreamStop();
#ifdef USE_AUDIO
	ofRemoveListener(inputSoundStream.audioReceivedEvent, this, &testApp::audioInputListener);
	leftAudioIn.clear();
	rightAudioIn.clear();
#endif
    
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
                if (bPlaying) quarterBeatCounter++;
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
            
//            Play.setupMidi(41, 1, midiInPort, midiOutPort);
//            Play.setMidiActive(true);
//            Start.setupMidi(43, 1, midiInPort, midiOutPort);
            
            
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
    
    if (midiId == MIDI_SEQ_START_CC && midiValue == 127) quarterBeatCounter = 0;// Play->setValue(!Play->getValue());
    if (midiId == MIDI_SEQ_PLAY_CC && midiValue == 127) Play->setValue(!Play->getValue());
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

void testApp::toggleAddressMode() {
    LedAddrMode->setValue(!LedAddrMode->getValue());
}