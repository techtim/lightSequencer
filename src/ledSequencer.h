#ifndef _ledSEQ
#define _ledSEQ

#include "ofMain.h"
#include "ofColor.h"
#include "LEDS.h"
#include "ofxSlider.h"
#include "ofxMidi.h"
#include "ofxButton.h"

class ledSequencer : public LEDS {
	public:
		ledSequencer();
		~ledSequencer();
		void Create(unsigned int steps, unsigned int x, unsigned int y, 
					unsigned int cell, unsigned int spac, unsigned int seqBitLen,
					ofColor selColor, ofColor actColor, ofColor inactColor);
		bool isClicked(int x, int y, bool dragged = false);
		bool isSelectedStep(int step);
		void setStep(int step, ofColor * feedBitmap);
		
		unsigned int quant;
		ofxButton button8;
		ofxButton button16;

        void setQuant8() {
			quant = 2;
            button8.isOn = true;
            button16.isOn = false;
            setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
//            printf("A:%f D:%f R:%f", a,d,r);
		}

		void setQuant16() {
			quant = 1; 
            button16.isOn = true;
            button8.isOn = false;
            setADSR(sliders[0].getValue(), sliders[1].getValue(), sliders[2].getValue());
//            printf("A:%f D:%f R:%f", a,d,r);
		}

		ofColor * getSequencedBitmap();
		unsigned char * getSequencedBitmapChar();
		unsigned int seqBitmapLen;
		unsigned char * sequencedBitmapChar;
		ofColor * sequencedBitmap;
	
		void print();

		void setADSR(float _a, float _d, float _r){
			a=_a*quant; d=_d*quant; r=_r*quant;
			attack = _a; decay = _d; release = _r;
            sliders[0].setValueReal(_a) , sliders[1].setValueReal(_d), sliders[2].setValueReal(_r);
		};

		ofxSlider * sliders;
	
		float a, d, s, r;
		unsigned int ADSRoffset;
		float ADSRvalue;
	
		unsigned int steps;
		unsigned int currentStep;

		unsigned int attack, decay, release;
		ofColor inactiveColor;
		ofColor activeColor;
		ofColor selectColor;
	
		//---MIDI---
        void setupMidi(unsigned int midiSeqStart, unsigned int channel, unsigned int InPort, unsigned int OutPort);
		void newMessage(ofxMidiEventArgs &args);
		ofxMidiIn	midiIn;
		ofxMidiOut	midiOut;
        unsigned int midiInPort, midiOutPort;
		unsigned int midiChannel; // channel for exact sequence , setuped 
		unsigned int midiId;
		unsigned int midiValue;
        
        unsigned int midiActivationCC;
        unsigned int midiSeqStartCC;
        bool midiActive;
        void setMidiActive(bool is) {
            midiActive = is;
        }

        void setMidiActivationCC(unsigned int cc) {
            midiActivationCC = cc;
        }
};







#endif
