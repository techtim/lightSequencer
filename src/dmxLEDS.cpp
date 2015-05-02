//
//  dmxLEDS.cpp
//  lightSeqDmx
//
//  Created by Tim TVL on 24/09/14.
//
//

#include "dmxLEDS.h"


DmxLEDS::DmxLEDS()
{
    ;
};

DmxLEDS::~DmxLEDS()
{
//	delete [] leds;
    leds.clear();
	delete [] bitmap;
	delete [] bitmapChar;
}