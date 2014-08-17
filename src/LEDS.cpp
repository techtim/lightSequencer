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

void LEDS::clear(){
//    if (bSetuped && ledsNumber > 0) {
//        delete [] leds;
//        delete [] bitmap;
//        delete [] bitmapChar;
//    }
//    bSetuped = false;
}

void LEDS::set(int col, int row, int cell, int x, int y, int spac, bool setAllWhite)
{
    columns = col;
    rows = row;
    cellSize = cell;
	mSpace = spac;
    ledsNumber = rows*columns;

    xLeft = x;
    yLeft = y;
    xRight = x + columns * cellSize + (columns-1) * mSpace;
    yRight = y + rows * cellSize + (rows-1) * mSpace;

	leds = new LED [columns * rows];
    ledLastClicked = -1; // mean no clicks made
	bitmap = new ofColor [columns * rows];
	bitmapChar = new unsigned char [columns * rows * 3];
	
	if (setAllWhite) {
		for (int i=0; i<columns*rows; i++)
			leds[i].color = ofColor(255.0f,255.0f,255.0f,255.0f);
	}
	matrixImg.allocate(columns, rows, OF_IMAGE_COLOR);
    //matrixImg.setPosition(xLeft, yLeft);

    midiActive = false;
    midiSeqStartCC = 10;

    changedBitmap = false;

    inAddrMode = false;
    ledsInChain = 0;
    myFont.loadFont("../../data/verdana.ttf", 10);

    setPosition(xLeft, yLeft);
};

void LEDS::setPosition(int x, int y)
{
	xLeft = x , yLeft = y;
    xRight = x + columns * cellSize + (columns-1) * mSpace;
    yRight = y + rows * cellSize + (rows-1) * mSpace;
    int count = 0;
	int row = 0, cell = 0;

    for (int i = 0; i < columns * rows; i++)
	{
		int ledXLeft = xLeft + cell * mSpace + cell * cellSize;
		int ledYLeft = yLeft + row * mSpace + row * cellSize;
		leds[count].num = count;
		leds[count].numColumn = cell;
		leds[count].numRow = row;
        leds[count].setDmxAddress(DMX_ADDR_MAP[count]);
		leds[count].position(ledXLeft, ledYLeft, ledXLeft + cellSize, ledYLeft + cellSize);

		//printf("%i PIXEL: XLeft = %i , YLeft = %i \n", count, ledXLeft, ledYLeft);
		count++;
		cell++;
		if (cell == columns) cell = 0, row++;
	}

}

void LEDS::trigADSR() {
    for (int i=0; i<ledsNumber; i++) {
        if (leds[i].isSelected) {
            leds[i].trigADSR();
        }
    }
}

void LEDS::setADSR(ofVec3f adsr, unsigned int _min, unsigned int _max){
    setADSR(adsr.x, adsr.y, adsr.z, _min, _max);
}

void LEDS::setADSR(unsigned int _att, unsigned int _dec, unsigned int _rel, unsigned int _min, unsigned int _max ){
    for (int i=0; i<ledsNumber; i++)
        leds[i].setADSR(_att, _dec, _rel, _min, _max);
}

void LEDS::parseBitmap(unsigned char * pixels, bool onlyState)
{
	int count = 0;
	for (int i = 0; i < columns * rows * 3; i+=3)
	{	
        if (pixels[i] == '\0' && pixels[i+1] == '\0' && pixels[i+2] == '\0') {
            leds[count].isSelected = false;
        } else {
            leds[count].isSelected = true;
//       		printf("%i PIXEL: %i , %i , %i \n", count, pixels[i], pixels[i+1], pixels[i+2]);
        }
        leds[count].color.set((float)pixels[i],(float)pixels[i+1], (float)pixels[i+2]);
        count++;
	}
    changedBitmap = true;
//    printf("\n \n \n");
};

