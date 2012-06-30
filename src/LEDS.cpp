#include "LEDS.h"

LEDS::LEDS()
{
    ;
};

LEDS::~LEDS()
{
	delete [] leds;
	delete [] bitmap;
	delete [] bitmapChar;
}

void LEDS::set(int col, int row, int cell, int x, int y, int spac, bool setAllWhite)
{
    columns = col;
    rows = row;
    cellSize = cell;
	mSpace = spac;
    xLeft = x;
    yLeft = y;
    xRight = x + columns * cellSize + columns * mSpace;
    yRight = y + rows * cellSize + rows * mSpace;

	leds = new LED [columns * rows];
    ledLastClicked = -1; // mean no clicks made
	bitmap = new ofColor [columns * rows];
	bitmapChar = new unsigned char [columns * rows * 3];
	
	if (setAllWhite) {
		for (int i; i<columns*rows; i++) 
			leds[i].color.set(255.0f,255.0f,255.0f,255.0f);
	}
	matrixImg.allocate(columns, rows, OF_IMAGE_COLOR);
    //matrixImg.setPosition(xLeft, yLeft);

    midiActive = false;
    midiSeqStartCC = 10;

    changedBitmap = false;

    setPosition();
};

void LEDS::setPosition()
{
	int count = 0;
	int row = 0, cell = 0;

    for (int i = 0; i < columns * rows; i++)
	{
		int ledXLeft = xLeft + cell * mSpace + cell * cellSize;
		int ledYLeft = yLeft + row * mSpace + row * cellSize;
		leds[count].num = count;
		leds[count].numColumn = cell;
		leds[count].numRow = row;
		leds[count].position(ledXLeft, ledYLeft, ledXLeft + cellSize, ledYLeft + cellSize);
		//printf("%i PIXEL: XLeft = %i , YLeft = %i \n", count, ledXLeft, ledYLeft);
		count++;
		cell++;
		if (cell == columns) cell = 0, row++;
	}

}

void LEDS::parseBitmap(unsigned char * pixels)
{
	int count = 0;
	for (int i = 0; i < columns * rows * 3; i+=3)
	{	
        if (pixels[i] == '\0' && pixels[i+1] == '\0' && pixels[i+2] == '\0') {
            leds[count].isSelected = false;
        } else {
            leds[count].color.set((float)pixels[i],(float)pixels[i+1], (float)pixels[i+2]);
            leds[count].isSelected = true;
//       		printf("%i PIXEL: %i , %i , %i \n", count, pixels[i], pixels[i+1], pixels[i+2]);
        }
        count++;
	}
    changedBitmap = true;
//    printf("\n \n \n");
};

void LEDS::parseBitmap(ofColor * pixels)
{
	int count = 0;
	for (int i = 0; i < columns * rows; i++)
	{
		leds[count].color.set(pixels[i]);
        leds[count].isSelected = pixels[i].getHex() == 0 ? false : true;
        count++;
		//printf("%i PIXEL: %i , %i , %i - XLeft = %i , YLeft = %i \n", count, pixels[i], pixels[i+1], pixels[i+2]);
	}
	changedBitmap = true;
    //printf("\n \n \n \n");
};

void LEDS::parseBitmap()
{
    matrixPixels = matrixImg.getPixels();
    int count = 0;
	for (int i = 0; i < columns * rows * 3; i+=3)
	{
		leds[count++].color.set(matrixPixels[i], matrixPixels[i+1], matrixPixels[i+2]);
	}
    changedBitmap = true;
}

void LEDS::print()
{
    int x = xLeft;
    int y = yLeft;
    int counter = 0;
    
    if (midiActive) {
        ofSetColor(150, 150, 150);
        ofRect(x-mSpace, y-mSpace, (cellSize + mSpace) * columns+mSpace, (cellSize + mSpace) * rows+mSpace);
        
        for (int i=0; i < rows * columns; i++) {
            leds[i].isSelected ? 
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 127) :
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 0);
        }
        for (int i=rows * columns; i<16; i++) midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 0);
