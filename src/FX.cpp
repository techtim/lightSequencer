//
//  FX.cpp
//  lightSequencer
//
//  Created by Тимофей Тавлинцев on 19.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "FX.h"

FX::FX() {
    setupFinished = false;
    moveStep    = 0;
    isEvenStep  = false;
    numEffects  = 4;
    bRetriger = true; // to trigger origin bitmap

    columns = 0;
    rows = 0;
    length = 0;
    
    gui = new ofxUISuperCanvas("fx", 0, 0, 0, height);
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->setColorBack(ofColor(0,0,0,0));
    gui->setFontSize(OFX_UI_FONT_SMALL, 5);
    
    invertButton = new ofxUILabelToggle("inv", false, 0,0,0,0, OFX_UI_FONT_SMALL);
    gui->addWidgetRight(invertButton);
    mirrorButton = new ofxUILabelToggle("mirr", false, 0,0,0,0, OFX_UI_FONT_SMALL);
    gui->addWidgetRight(mirrorButton);
    
    moveLeftButton = new ofxUILabelToggle("<", false, 0,0,0,0, OFX_UI_FONT_SMALL);
    gui->addWidgetRight(moveLeftButton);
    moveRightButton = new ofxUILabelToggle(">", false, 0,0,0,0, OFX_UI_FONT_SMALL);
    gui->addWidgetRight(moveRightButton);
    
    moveUpButton = new ofxUILabelToggle("^", false, 0,0,0,0, OFX_UI_FONT_SMALL);
    gui->addWidgetRight(moveUpButton);
    
    gui->getCanvasTitle()->setVisible(false);
    gui->autoSizeToFitWidgets();
    
    gui->disable();
    gui->setVisible(false);
}

FX::~FX() {
    LEDS::clear();
}

void FX::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int col, unsigned int row) {
    xLeft = x, yLeft = y, width = wid, height = hei, columns = col, rows = row, length = col*row;
    xRight = xLeft+width; yRight = yLeft+height;
    ledsNumber = col*row;
    
    LEDS::set(columns, rows, 1, xLeft, yLeft, 1, true);

    gui->setPosition(xLeft+(width-gui->getRect()->width)/2, yLeft);
    //    gui->setGlobalSliderHeight(<#float _globalSliderHeight#>)
    gui->enable();
    gui->setVisible(true);
    
    yRight = gui->getRect()->getY()+gui->getRect()->height;
    setupFinished = true;
    ofAddListener(gui->newGUIEvent, this, &FX::guiEvent);
}

void FX::process() {
    //    isEvenStep != isEvenStep;
    moveLeft();
    moveRight();
    moveUp();
    invert();
    mirror();
    bRetriger = false;
}

void FX::draw() {
    gui->draw();
}

bool FX::isClicked(int x, int y) {
    //    if (!LEDS::isClicked(x, y)) return false;
    //    mirrorButton->isClicked(x, y);
    //    invertButton->isClicked(x, y);
    //    moveLeftButton->isClicked(x, y);
    isOn = (invertButton->getValue() || mirrorButton->getValue() || moveLeftButton->getValue()) ? true : false;
    return true;
}

void FX::guiEvent(ofxUIEventArgs &e){
    
    if (e.widget == invertButton) {
        
    }
}

void FX::invert() {
    if (!setupFinished || !invertButton->getValue()) return;
    //        if (!isEvenStep) return;
    //        printf("IVERT");
    for (int i=0; i<length; i++) {
        leds[i].color = leds[i].color.getHex() == 0 ? color : tmpColor.black;
        leds[i].isSelected = !leds[i].isSelected;
    }
}

void FX::mirror() {
    if (!setupFinished || !mirrorButton->getValue()) return;
    //        if (!isEvenStep) return;
    
    for (int i=0; i<length/2; i++) {
        tmpColor = leds[i].color;
        tmpSelected = leds[i].isSelected;
        leds[i].isSelected = leds[length-i-1].isSelected;
        leds[i].color = leds[length-i-1].color;
        leds[length-i-1].color = tmpColor;
        leds[length-i-1].isSelected = tmpSelected;
        //            printf("COLOR:%X, SELECT: %i \n", leds[i].color.getHex(), (leds[i].isSelected?1:0) );
        //            }
    }
}

void FX::moveLeft() {
    if (!setupFinished || !moveLeftButton->getValue()) return;
    
    if (!bRetriger)
        for (int i=0; i<length; i+=columns) {
            LED tmp = leds[i];
            for (int j=0; j<columns-1; j++) {
                leds[i+j] = leds[i+j+1];
            }
            leds[i+columns-1] = tmp;
        }
    //        }
    moveStep++;
    if (moveStep>=columns) moveStep=0;
}

void FX::moveRight() {
    if (!setupFinished || !moveRightButton->getValue()) return;

    if (!bRetriger)
        for (int row=0;row<=rows; row++) {
            LED tmp = leds[row*columns-1];
            for (int j=1; j<columns; j++) {
                leds[row*columns-j] = leds[row*columns-j-1];
            }
            leds[(row-1)*columns] = tmp;
        }
    
    moveStep++;
    if (moveStep>columns) moveStep=0;
}

void FX::moveUp() {
    if (!setupFinished || !moveUpButton->getValue()) return;

    if (!bRetriger)
        for (int i=0; i<columns; i++) {

            LED tmp = leds[i];
            for (int row=0; row<rows-1; row++) {
                leds[i+columns*row]=leds[i+columns*(row+1)];
            }
            leds[i+columns*(rows-1)]=tmp;
        }
  
    moveStep++;
    if (moveStep>rows) moveStep=0;
}

void FX::moveDown() {
    if (!setupFinished || !moveUpButton->getValue()) return;
    
    if (!bRetriger)
        for (int i=0; i<columns; i++) {
            
            LED tmp = leds[i];
            for (int row=0; row<rows-1; row++) {
                leds[i+columns*row]=leds[i+columns*(row+1)];
            }
            leds[i+columns*(rows-1)]=tmp;
        }
    
    moveStep++;
    if (moveStep>rows) moveStep=0;
}