void LEDS::parseBitmap(ofColor * pixels, bool onlyState)
{
	int count = 0;
	for (int i = 0; i < columns * rows; i++)
	{
//        if (!onlyState || pixels[i].getHex() != 0) { // update only active pixels
        if (onlyState && color == ofColor(255)) {
//            fuck
            ;;
        }
            leds[count].color.set(pixels[i]);
            leds[count].isSelected = pixels[i].getHex() == 0 ? false : true;
//        }
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

void LEDS::update()
{
    for (int i = 0; i < ledsNumber; i++)
        leds[i].update();
}

void LEDS::print(int shape)
{
    int x = xLeft;
    int y = yLeft;
    int counter = 0;
    float parallH = cellSize;
    float parallW = cellSize;
    float parallMove = parallH/sin(70.0f*PI/180)*cos(70.0f*PI/180);
    float parallStartX = xLeft+(parallW)*columns/4;
    if (shape == 1) x = parallStartX;

    if (midiActive) {
        ofSetColor(150, 150, 150);
        ofRect(x-mSpace, y-mSpace, (cellSize + mSpace) * columns+mSpace, (cellSize + mSpace) * rows+mSpace);

        for (int i=0; i < rows * columns; i++) {
            leds[i].isSelected ? 
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 127) :
            midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 0);
        }
//        for (int i=rows * columns; i<16; i++) midiOut.sendControlChange(midiChannel, i + midiSeqStartCC, 0);
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
//                ofSetColor(leds[counter].color.r, leds[counter].color.g, leds[counter].color.b);
                ofSetColor(leds[counter].color);
            }
            else
            {
                ofSetColor(0,0,0);
            }

            switch (shape) {
                case 0:
                    ofRect(x, y, cellSize, cellSize);
                    if (inAddrMode) {
                        ofSetColor(0,150,200);
                        ofDrawBitmapString( ofToString(leds[counter].dmxStartAddress), x+cellSize/2, y+cellSize/2 );
                    }
                    x += cellSize + mSpace;
                    break;
                case 1:
//                    ofFill();
//                    ofBeginShape();
//                        ofVertex(x+parallMove,y);
//                        ofVertex(x+parallMove+parallW,y);
//                        ofVertex(x+parallW,y+parallH);
//                        ofVertex(x,y+parallH);
//                    ofEndShape();
//                    x += parallW + mSpace;
//                    break;

                    break;
            }
//            x += cellSize + mSpace;
            counter++;
        }

        switch (shape) {
            case 0:
                y += cellSize + mSpace;
                x = xLeft;
                break;
            case 1:
                y += parallH + mSpace;
                x = parallStartX-parallMove*(i+1);
                break;
                
                break;
        }
    }
    
//    ofNoFill();
//    ofSetColor(0, 255, 0);
//    ofRect(getRegion());
//    ofFill();
    //ofDrawBitmapString("COUNTER:"+ofToString(counter),30,20);
    //printf("COUNTER:%i \n", counter);
};

void LEDS::setupTexture(int w, int h) {
    tex = new ofTexture;
    tex->allocate(w, h, GL_RGB);
}

ofTexture * LEDS::getTexture(int shape)
{
//    if (!tex->isAllocated()) return tex;

//    tex->clear();
    int counter = 0;
    int x = xLeft;
    int y = yLeft;
    float parallH = cellSize/3;
//
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (leds[counter].isSelected == true)
            {
//                ofSetColor(leds[counter].color.r, leds[counter].color.g, leds[counter].color.b);
                ofSetColor(leds[counter].color);
            }
            else
            {
                ofSetColor(0,0,0);
            }
    //            ofTriangle(x, y, x+cellSize, y, x+cellSize/2, y+cellSize);
//            ofRect(x, y, cellSize, cellSize);
            switch (shape) {
                case 0:
                    ofRect(x, y, cellSize, cellSize);
                    break;
                case 1:
//                    ofTriangle(x, y, x+cellSize, y, x+cellSize/2, y+cellSize);
//                    printf("%fx%d - %fx%d - %dx%d - %dx%d \n",
//                           x+parallH/sin(70), y,
//                           x+parallH/sin(70)+cellSize,y,
//                           x+cellSize,y+cellSize,
//                           x,y+cellSize);
                    ofFill();
                    ofBeginShape();
                        ofVertex(x+parallH/sin(70),y);
                        ofVertex(x+parallH/sin(70)+cellSize,y);
                        ofVertex(x+cellSize,y+cellSize);
                        ofVertex(x,y+cellSize);
                    ofEndShape();
                    break;
            }

            x += cellSize + mSpace;
            counter++;
        }
        y += cellSize + mSpace;
        x = xLeft;
    }

    tex->loadScreenData(x,y,columns * cellSize + (columns-1) * mSpace, rows * cellSize + (rows-1) * mSpace);