//        midiOut.sendControlChange(midiChannel, midiActivationCC, 127);
    } else {
//        midiOut.sendControlChange(midiChannel, midiActivationCC, 0); try control from app
        ;; 
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (leds[counter].isSelected == true)
            {
                ofSetColor(leds[counter].color.r, leds[counter].color.g, leds[counter].color.b);
            }
            else
            {
                ofSetColor(0,0,0);
            }

            ofRect(x, y, cellSize, cellSize);
            x += cellSize + mSpace;
            counter++;
        }
        y += cellSize + mSpace;
        x = xLeft;
    }
    //ofDrawBitmapString("COUNTER:"+ofToString(counter),30,20);
    //printf("COUNTER:%i \n", counter);
};

bool LEDS::isClicked(int x, int y)
{
	if (y <= yRight && y >= yLeft && x <= xRight && x >= xLeft){
		return true;
	}
    return false;
};



void LEDS::setClicked(int x, int y, ofColor newColor, bool isDragged)
{
	for (unsigned int i = 0; i < columns * rows; i++)
		if (leds[i].isClicked(x, y)) 
		{
			if (leds[i].isSelected) {
                leds[i].color = newColor;
                changedBitmap = true;
            }
            if (ledLastClicked == i && isDragged) leds[i].isClicked(x, y);
            ledLastClicked = i;
            break;
		}
}

int LEDS::numClicked(int x, int y, ofColor newColor)
{
	for (unsigned int i = 0; i < columns * rows; i++)
		if (leds[i].isClicked(x, y)) 
		{
			leds[i].color = newColor;
            changedBitmap = true;
//            ledLastClicked = i;
            return i;
		}
}


void LEDS::setClickedAll()
{
	for (int i = 0; i < columns * rows; i++)
		leds[i].isSelected=true;
}

void LEDS::updateColor(ofColor newColor)
{
    for (int i = 0; i < rows*columns; i++)
    {
        if (leds[i].isSelected)
        {
            leds[i].color = newColor;
        }
    }
}

void LEDS::updateColor(ofColor newColor, ofColor inactColor)
{
    for (int i = 0; i < rows*columns; i++)
    {
        if (leds[i].isSelected)
        {
            leds[i].color = newColor;
        }
		else {
			leds[i].color = inactColor;
		}
    }
}

ofColor * LEDS::getBitmap()
{
    for (int i = 0; i < rows*columns; i++)
    {
        if (leds[i].isSelected)
        {
            bitmap[i] = leds[i].color;
        }
        else
        {
			bitmap[i] = ofColor();
        }
    }
	return bitmap;
}

unsigned char * LEDS::getBitmapChar()
{
    int counter = 0;
	for (int i = 0; i < rows*columns; i++)
    {
        if (leds[i].isSelected)
        {
            bitmapChar[counter++] = leds[i].color.r;
            bitmapChar[counter++] = leds[i].color.g;
            bitmapChar[counter++] = leds[i].color.b;
        }
        else
        {
			bitmapChar[counter++] = 0x00;
			bitmapChar[counter++] = 0x00;
			bitmapChar[counter++] = 0x00;
        }
    }
	return bitmapChar;
}

void LEDS::setupMidi(unsigned int ident, unsigned int channel, unsigned int inPort, unsigned int outPort) {
    midiChannel = channel;
	midiId = ident;
    midiInPort = inPort, midiOutPort = outPort;
    if (midiInPort != 100) { // do not turn on trick
        midiIn.openPort(midiInPort);
        if (midiOutPort != 100) midiOut.openPort(midiOutPort);
        ofAddListener(midiIn.newMessageEvent, this, &LEDS::receiveMidi);
    }
}

void LEDS::receiveMidi(ofxMidiEventArgs &args){
//	if (midiActivationCC == args.byteOne && args.channel == midiChannel) midiActive = args.byteTwo == 127 ? true : false;
    
	if (midiActive && args.channel == midiChannel &&
        args.byteOne >= midiSeqStartCC && 
        args.byteOne<midiSeqStartCC+columns*rows && args.byteTwo == 127)
    {
		leds[args.byteOne-midiSeqStartCC].isSelected = (leds[args.byteOne-midiSeqStartCC].isSelected == true ? false : true);
        changedBitmap = true;
    }
}