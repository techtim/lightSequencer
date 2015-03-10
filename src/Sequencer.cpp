//
//  Sequencer.cpp
//  dmxSequencer
//
//  Created by Tim TVL on 24/03/14.
//
//

#include "Sequencer.h"

Sequencer::Sequencer():
midiButton8Id(30), midiButton16Id(31), midiButton32Id(32), midiAttSliderId(100), midiDecSliderId(101), midiRelSliderId(102),
currentStep(1000), // big value to work on step == 0
ADSRoffset(0), ADSRvalue(0),
midiId(0), midiValue(0), stepTimestamp(0),
a(0), d(0), s(0), r(0)
{
    
}

void Sequencer::setup(
                     unsigned int stepsCnt, unsigned int x, unsigned int y,
                     unsigned int cell, unsigned int spac, unsigned int seqBitLen,
                     ofColor selColor, ofColor actColor, ofColor inactColor)
{
    //	set(int col, int row, int cell, int x, int y, int spac);
	steps = stepsCnt;
	cellSize = cell;
    columns = steps/2;
    rows = 2;

	LEDS::set(steps/2, 2, cell, x+cell+spac, y, spac);
	seqBitmapLen = seqBitLen;
	sequencedBitmap = new ofColor [seqBitmapLen];
	sequencedBitmapChar = new unsigned char [seqBitmapLen*3];
	color = selColor;
	inactiveColor = inactColor;
	activeColor = actColor;
    
	//---Controll---
	int slidersLen = 255;
	slidersOffset = yRight+mSpace;

    //    attSlider = new ofxUIMinimalSlider(_name, _min, _max, _value, rect->getWidth()-widgetSpacing*2, globalSliderHeight, 0, 0, size);
    sliders = new ofxUISuperCanvas("sliders", 0,0,0,10);
    sliders->setWidgetFontSize(OFX_UI_FONT_SMALL);
    sliders->setFontSize(OFX_UI_FONT_SMALL, 5);
    sliders->setPosition(xLeft-cellSize, slidersOffset-10);

    attSlider = new ofxUIMinimalSlider("att", 0, 255, &a, getWidth()+cellSize, 10);
    decSlider = new ofxUIMinimalSlider("dec", 0, 255, &d, getWidth()+cellSize, 10);
    relSlider = new ofxUIMinimalSlider("rel", 0, 255, &r, getWidth()+cellSize, 10);

    sliders->addWidgetDown(attSlider);
    sliders->addWidgetDown(decSlider);
    sliders->addWidgetDown(relSlider);

//    attSlider = sliders->addMinimalSlider("att", 0, 255, &a, getWidth()+cellSize, 5);
//    decSlider = sliders->addMinimalSlider("dec", 0, 255, &d, getWidth()+cellSize, 5);
//    relSlider = sliders->addMinimalSlider("rel", 0, 255, &r, getWidth()+cellSize, 5);
    
//    attSlider = sliders->addSlider("att", 0, 255, &a);
//    decSlider = sliders->addSlider("dec", 0, 255, &d);
//    relSlider = sliders->addSlider("rel", 0, 255, &r);

    sliders->autoSizeToFitWidgets();
//    sliders->setGlobalSliderHeight(5);
    sliders->setGlobalSliderHeight(5);
    //    sliders->setColorFill(ofColor(0,0,0,0));
    //    sliders->setColorPadded(ofColor(0,0,0,0));
    sliders->setColorBack(ofColor(0,0,0,0));
    
	setADSR(10.0f,10.0f,10.0f);
	a = d = r = 10.f;

    bQuant32     = false;
    bQuant16    = true;
    bQuant8     = false;
    
    buttons =  new ofxUISuperCanvas("", OFX_UI_FONT_SMALL);
//    ofxUILabelToggle(<#string _name#>, <#bool *_value#>)
    button8     = new ofxUILabelToggle("8", bQuant8, cellSize, cellSize, x, yLeft, OFX_UI_FONT_SMALL);
    button16    = new ofxUILabelToggle("16", bQuant16, cellSize, cellSize, x, yLeft+cell+spac, OFX_UI_FONT_SMALL);
    button32    = new ofxUILabelToggle("32", bQuant16, cellSize, cellSize, x-cell-spac, yLeft+cell+spac, OFX_UI_FONT_SMALL);
    buttons->addWidget(button8);
    buttons->addWidget(button16);
    buttons->addWidget(button32);
    
    buttons->setColorBack(ofColor(0,0,0,0));
    buttons->autoSizeToFitWidgets();
    buttonsPos = ofVec2f(buttons->getRect()->x,buttons->getRect()->y);
	setQuant16();
    
    
    ofAddListener(sliders->newGUIEvent, this, &Sequencer::guiEvent);
    ofAddListener(buttons->newGUIEvent, this, &Sequencer::guiEvent);
}

