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

}

FX::~FX() {
    delete [] leds;
	delete [] bitmap;
	delete [] bitmapChar;

}

void FX::setup(unsigned int x, unsigned int y, unsigned int wid, unsigned int hei, unsigned int col, unsigned int row) {
    xLeft = x, yLeft = y, width = wid, height = hei, columns = col, rows = row, length = col*row;
    xRight = xLeft+width; yRight = yLeft+height;

    LEDS::set(columns, rows, 1, xLeft, yLeft, 1, true);

    invertButton.setup(xLeft+20, yLeft, 80, height, true);
    invertButton.setActictiveText("invert");
    mirrorButton.setup(xLeft+110, yLeft, 80, height, true);
    mirrorButton.setActictiveText("mirror");

    setupFinished = true;
}

void FX::process() {
    invert();
    mirror();
}

void FX::draw() {
    invertButton.draw();
    mirrorButton.draw();
}

bool FX::isClicked(int x, int y) {
//    if (!LEDS::isClicked(x, y)) return false;
    mirrorButton.isClicked(x, y);
    invertButton.isClicked(x, y);
    isOn = (invertButton.isOn || mirrorButton.isOn) ? true : false;
    return true;
}