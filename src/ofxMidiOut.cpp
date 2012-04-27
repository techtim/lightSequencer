/*
 *      ofxMidiOut.cpp
 *
 *      v 0.02 July 2009
 *      Arturo & Chris O'Shea
 */

#include "ofxMidiOut.h"


// --------------------------------------------------------------------------------------
ofxMidiOut::ofxMidiOut() {
	//eventManager = new ofxMidiEventManager(this);
	//newMessageEvent.init("ofxMidiIn::newMessage");
	//ofEvents::addEvent(newMessageEvent,"ofxMidiIn::newMessage");
	// Check available ports.
	nPorts = midiout.getPortCount();
	portNames.clear();
}
// --------------------------------------------------------------------------------------
ofxMidiOut::~ofxMidiOut() {
    closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::listPorts(){
	printf( "ofxMidiOut: %i ports available \n", nPorts );
	for(unsigned int i=0; i<nPorts; i++){
		printf("%i: %s\n",i,midiout.getPortName(i).c_str());
		portNames.push_back( midiout.getPortName(i) );
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::openPort(unsigned int _port){
	if ( nPorts == 0 ) {
		printf( "No ports available!\n" );
		return;
	}
	if ( _port+1 > nPorts ){
		printf("The selected port is not available\n");
		return;
	}

	port = _port;
	midiout.openPort( port );
}

void ofxMidiOut::openPortByName(string portName){
    openPort(getPortByName(portName));
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::closePort(){
    midiout.closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::setVerbose(bool verbose){
	bVerbose=verbose;
}
// --------------------------------------------------------------------------------------
unsigned int ofxMidiOut::getPort(){
	return port;
}
unsigned int ofxMidiOut::getPortByName(string portName){
    for(unsigned int i=0; i<nPorts; i++){
        size_t found = midiout.getPortName(i).find(portName);
        if (found != string::npos) return i;
    }
    printf("MIDI OUT Port with name: \"%s\" not found", portName.c_str());
    return 100;
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendMessage(){
    midiout.sendMessage( &message );
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOn(int channel, int id, int value){
    message.clear();
    message.push_back( MIDI_NOTE_ON+(channel-1) );
    message.push_back( id );
    message.push_back( value );
    sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOff(int channel, int id, int value){
    message.clear();
    message.push_back( MIDI_NOTE_OFF+(channel-1) );
    message.push_back( id );
    message.push_back( value );
    sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendControlChange(int channel, int id, int value){
    message.clear();
    message.push_back( MIDI_CONTROL_CHANGE+(channel-1) );
    message.push_back( id );
    message.push_back( value );
    sendMessage();
}