Sequencer::~Sequencer()
{
    //	delete [] leds;
    //	delete [] bitmap;
	delete [] sequencedBitmap;
    //	delete [] sequencedBitmapChar;
    //	delete [] sliders;
    ;
}

bool Sequencer::isSelectedStep(int step)
{
	if (!leds[step-1].isSelected)
		(step == 0 ? leds[steps-1].color = inactiveColor : leds[step-1].color = inactiveColor);
    
	leds[step].color = color;
	//leds[step].isSelected = true;
	//printf("leds[%i].color=%f", step, leds[step].color.r);
	if (leds[step].isSelected && (currentStep != step))
	{
		setADSR(attSlider->getValue(), decSlider->getValue(), relSlider->getValue());
        stepTimestamp = ofGetFrameNum();
		currentStep = step;
		return true;
	}
	currentStep = step;
	return false;
}

void Sequencer::setStep(int step, ofColor * feedBitmap)
{
	const int stp = (step/quant)%steps;
	if (isSelectedStep(stp))
	{
		ADSRoffset = 0;
	} else {
        ADSRoffset++;
    }
	memcpy(sequencedBitmap, feedBitmap, seqBitmapLen*sizeof(ofColor));
	setADSR(attSlider->getValue(), decSlider->getValue(), relSlider->getValue());
}

void Sequencer::setADSR(float _a, float _d, float _r){
    a=_a*quant; d=_d*quant; r=_r*quant;
    attack = _a; decay = _d; release = _r;
    
    ((ofxUISlider*)sliders->getWidget("att"))->setValue(_a);
    ((ofxUISlider*)sliders->getWidget("dec"))->setValue(_d);
    ((ofxUISlider*)sliders->getWidget("rel"))->setValue(_r);
    //        sliders[0].setValueReal(_a) , sliders[1].setValueReal(_d), sliders[2].setValueReal(_r);
};

ofVec3f Sequencer::getADSR() {
    return ofVec3f(a, d, r);
}

ofColor * Sequencer::getSequencedBitmap()
{
	if (a == 0 && d == 0 && r ==0 ) {
        ADSRvalue = 0;
    } else if (ADSRoffset<=a){ // attack
		ADSRvalue += 255.0/a;
        if (ADSRvalue > 255) ADSRvalue = 255.0;
	} else if (ADSRoffset>a && ADSRoffset<=a+d) { // decay
 		ADSRvalue = 255;
		ADSRoffset++;
	} else if (ADSRoffset<=r+a+d){ // release
		(ADSRvalue == 0.0 ? ADSRvalue = 255.0 : (unsigned int)(ADSRvalue -= 255.0/r));
		if (ADSRvalue < 0) ADSRvalue = 0;
	} else {
		ADSRvalue = 0;
	}
	ADSRoffset++;
    //	if (midiChannel ==1) printf("\nOFFSET=%i, ADSRVAL=%f \n", ADSRoffset, ADSRvalue);
	
	for (int i=0; i<seqBitmapLen; i++)
	{
        if (sequencedBitmap[i].getHex() != 0) {
			sequencedBitmap[i].setBrightness(ADSRvalue);
		}
	}
	return sequencedBitmap;
}

unsigned char * Sequencer::getSequencedBitmapChar()
{
    sequencedBitmap = getSequencedBitmap();

	int count=0;
	for (int i=0; i<seqBitmapLen; i++)
    {
        sequencedBitmapChar[count++] = sequencedBitmap[i].r;
        sequencedBitmapChar[count++] = sequencedBitmap[i].g;
        sequencedBitmapChar[count++] = sequencedBitmap[i].b;
    }
	
	return sequencedBitmapChar;
}

