/*
 *  bitmapMixer.h
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _bitmapMix
#define _bitmapMix

#include "ofMain.h"
#include "ofColor.h"

class bitmapMixer{
	public:
		bitmapMixer();
		~bitmapMixer();
		bitmapMixer(unsigned int chanCount, unsigned int bitmapLen);

		void setup(unsigned int chanCount, unsigned int cls, unsigned int rws);
	
		void feedBitmap(ofColor * map, unsigned int chanNum, unsigned int vol);

        ofColor * outputMixed();
        unsigned char* outputMixedChar();
		//ofImage matrixImg;
		
		void Blend(ofColor c1, ofColor c2, ofColor & dest );
        void setSaturation(unsigned int sat=255){
            saturation = sat;
        };

        void setBrightness(unsigned int brt=255){
            brightness = brt;
        };

		ofColor * outBitmap;
		ofColor * outBitmap1;
		ofColor * outBitmap2;
		unsigned char * outBitmapChar;
		unsigned int bitmapLength;
		unsigned int columns;
		unsigned int rows;
		unsigned int channelsCount;
        unsigned int saturation;
        unsigned int brightness;

		struct Channel {
			unsigned int volume;
			unsigned int channelNum;
			ofColor * bitmap;
		} * channel;
    
        ofFbo fbo;
        ofPixels pixs;
};

#endif