//
//  DmxInterface.h
//  lightSequencerDmx
//
//  Created by Tim TVL on 21/04/15.
//
//

#ifndef __lightSequencerDmx__DmxInterface__
#define __lightSequencerDmx__DmxInterface__

#include <stdio.h>
#include "ofxDmx.h"
#include "ofxDmxFtdi.h"
//#include "ofxArtnet.h"
#include "ofxUI.h"

enum DMX_CONTROLLER_TYPES {
    DMX_SERIAL = 0,
    DMX_FTDI = 1,
    DMX_ARTNET = 2,
};


class DmxInterface
{
    DmxInterface();
    ~DmxInterface();
    
    void setup();
    
    void draw();
    
    void setControllerType(DMX_CONTROLLER_TYPES);
    void setTargetIp(string targerIp);
    
  private:
    ofxUISuperCanvas *gui;

    ofxDmx dmx;
    ofxDmxFtdi dmxFtdi;
//    ofxArtnet artnet;
    
};


#endif /* defined(__lightSequencerDmx__DmxInterface__) */