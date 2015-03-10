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
    //    for (int i=0; i<presetsNum; i++) {
    //        for (int j=0; j<4; j++) {
    //            delete [] presets[i].generators[j].matrix;
    //            delete [] presets[i].generators[j].sequence;
    //        }
    //    }
    
    delete [] presets;
}

void ledPresets::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int matrW, unsigned int matrH, unsigned int presNum) {
    leftX = x, leftY = y, width = wid, height = hei;
    presetsNum = presNum; ledsNum = matrW*matrH;
    ledsW = matrW, ledsH = matrH;
    presets = new Preset [presetsNum];
    
    for (int i=0; i<presetsNum; i++) {
        for (int j=0; j<4; j++) {
            //            presets[i].generators[j].matrix = new unsigned char [ledsNum*3];
            //            presets[i].generators[j].sequence = new unsigned char [16*3];
            presets[i].generators[j].matrix = "";
            presets[i].generators[j].sequence = "";
            presets[i].generators[j].fxInv = 0;
            presets[i].generators[j].fxMir = 0;
            presets[i].is_active = false;
        }
    }
    true;
    //LEDS::set(<#int col#>, <#int row#>, <#int cell#>, <#int x#>, <#int y#>, <#int spac#>)
    unsigned int cellSize = (height-4*8)/(presetsNum/8*1.5);

    LEDS::set(8, presetsNum/8, cellSize, leftX+width/2-(cellSize+4)*8/2, y, 4);
    LEDS::setupMidi(10, 2, 1, 1);
    Record.setup(leftX+width/2-width/8*2, LEDS::yRight+5, width/8*4, 15, true);
    Record.setActictiveText("rec on");
    Record.setInactictiveText("rec off");
    SaveXML.setup(leftX+width/2-width/8*2, LEDS::yRight+25, width/8*4, 15, false);
    SaveXML.setActictiveText("save");
    LoadXML.setup(leftX+width/2-width/8*2, LEDS::yRight+45, width/8*4, 15, false);
    LoadXML.setActictiveText("load");
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
    if (genNum>3) {printf("WRONG GENER NUM: %i \n",genNum); return;}
    if (presNum<0 || presNum>=presetsNum) {printf("WRONG PRES NUM: %i",presNum); return;}
    printf("QUANT = %i\n", gen.Sequencer.quant);
    presets[presNum].generators[genNum].quant = gen.Sequencer.quant;
    
    presets[presNum].generators[genNum].colorPos = gen.hueLine.colorPos;
    //    memcpy(presets[presNum].generators[genNum].matrix, gen.ledMatrix.getBitmapChar(), ledsNum*3);
    //    unsigned char * map = presets[presNum].generators[genNum].matrix;
    //    for (int i=0; i<ledsNum*3; i++) printf("%02X",map[i]);
    //    printf("\n");
    gen.ledMatrix.getPixelsState(presets[presNum].generators[genNum].matrix);
    printf("SAVE MATR: %s",presets[presNum].generators[genNum].matrix.c_str());
    gen.Sequencer.getPixelsState(presets[presNum].generators[genNum].sequence);
    printf("SAVE SEQ: %s",presets[presNum].generators[genNum].sequence.c_str());
    //    memcpy(presets[presNum].generators[genNum].sequence, gen.Sequencer.getBitmapChar(), 16*3);
    presets[presNum].generators[genNum].att     = gen.Sequencer.attack;
    presets[presNum].generators[genNum].dec     = gen.Sequencer.decay;
    presets[presNum].generators[genNum].rel     = gen.Sequencer.release;
    presets[presNum].generators[genNum].fxInv   = gen.effects.invertButton->getValue() ? 1 : 0;
    presets[presNum].generators[genNum].fxMir   = gen.effects.mirrorButton->getValue() ? 1 : 0;
    presets[presNum].generators[genNum].fxMoveLeft   = gen.effects.moveLeftButton->getValue() ? 1 : 0;
    presets[presNum].generators[genNum].fxMoveRight   = gen.effects.moveRightButton->getValue() ? 1 : 0;
    
    presets[presNum].generators[genNum].psyOn   = gen.hueLine.psyButton->getValue() ? 1 : 0;
    presets[presNum].generators[genNum].psySpeed= gen.hueLine.hueControl->getValue();
    
//    presets[presNum].generators[genNum].shapeType = gen.effects.getShapeType();

    
    printf("%f , %f , %f, %d, %d",
           presets[presNum].generators[genNum].att,
           presets[presNum].generators[genNum].dec,
           presets[presNum].generators[genNum].rel,
           presets[presNum].generators[genNum].fxInv,
           presets[presNum].generators[genNum].fxMir
           );
    
    presets[presNum].is_active = true;
    //    saveXML();
}

