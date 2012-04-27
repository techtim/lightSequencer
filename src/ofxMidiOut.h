/*
 *      ofxMidiOut.h
 *
 *      v 0.02 July 2009
 *      Arturo & Chris O'Shea
 */

#ifndef OFX_MIDI_OUT_H_
#define OFX_MIDI_OUT_H_

#include <iostream>
#include "RtMidi.h"

#include "ofxMidiConstants.h"
#include "ofxMidiEvents.h"

#include "ofMain.h"

class ofxMidiOut{
public:
	ofxMidiOut();
	virtual ~ofxMidiOut();

	void            listPorts();
	void            openPort(unsigned int port=0);
    void            openPortByName(string portName);
	void            closePort();
	unsigned int    getPort();
    unsigned int    getPortByName(string portName);
	void            setVerbose(bool verbose = false);

	void            sendNoteOn(int channel, int id, int value);
	void            sendNoteOff(int channel, int id, int value);
	void            sendControlChange(int channel, int id, int value);

	vector <string> portNames;


protected:

    void            sendMessage();

	RtMidiOut 			        midiout;
	unsigned int 		        nPorts;
	unsigned int 		        port;
	bool				        bVerbose;
	std::vector<unsigned char>  message;

};

#endif