//    unsigned char pixels[w*h*4];
//    tex.loadData(pixels, w, h, GL_RGBA);
    return tex;
}

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
                if (inAddrMode) leds[i].setNumInChain(++ledsInChain);
            } else {
                if (inAddrMode) leds[i].setNumInChain(0);
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

void LEDS::updateColor(ofColor newColor, bool updAll)
{
    for (int i = 0; i < rows*columns; i++)
    {
        if (updAll || leds[i].isSelected)
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
			bitmap[i] = ofColor(0);
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
            bitmapChar[counter++] = (leds[i].color.r == (unsigned char)255 ? (unsigned char)254 : leds[i].color.r);
            bitmapChar[counter++] = (leds[i].color.g == (unsigned char)255 ? (unsigned char)254 : leds[i].color.g);;
            bitmapChar[counter++] = (leds[i].color.b == (unsigned char)255 ? (unsigned char)254 : leds[i].color.b);;
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

void LEDS::getPixelsState (string & pixelsState) {
    pixelsState = "";
    for (int i = 0; i < rows*columns; i++)
    {
        pixelsState += leds[i].isSelected ? "1": "0";
    }
}

void LEDS::setPixelsState (const string pixelsState) {
    for (int i = 0; i < rows*columns; i++)
    {
        if (pixelsState.length()<i ) break;
        leds[i].isSelected = (pixelsState.substr(i, 1) == "1" ? true : false);
    }
    
}

void LEDS::setSaturation(float sat) {
    for (int i = 0; i < rows*columns; i++)
        leds[i].color.setSaturation(sat);
}

void LEDS::setBrightness(float bright) {
    for (int i = 0; i < rows*columns; i++)
        leds[i].color.setBrightness(bright);
}

string LEDS::getChainState () {
    string chainState = "";
//    printf("\n [");
    for (int i = 0; i < rows*columns; i++)
    {
        //        pixelsState += leds[i].numInChain ? "1": "0";
        string num_str = ofToString(leds[i].numInChain);
        num_str.length();
        for (int i = 0; i<3-num_str.size(); i++) chainState += "0";
        chainState += num_str;
//        printf("%s , %li, ", num_str.c_str(), num_str.size());
    }
//    printf("]\n");
//    printf("%s", chainState.c_str());
    return chainState;
}

void LEDS::setChainState (const string & chainState) {
    printf("\n !!! \n");
    int cntr = 0, chain_ctr = 0;
    for (int i = 0; i < rows*columns*3; i+=3)
    {
        if (chainState.length()<i ) break;
//        string num_str = chainState.substr(i, 3);
//        printf("%s", num_str.c_str());
        stringstream convert(chainState.substr(i, 3)); // stringstream used for the conversion initialized with the contents of Text
        if (!(convert >> leds[cntr].numInChain))
            leds[cntr].numInChain = 0;
        if (leds[cntr].numInChain > 0) chain_ctr++;
//        printf(" %i \n", leds[cntr].numInChain);
        cntr++;
    }
    ledsInChain = chain_ctr;
}

void LEDS::getChainBitmapChar(unsigned char * chainMap) {
    int counter = 0;

	for (int i = 0; i < rows*columns; i++)
    {
        if (leds[i].numInChain > 0) {
            if (leds[i].isSelected)
            {
                chainMap[leds[i].numInChain*3-3] = (leds[i].color.r == (unsigned char)255 ? (unsigned char)254 : leds[i].color.r);
                chainMap[leds[i].numInChain*3-2] = (leds[i].color.g == (unsigned char)255 ? (unsigned char)254 : leds[i].color.g);;
                chainMap[leds[i].numInChain*3-1] = (leds[i].color.b == (unsigned char)255 ? (unsigned char)254 : leds[i].color.b);;
            }
            else
            {
                chainMap[leds[i].numInChain*3-3] = 0x00;
                chainMap[leds[i].numInChain*3-2] = 0x00;
                chainMap[leds[i].numInChain*3-1] = 0x00;
            }
        }
    }
//    for (int i=0; i<ledsInChain; i++) printf("%f",chainMap)
    return chainMap;
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


void LEDS::getDmx(ofxDmx &dmx) {
    for (int i = 0; i < rows*columns; i++)
    {
        leds[i].getDmx(dmx);
    }
    
}
