//
//  ledPresets.cpp
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 07.03.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ledPresets.h"

ledPresets::ledPresets() {
    ;
}

ledPresets::~ledPresets() {
//    saveXML();
    for (int i=0; i<presetsNum; i++) {
        for (int j=0; j<4; j++) {
            delete [] presets[i].generators[j].matrix;
            delete [] presets[i].generators[j].sequence;
        }
    }
    
    delete [] presets;
}

void ledPresets::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int matrW, unsigned int matrH, unsigned int presNum) {
    leftX = x, leftY = y, width = wid, height = hei;
    presetsNum = presNum; ledsNum = matrW*matrH;
    ledsW = matrW, ledsH = matrH;
    presets = new Preset [presetsNum];
    
    for (int i=0; i<presetsNum; i++) {
        for (int j=0; j<4; j++) {
            presets[i].generators[j].matrix = new unsigned char [ledsNum*3];
            presets[i].generators[j].sequence = new unsigned char [16*3];
            presets[i].is_active = false;
        }
    }
        true;
//LEDS::set(<#int col#>, <#int row#>, <#int cell#>, <#int x#>, <#int y#>, <#int spac#>)
    LEDS::set(8, presetsNum/8, width/8-2, x, y, 4);
    
    Record.setup(leftX+width/2-width/8*2, leftY+presetsNum/8*width/8+4, width/8*4, width/8, true);
    Record.setActictiveText("REC ON");
    Record.setInactictiveText("REC OFF");
    SaveXML.setup(leftX+width/2-width/8*2, leftY+presetsNum/8*width/8+35, width/8*4, width/8, false);
    SaveXML.setActictiveText("SAVE XML");
    LoadXML.setup(leftX+width/2-width/8*2, leftY+presetsNum/8*width/8+80, width/8*4, width/8, false);
    LoadXML.setActictiveText("LOAD XML");
//    Record.setupMidi(41, 1, 0);
//    Record.setMidiActive(true);
    xmlName = "ledPresets-"+ofToString(matrW)+"x"+ofToString(matrH)+".xml";

//        loadXML();
//    } else {
//        XML.saveFile(xmlName);
//        message = xmlName+" created!";
//    }
        
}

void ledPresets::save(int presNum, unsigned int genNum, Generator & gen) {
    if (genNum>3) {printf("WRONG GENER NUM"); return;}
    if (presNum<0 || presNum>presetsNum) {printf("WRONG PRES NUM"); return;}
    printf("QUANT = %i\n", gen.Sequencer.quant);
    presets[presNum].generators[genNum].quant = gen.Sequencer.quant;
    memcpy(presets[presNum].generators[genNum].matrix, gen.ledMatrix.getBitmapChar(), ledsNum*3);
    unsigned char * map = presets[presNum].generators[genNum].matrix;
    for (int i=0; i<ledsNum*3; i++) printf("%02X",map[i]);
    printf("\n");
    
    memcpy(presets[presNum].generators[genNum].sequence, gen.Sequencer.getBitmapChar(), 16*3);
    presets[presNum].generators[genNum].att     = gen.Sequencer.attack;
    presets[presNum].generators[genNum].dec     = gen.Sequencer.decay;
    presets[presNum].generators[genNum].rel     = gen.Sequencer.release;
    
    printf("%f , %f , %f", presets[presNum].generators[genNum].att, 
    presets[presNum].generators[genNum].dec,
           presets[presNum].generators[genNum].rel);
    
    presets[presNum].is_active = true;
//    saveXML();
}

void ledPresets::load(int presNum, unsigned int genNum, Generator & gen) {
    if (genNum>3) {printf("WRONG GENER NUM"); return;}
    if (presNum<0 || presNum>presetsNum) {printf("WRONG PRES NUM"); return;}
    if (presets[presNum].is_active == false) {printf("EMPTY PRES NUM");return;}

    presets[presNum].generators[genNum].quant == 1 ? gen.Sequencer.setQuant16() : gen.Sequencer.setQuant8();

    unsigned char * map = presets[presNum].generators[genNum].matrix;
    for (int i=0; i<ledsNum*3; i++) printf("%02X",map[i]);
    printf("\n");
    gen.ledMatrix.parseBitmap(presets[presNum].generators[genNum].matrix);
    gen.Sequencer.parseBitmap(presets[presNum].generators[genNum].sequence);
    gen.Sequencer.setADSR(presets[presNum].generators[genNum].att, presets[presNum].generators[genNum].dec, presets[presNum].generators[genNum].rel);
}

void ledPresets::draw() {
    LEDS::print();
    Record.draw();
    SaveXML.draw();
    LoadXML.draw();
    printf("%s", message.c_str());
    message = "";
}


