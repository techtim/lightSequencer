//
//  tcpControl.cpp
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 10.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//
#include <iostream>
#include "tcpControl.h"

void tcpControl::setup() {
    host = "192.168.1.1";
//    host = "127.0.0.1";
    port = 5050;
    weConnected = tcpClient.setup(host, port);
    //	weConnected = tcpClient.setup("127.0.0.1", 5050);
    //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
    tcpClient.setMessageDelimiter("\n");

    connectTime = 0;
    deltaTime = 0;

    tcpClient.setVerbose(true);

}

bool tcpControl::write(unsigned char pix, bool is_Hex) {
    if (!tcpClient.isConnected()) {
        weConnected = false; 
        update();
    }
//    msgTx = ofToString(pix);
//msgTx = hexString(pix);
    if (is_Hex) {
        msgTx = hexString(pix);
    } else {
        msgTx = 'R';
    }
//    printf("%s",msgTx.c_str());
//    unsigned char * tmpByte = new unsigned char[1];
//	tmpByte[0] = pix;
//    pix
//    tcpClient.sendRawBytes((const char *)pix, 1);
    if (tcpClient.isConnected()){ // 
//        tcpClient.sendRawBytes((const char *)tmpByte, 1);
        tcpClient.send(msgTx);
        std::cout<< msgTx<<"\n";
//        std::cout << msgTx<<"\n";
        return true;
    } else if (!tcpClient.isConnected()){
        weConnected = false;
    }
    return false;
}
bool tcpControl::writeRaw(unsigned char * pixs, const int len) {
    tcpClient.sendRawBytes((const char*)pixs, len);
    return true;
}
void tcpControl::update() {
    
	//we are connected - lets send our text and check what we get back
	if (!weConnected){
        printf("TCP TRY TO CONNECT %s : %d \n", host.c_str(), port);
		//if we are not connected lets try and reconnect every 5 seconds
		deltaTime = ofGetElapsedTimeMillis() - connectTime;
        
		if( deltaTime > 3000){
			weConnected = tcpClient.setup(host, port);
            tcpClient.setMessageDelimiter("\n");
			connectTime = ofGetElapsedTimeMillis();
		}
        
	}
}

