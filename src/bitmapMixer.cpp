/*
 *  bitmapMixer.cpp
 *  sequenceLed
 *
 *  Created by tim tavli on 10/09/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "bitmapMixer.h"

bitmapMixer::bitmapMixer(){;}

bitmapMixer::~bitmapMixer()
{
	for (int i =0; i<channelsCount; i++) {
		delete [] channel[i].bitmap;
	};
	delete [] channel;

	delete [] outBitmap;
	delete [] outBitmap1;
	delete [] outBitmap2;

	delete [] outBitmapChar;
}

bitmapMixer::bitmapMixer(unsigned int chanCount, unsigned int bitmapLen)
{
	saturation = 255;
}

void bitmapMixer::setup(unsigned int chanCount, unsigned int cls, unsigned int rws)
{
	channelsCount = chanCount;
	channel = new Channel[channelsCount];
	columns = cls;
	rows = rws;
	bitmapLength = columns*rows;
	for (int i = 0; i < channelsCount; i++) {
		channel[i].bitmap = new ofColor [bitmapLength];
	}

	outBitmap = new ofColor [bitmapLength];
	outBitmap1 = new ofColor [bitmapLength];
	outBitmap2 = new ofColor [bitmapLength];

	outBitmapChar = new unsigned char [bitmapLength*3];
//	matrixImg.allocate(columns, rows, OF_IMAGE_COLOR);
//    matrixImg.setPosition(0, 0);
}


void bitmapMixer::feedBitmap(ofColor * map, unsigned int chanNum, unsigned int vol)
{
	channel[chanNum].volume = vol;
	channel[chanNum].channelNum = chanNum;
	memcpy(channel[chanNum].bitmap, map, bitmapLength * sizeof(ofColor));

//	if (chanNum == 0) {
//		printf("channel 0:");
//		for (int i=0; i<bitmapLength; i++) {
//			printf(" %02X ", channel[chanNum].bitmap[i].getHex());
//		}
//		printf("\n");
//		printf("VOLUME 0 = %i \n", channel[chanNum].volume);
//	}
}

ofColor * bitmapMixer::outputMixed()
{
//	matrixImg.clear();
//	matrixImg.update();
	
	ofEnableAlphaBlending();

//	ofSetColor(255, 255, 255, 255);
//	ofSetColor(0, 0, 0, 120);
//	ofRect(0, 0, columns, rows);	
	
//	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	//glBlendFunc(GL_DST_COLOR, GL_ONE);
	//glBlendFunc(GL_DST_COLOR,GL_DST_COLOR);
//printf("_________\n");
//printf("CREATED OUT\n");

		unsigned char hexCollector;

		for (int j=0; j<channelsCount-1; j+=2) {

			for (int i = 0; i < bitmapLength; i++)
			{
				// Set Volume with Alpha
				channel[j].bitmap[i].setHex(channel[j].bitmap[i].getHex(), channel[j].volume);
				channel[j+1].bitmap[i].setHex(channel[j+1].bitmap[i].getHex(), channel[j+1].volume);
				
				(j < 2?
					Blend(channel[j].bitmap[i], channel[j+1].bitmap[i], outBitmap1[i]) :
				 	Blend(channel[j].bitmap[i], channel[j+1].bitmap[i], outBitmap2[i])
				 );
				
				//printf("LED%d:%X \n", i,outBitmap[i].getHex());
			}

		}
	for (int i = 0; i < bitmapLength; i++)
		Blend(outBitmap1[i], outBitmap2[i], outBitmap[i]), outBitmap[i].setSaturation(saturation);

//printf("---------\n\n");

	ofDisableAlphaBlending();
	//memcpy(outBitmapChar, matrixImg.getPixels(), bitmapLength*3);

	return outBitmap;
}

void bitmapMixer::Blend(ofColor c1, ofColor c2,  ofColor & dest)
{
    double a1 = (double)c1.a / 255.0;
    double a2 = (double)c2.a / 255.0;
	if (c1.getHex() == 0) a1 = 0.0;
	if (c2.getHex() == 0) a2 = 0.0;
	
    int R = (int) (a1 * c1.r + a2 * (1 - a1) * c2.r);
    int G = (int) (a1 * c1.g + a2 * (1 - a1) * c2.g);
    int B = (int) (a1 * c1.b + a2 * (1 - a1) * c2.b);
    int A = (int) (255 * (a1 + a2 * (1 - a1)));
	dest.set(R, G, B, A);
}

//int counter=0;
//	for (int i = 0; i < bitmapLength; i++)
//	{
//		printf("LED%d: R=%02X \n", i, outBitmapChar[counter++]);
//		printf("LED%d: G=%02X \n", i, outBitmapChar[counter++]);
//		printf("LED%d: B=%02X \n", i, outBitmapChar[counter++]);
//		//printf("LED%d: A=%02X \n", i, outBitmapChar[counter++]);
//	}