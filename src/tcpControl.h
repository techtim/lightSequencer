//
//  tcpControl.h
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 10.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lightSequencer_tcpControl_h
#define lightSequencer_tcpControl_h

#include "ofMain.h"
#include "ofxNetwork.h"

class tcpControl {
public:
    tcpControl() {;}
    ~tcpControl() {;}
    
    void setup();
    void update();
    bool write(unsigned char pix, bool is_Hex = false);
    bool writeRaw(unsigned char * pixs, const int len);
        
    ofxTCPClient tcpClient;
    string msgTx, msgRx;

    float counter;
    int connectTime;
    int deltaTime;
    unsigned int port;
    string host;
    stringstream ss;

    bool weConnected;
    
    string hexString( unsigned char byte )
    {
        char lookup[] = "0123456789abcdef";
        string s = "";
        
        //    for( int i = 0; i < n; i++ ) {
        s += lookup[ byte >> 4 ];
        s += lookup[ byte & 0x0f ];
        //    }
        
        return s;
    }

};

struct HexCharStruct
{
    char c;
    HexCharStruct(char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
    return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(char _c)
{
    return HexCharStruct(_c);
}

#endif