bool ledPresets::isClicked(int x, int y, bool dragged)
{
	bool isclicked = false;
//	for (int i = 0; i < 3; i++)

	if (dragged) return isclicked;
    
	if (Record.isClicked(x, y)) { isRec = Record.isOn; isLedsClicked = false; return true; }
    if (LEDS::isClicked(x, y)) {
        isLedsClicked = true;
//        LEDS::setClicked(x, y, isRec ? color.red : color.black);
        ledLastClicked = LEDS::numClicked(x, y, isRec ? color.red : color.yellow); 
        // get last clicked only after setClicked
        leds[ledLastClicked].color = isRec ? color.red :
                                            (presets[ledLastClicked].is_active ? color.yellow : color.black);
        printf("PRESET LED CLIK %i\n", ledLastClicked);
        
        LEDS::setClickedAll();
        return true;
    }
    isLedsClicked = false;
    
    if (SaveXML.isClicked(x, y)) { saveXML(); }
    if (LoadXML.isClicked(x, y)) { loadXML(); }
    return false;        
}

bool ledPresets::loadXML() {
    ofxXmlSettings XML;
    XML.loadFile(xmlName);
    message = xmlName+" loaded!";
//    XML.pushTag("PRESETS");
    int numGenTags = XML.getNumTags("GENERATOR");
    int ct = 0;
    for (int i=0; i<numGenTags/4; i++){
//        XML.pushTag("GENERATORS");
//        
        for (int j=0; j<4; j++){
            XML.pushTag("GENERATOR", ct++);
            presets[i].generators[j].quant = XML.getValue("quant", 1);
            presets[i].generators[j].att = XML.getValue("att", 0);
            presets[i].generators[j].dec = XML.getValue("dec", 0);
            presets[i].generators[j].rel = XML.getValue("rel", 0);
            presets[i].is_active = leds[i].isSelected = XML.getValue("isActive", 0) ? true : false;
            if (leds[i].isSelected) leds[i].color = color.yellow;
            XML.popTag();
        }
        XML.popTag();
    }
//    LEDS::setClickedAll();
//    XML.popTag();
    return true;
}

bool ledPresets::saveXML() {
    printf("SAVING XML: %s", xmlName.c_str() );
//    XML.addTag("PRESETS");
//    XML.pushTag("PRESETS");
    unsigned int ct = 0;
    ofxXmlSettings TMP;
    
    for (int i=0; i<presetsNum; i++){
//        XML.addTag("PRESET");
//        XML.pushTag("PRESET", i);
//        XML.addTag("GENERATORS");
//        XML.pushTag("GENERATORS");

        for (int j=0; j<4; j++){
            int lastTagNumber = TMP.addTag("GENERATOR");
//            XML.pushTag("GENERATOR", lastTagNumber);
            if (presets[i].is_active) {
                TMP.setValue("GENERATOR:att", presets[i].generators[j].att, lastTagNumber);
                TMP.setValue("GENERATOR:dec", presets[i].generators[j].dec, lastTagNumber);
                TMP.setValue("GENERATOR:rel", presets[i].generators[j].rel, lastTagNumber);
                TMP.setValue("GENERATOR:quant", (int)presets[i].generators[j].quant, lastTagNumber);
                TMP.setValue("GENERATOR:isActive", 1, lastTagNumber);
                string strmat((char *) presets[i].generators[j].matrix, ledsW*ledsH*3);
                printf("strmat = %s\n", strmat.c_str());
                TMP.setValue("GENERATOR:bitmap", strmat, lastTagNumber);
//                TMP.setValue(
                string strseq = hexString(presets[i].generators[j].sequence, 16*3);
                printf("\nSEQ = ");
                for (int z=0; z<16*3; z++) printf("%02X", presets[i].generators[j].sequence[i]);
                printf("\nstrseq = %s\n", strseq.c_str());
                TMP.setValue("GENERATOR:seq", strseq, lastTagNumber);
            } else {
                TMP.setValue("GENERATOR:att", 0, lastTagNumber);
                TMP.setValue("GENERATOR:dec", 0, lastTagNumber);
                TMP.setValue("GENERATOR:rel", 0, lastTagNumber);
                TMP.setValue("GENERATOR:quant", 0, lastTagNumber);
            }
            printf("GEN %d\n", lastTagNumber);
            ct++;
            TMP.popTag();
        }
//        XML.popTag();
//        XML.popTag();
//        XML.popTag();  
    }
//    XML.popTag();
    
    TMP.saveFile(xmlName);
    
    return true;
}