void Sequencer::draw()
{
    int x = xLeft;
    int y = yLeft;
    int counter = 0;

    sliders->setPosition(xLeft-cellSize, slidersOffset-10);
    buttons->setPosition(buttonsPos.x, buttonsPos.y);

    if (midiActive) {
        ofSetColor(150, 150, 150);
        ofRect(x-mSpace, y-mSpace, (cellSize + mSpace) * columns+mSpace, (cellSize + mSpace) * rows+mSpace);
        for (int i=0; i < rows * columns; i++) {
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, leds[i].isSelected ? 127 : 0);
        }

        if(ofGetFrameNum()%2 == 0) {
            midiOut.sendControlChange(midiChannel, midiActivationCC, 127);

            midiOut.sendControlChange(midiChannel, midiSeqStartCC+currentStep, 127);
            currentStep == 0 ?
            midiOut.sendControlChange(midiChannel, midiSeqStartCC+steps-1, leds[steps-1].isSelected ? 127 : 0) :
            midiOut.sendControlChange(midiChannel, midiSeqStartCC+currentStep-1, leds[currentStep-1].isSelected ? 127 : 0) ;
        }
    }
    
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (leds[counter].isSelected == true)
            {
                ofSetColor(leds[counter].color.r, leds[counter].color.g, leds[counter].color.b);
			}
            else if (counter == currentStep) {
                ofSetColor(150);
            }
			else
            {
                ofSetColor(0,0,0);
            }
            ofRect(x, y, cellSize, cellSize);
            x += cellSize + mSpace;
            counter++;
        }
        y += cellSize + mSpace;
        x = xLeft;
    }

	updateColor(activeColor, inactiveColor);

    sliders->draw();
//    button8->draw();
//	button16->draw();
    ofNoFill();
    ofSetColor(100);
    ofRect(getRegion());
    ofFill();

};

// ---MIDI---
void Sequencer::setupMidi(unsigned int midiSeqStart, unsigned int channel, unsigned int inPort, unsigned int outPort) {

	midiChannel = channel;
	midiInPort = inPort;
    midiOutPort = outPort;

    midiIn.closePort();
    midiOut.closePort();
    midiIn.removeListener(this);
    midiIn.openPort(midiInPort); // opens a connection with the device at port 0 (default)
    midiIn.ignoreTypes(false, false, false);
    midiOut.openPort(midiOutPort);
    midiIn.addListener(this);

    midiActive = false;
    midiSeqStartCC = midiSeqStart;

}

void Sequencer::guiEvent(ofxUIEventArgs &e){

    if (e.widget == button8) {
        setQuant8();
    }
    else if (e.widget == button16) {
        setQuant16();
    }
    else if (e.widget == button32) {
        setQuant32();
    }
    
}

bool Sequencer::isClicked(int x, int y, bool dragged)
{
	return LEDS::isClicked(x, y);
}
//
void Sequencer::newMidiMessage(ofxMidiMessage &args){
//	if (midiActivationCC == args.control && args.value == 127 && args.channel == midiChannel) midiActive = !midiActive;

    if (midiActive) {
        //        for (int i=0; i<3; i++) sliders[i].receiveMidi(args); // send command right to slider
        midiOut.sendControlChange(midiChannel, midiActivationCC, 0); // turn off activation button

        if (midiButton8Id == args.control) {
            setQuant8();
        }
        else if (midiButton16Id == args.control) {
            setQuant16();
        }
        else if (midiButton32Id == args.control) {
            setQuant32();
        }
        else if (midiAttSliderId == args.control) {
            attSlider->setValue(mapNonLinear(args.value, 0, 127, 0, 255, 2));
        }
        else if (midiDecSliderId == args.control) {
            decSlider->setValue(mapNonLinear(args.value, 0, 127, 0, 255, 2));
        }
        else if (midiRelSliderId == args.control) {
            relSlider->setValue(mapNonLinear(args.value, 0, 127, 0, 255, 2));
        }
    } else {
        midiOut.sendControlChange(midiChannel, midiActivationCC, 0); // turn off activation button
    }

	if (midiActive && args.channel == midiChannel &&
        args.control >= midiSeqStartCC &&
        args.control<midiSeqStartCC+steps && args.value == 127
        )
		leds[args.control-midiSeqStartCC].isSelected = (leds[args.control-midiSeqStartCC].isSelected == true ? false : true);
}

