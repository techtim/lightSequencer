#include "colorPicker.h"

colorPicker::colorPicker():width(0),height(0),
xPos(0),yPos(0), hueSpeed(0),
colorPos(0),triWidth(30),psyPos(0),evenCounter(0),colorPosStart(-1),colorPosEnd(-1)
{

}

void colorPicker::loadImage(string path){
	img.loadImage(path);
    width = img.width;
    height = img.height;
}

void colorPicker::setPosition(int x, int y){
    xPos = x;
    yPos = y;
    gui = new ofxUISuperCanvas("fx", 0, 0, 0, height);
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->setColorBack(ofColor(0,0,0,0));
    gui->setFontSize(OFX_UI_FONT_SMALL, 5);
    
    psyButton = new ofxUILabelToggle("psy", false, 0,0,0,0, OFX_UI_FONT_SMALL);
//    gui->addWidgetRight(psyButton);
    
    hueControl = new ofxUIMinimalSlider("speed", 0, 60, &hueSpeed, 128, 10);
    gui->addWidgetRight(hueControl);
    gui->autoSizeToFitWidgets();
    gui->setPosition(x, y+height);

    //    psyButton.setup(xPos-30, yPos, 30, 30, true);
    //    psyButton.setActictiveText("psy");
    //
    //    hueControl.setup(xPos, yPos+35, 127, 15, true, 0, 127, true);
    //    hueControl.setActictiveText("");
    
}

bool colorPicker::isClicked(int x, int y, bool isDragged) {
    //    psyButton.isClicked(x, y);
    //    hueControl.isClicked(x, y);
    if (y >= yPos && y <= (yPos + height) && x >= xPos && x <= xPos + (width))
	{
        colorPos = getClickedX(x, y);
        if (isDragged) {
            printf("Dragged: ");
            colorPosEnd = colorPos;
        } else {
            colorPosStart = colorPos;
            colorPosEnd = -1;
        }
		color = HsvToRgb(colorPos, 255, 255);
		return true;
	}
//    printf("POS =%i, LEFT=%i, POS RIGHT=%i\n", colorPos, colorPosStart, colorPosEnd);
    return false;
}


int colorPicker::getClickedX(int x, int y) {
	return x-xPos;
}

int colorPicker::getClickedY(int x, int y) {
	return y-yPos;
}

int colorPicker::getColorPos() {
    return colorPos;
}

void colorPicker::draw()
{
	img.draw(xPos, yPos);
    int psyLimitLeft = colorPosStart > 0 ? colorPosStart : 0; // check if PosEnd > 0 means active color select region
    int psyLimitRight = colorPosEnd > 0 ? colorPosEnd : 360;
    if (hueControl->getValue() > 0) {
        if (evenCounter == 0) psyPos+=hueControl->getValue();
        if (evenCounter == 1) psyPos-=hueControl->getValue();
//        evenCounter = evenCounter ? 0 : 1;
        if (psyPos >= psyLimitRight) evenCounter = 1;
        if (psyPos <= psyLimitLeft) evenCounter = 0;
        colorPos = psyPos;
//        color.setHsb(psyPos, 255.0f, 255.0f);
        color = HsvToRgb(psyPos, 255, 255);
    } else {
//        color.setHue(colorPos);

        color = HsvToRgb(colorPos, 255, 255);
//        color = img.getColor(colorPos, 0);
    }
    
    gui->draw();
    
	ofSetColor(50);
    ofNoFill();
    ofRect(xPos+colorPos-5, yPos-5, 10, img.height+10);

    ofSetHexColor(color.getHex());
    ofFill();
    ofRect(xPos+colorPos-5, yPos-5, 10, img.height+10);
    
//	ofTriangle(xPos+colorPos, yPos+img.height,
//			   xPos+colorPos-triWidth/2, yPos+img.height+15,
//			   xPos+colorPos+triWidth/2, yPos+img.height+15
//               );
}

ofColor colorPicker::HsvToRgb (float hue, float satur, float value )
{
	float r=0, g=0, b=0, fr, q, t, p;
    int i, H;
	
    ofColor rgbColor;
	
    rgbColor.set(r, g, b, 0);
	
    //	if (hue==0) return rgbColor;
//    if (hue==0){
//        rgbColor.set(255, 255, 255, 255);
//        return rgbColor;
//    }
	
    //  For rgbColor
    H = (int)(hue / 60);
    fr = hue/60 - H;
    p = (value * (255 - satur))/255;
    q = (value * (255 - satur * fr))/255;
    t = (value * (255 - satur * (1.0 - fr)))/255;
	
    //Msg	= "hue: " + ofToString(hue) + ", satur: " + ofToString(satur) +", H: "+ ofToString(H) + ", p: " + ofToString(p) + ", q: " + ofToString(q) + ", t: " + ofToString(t) + ", fr: " + ofToString(fr);
	
    switch (H) {
        case 0:
            rgbColor.set(value, t, p, value);
            break;
        case 1:
            rgbColor.set(q, value, p, value);
            break;
        case 2:
            rgbColor.set(p, value, t, value);
            break;
        case 3:
            rgbColor.set(p, q, value, value);
            break;
        case 4:
            rgbColor.set(t, p, value, value);
            break;
        case 5:
            rgbColor.set(value, p, q, value);
            break;
        default:
            rgbColor.set(255, 255, 255, 255);
    }
	
    return rgbColor;
	
}

void colorPicker::setupMidi(unsigned int ident, unsigned int channel, unsigned int port, unsigned int psy_bottom, unsigned int psy_speed) {
    midiChannel = channel;
	midiId = ident;
    if (port != 100) { // do not turn on trick
        midiIn.openPort(port);
        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        midiIn.ignoreTypes(false, false, false);
        midiIn.addListener(this);
        midiIn.setVerbose(false);

//        ofAddListener(midiIn.newMessageEvent, this, &colorPicker::receiveMidi);
        //        hueControl.setupMidi(psy_speed);
        //        psyButton.setupMidi( psy_bottom, channel, port, port);
    }
}

void colorPicker::newMidiMessage(ofxMidiMessage& msg) {
	if (midiChannel == msg.channel)
		if (midiId == msg.control)
		{
            //            printf("PICKER Knob CHAN: %i ID: %i VALUE: %i \n", args.channel, midiId, midiValue) ;
            int valueMapped = ofMap(msg.value, 0, 127, 0, 360);
            colorPos = valueMapped;
            color = HsvToRgb(colorPos, 255, 255);
		}
    //    hueControl.receiveMidi(args);
}