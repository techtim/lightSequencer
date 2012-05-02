//
//  ledPresets.h
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 07.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lightSequencer_ledPresets_h
#define lightSequencer_ledPresets_h

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxButton.h"
#include "LEDS.h"
#include "Generator.h"
#include "ofxXmlSettings.h"

class ledPresets : public LEDS {
    public:
        ledPresets();
        ~ledPresets();
        void setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int matrW, unsigned int matrH, unsigned int presNum=32);
    
        void setupMidi(unsigned int ident, unsigned int channel=1, unsigned int port = 0);
        
        void save(int presNum, unsigned int genNum, Generator & gen);
        void load(int presNum, unsigned int genNum, Generator & gen);
        void draw();

        bool isClicked(int x, int y, bool dragged=false);
        
        bool loadXML();
        bool saveXML();

        unsigned int leftX, leftY, width, height;
        unsigned int presetsNum, ledsNum;
        ofxButton Record;
        ofxButton SaveXML;
        ofxButton LoadXML;
    
        bool isRec;
        bool isLedsClicked;
        unsigned int selPresetNum;
        unsigned int ledsW, ledsH;
    
        // +++ XML +++
        ofxXmlSettings XML;
        ofTrueTypeFont TTF;
        string message;
        string xmlName;
        string tmpStr;
    
        struct xmlGenerator {
            unsigned char * matrix;
            unsigned char * sequence;
            unsigned int quant;
            float att;
            float dec;
            float rel;
        };
    
        struct Preset {
            xmlGenerator generators[4];
            bool is_active;
        } * presets;
    
        string hexString( unsigned char * byte, unsigned char n)
        {
            char lookup[] = "0123456789abcdef";
            string s = "";
            
            for( int i = 0; i < n; i++ ) {
                s += lookup[ byte[i] >> 4 ];
                s += lookup[ byte[i] & 0x0f ];
            }
            
            return s;
        }
    
        void stringHex(unsigned char* hexString, char* string, int stringLength ) 
        {     
            // Post:
            
            unsigned char uChar = 0;           
            unsigned char ct = 0;
            for ( int x = 0; x<stringLength; x+=2 )     
            {         
                sscanf(&string[x], "%02x", &uChar); 
                printf("%02x", uChar);
                hexString[ct++] = uChar;     
            } 
            
        } 
};
#endif