void ledPresets::load(int presNum, unsigned int genNum, Generator & gen, bool bLoadColor) {
    if (genNum>3) {printf("WRONG GENER NUM\n"); return;}
    if (presNum<0 || presNum>=presetsNum) {printf("WRONG PRES NUM\n"); return;}
    if (presets[presNum].is_active == false) {printf("EMPTY PRES NUM\n");return;}
    
    switch (presets[presNum].generators[genNum].quant){
        case 1:
            gen.Sequencer.setQuant32();
            break;
        case 2:
            gen.Sequencer.setQuant16();
            break;
        case 4:
            gen.Sequencer.setQuant8();
            break;
    }
    
    //    unsigned char * map = presets[presNum].generators[genNum].matrix;
    //    for (int i=0; i<ledsNum*3; i++) printf("%02X",map[i]);
    //    printf("\n");
    
    gen.ledMatrix.setPixelsState(presets[presNum].generators[genNum].matrix);
    gen.effects.setPixelsState(presets[presNum].generators[genNum].matrix);
    gen.Sequencer.setPixelsState(presets[presNum].generators[genNum].sequence);
    gen.Sequencer.setADSR(presets[presNum].generators[genNum].att, presets[presNum].generators[genNum].dec, presets[presNum].generators[genNum].rel);

    //--- Commented for Ableton
    //    presets[presNum].generators[genNum].fxInv   = gen.effects.invertButton.isOn ? 1 : 0;
    ((ofxUIButton*)gen.effects.invertButton)->setValue(presets[presNum].generators[genNum].fxInv == 1 ? true : false);
    ((ofxUIButton*)gen.effects.mirrorButton)->setValue(presets[presNum].generators[genNum].fxMir == 1 ? true : false);
    ((ofxUIButton*)gen.effects.moveRightButton)->setValue(presets[presNum].generators[genNum].fxMoveRight == 1 ? true : false);
    ((ofxUIButton*)gen.effects.moveLeftButton)->setValue(presets[presNum].generators[genNum].fxMoveLeft == 1 ? true : false);
    
    gen.effects.retrigger();
//    gen.effects.setShapeType(presets[presNum].generators[genNum].shapeType);
    
    printf("INV => %d ; MIR => %d", presets[presNum].generators[genNum].fxMir, presets[presNum].generators[genNum].fxInv);

    if (bLoadColor) {
        gen.hueLine.colorPos = presets[presNum].generators[genNum].colorPos;
        ((ofxUIButton*)gen.hueLine.psyButton)->setValue(presets[presNum].generators[genNum].psyOn ? true : false);
        ((ofxUIButton*)gen.hueLine.hueControl)->setValue(presets[presNum].generators[genNum].psySpeed);
    }
    
    gen.getGui()->enable();
    gen.getGui()->setAutoDraw(true);
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
    
    //	if (dragged) return isclicked;
    
	if (Record.isClicked(x, y)) { isRec = Record.isOn; isLedsClicked = false; return true; }
    if (LEDS::isClicked(x, y)) {
        isLedsClicked = true;
        //        LEDS::setClicked(x, y, isRec ? color.red : color.black);
        ledLastClicked = LEDS::numClicked(x, y, isRec ? color.red : color.yellow);
        // get last clicked only after setClicked
        //        updateColor(color.yellow, color.black);
        for (int i=0; i < presetsNum; i++) leds[i].color = presets[i].is_active ? color.yellow : color.black;
        if (ledLastClicked > -1 && ledLastClicked < ledsH*ledsW)
            leds[ledLastClicked].color = isRec ? color.red :
                (presets[ledLastClicked].is_active ? color.green : color.black);
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
            presets[i].generators[j].fxInv = XML.getValue("fxInv", 0);
            presets[i].generators[j].fxMir = XML.getValue("fxMir", 0);
            presets[i].generators[j].fxMoveRight = XML.getValue("fxMoveRight", 0);
            presets[i].generators[j].fxMoveLeft = XML.getValue("fxMoveLeft", 0);
            presets[i].generators[j].shapeType = XML.getValue("shapeType", 0);
            presets[i].generators[j].colorPos = XML.getValue("colorPos", 0);
            
            presets[i].is_active = leds[i].isSelected = XML.getValue("isActive", 0) ? true : false;
            if (presets[i].is_active) {
                presets[i].generators[j].matrix = XML.getValue("bitmap", "");
                presets[i].generators[j].sequence = XML.getValue("seq", "");
                //                strcpy(p2,tmpStr.c_str());
                //                stringHex(presets[i].generators[j].matrix, (char*)tmpStr.c_str(), tmpStr.length());
                
                
                //                stringHex(presets[i].generators[j].sequence, (char*)tmpStr.c_str(), tmpStr.length());
            }
            if (leds[i].isSelected) leds[i].color = color.yellow;
            
            presets[i].generators[j].psyOn = XML.getValue("psyOn", 0);
            presets[i].generators[j].psySpeed = XML.getValue("psySpeed", 0);
            
            XML.popTag();
        }
        XML.popTag();
    }
    //    LEDS::setClickedAll();
    //    XML.popTag();
    
    chainState = XML.getValue("CHAINSTATE", "0");
    printf("\nCHAINSTATE: %s",  chainState.c_str());
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
                TMP.setValue("GENERATOR:fxInv", (int)presets[i].generators[j].fxInv, lastTagNumber);
                TMP.setValue("GENERATOR:fxMir", (int)presets[i].generators[j].fxMir, lastTagNumber);
                TMP.setValue("GENERATOR:fxMoveRight", (int)presets[i].generators[j].fxMoveRight, lastTagNumber);
                TMP.setValue("GENERATOR:fxMoveLeft", (int)presets[i].generators[j].fxMoveLeft, lastTagNumber);
                TMP.setValue("GENERATOR:shapeType", (int)presets[i].generators[j].shapeType, lastTagNumber);
                TMP.setValue("GENERATOR:colorPos", (int)presets[i].generators[j].colorPos, lastTagNumber);
                
                
                TMP.setValue("GENERATOR:isActive", 1, lastTagNumber);
                //                string strmat = hexString(presets[i].generators[j].matrix, ledsW*ledsH*3);
                printf("strmat = %s\n", presets[i].generators[j].matrix.c_str());
                TMP.setValue("GENERATOR:bitmap", presets[i].generators[j].matrix, lastTagNumber);
                
                //                string strseq = hexString(presets[i].generators[j].sequence, 16*3);
                //                printf("\nSEQ = ");
                //                for (int z=0; z<16*3; z++) printf("%02X", presets[i].generators[j].sequence[i]);
                printf("\nstrseq = %s\n", presets[i].generators[j].sequence.c_str());
                
                TMP.setValue("GENERATOR:seq", presets[i].generators[j].sequence, lastTagNumber);
                TMP.setValue("GENERATOR:psyOn", (presets[i].generators[j].psyOn ? 1 : 0) ,lastTagNumber);
                TMP.setValue("GENERATOR:psySpeed", (int)presets[i].generators[j].psySpeed, lastTagNumber);
            } else {
                TMP.setValue("GENERATOR:att", 0, lastTagNumber);
                TMP.setValue("GENERATOR:dec", 0, lastTagNumber);
                TMP.setValue("GENERATOR:rel", 0, lastTagNumber);
                TMP.setValue("GENERATOR:quant", 0, lastTagNumber);
                TMP.setValue("GENERATOR:fxInv", 0, lastTagNumber);
                TMP.setValue("GENERATOR:fxMir", 0, lastTagNumber);
                TMP.setValue("GENERATOR:psyOn", 0 ,lastTagNumber);
                TMP.setValue("GENERATOR:fxMoveRigth", 0 ,lastTagNumber);
                TMP.setValue("GENERATOR:fxMoveLeft", 0 ,lastTagNumber);
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
    
    TMP.setValue("CHAINSTATE", chainState);
    
    TMP.saveFile(xmlName);
    
    return true;
}
