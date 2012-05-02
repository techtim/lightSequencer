#include "colorPicker.h"

colorPicker::colorPicker(){

        width    = 0;
        height   = 0;

        xPos     = 0;
        yPos     = 0;
		
		colorPos = 0;
		triWidth = 40;
        psyPos   = 0;
        evenCounter = 0;
}

void colorPicker::loadImage(string path){
	img.loadImage(path);
    width = img.width;
    height = img.height;
}

void colorPicker::setPosition(int x, int y){
    xPos = x;
    yPos = y;
    psyButton.setup(xPos-30, yPos, 30, 30, true);
    psyButton.setActictiveText("psy");
}

bool colorPicker::isClicked(int x, int y) {
    psyButton.isClicked(x, y);
    if (y >= yPos && y <= (yPos + height) && x >= xPos && x <= xPos + (width))
	{ 
        colorPos = getClickedX(x, y);
		color = HsvToRgb(colorPos, 255, 255);
		return true;
	}
    return false;
}


int colorPicker::getClickedX(int x, int y) {
	return x-xPos;
}

int colorPicker::getClickedY(int x, int y) {
	return y-yPos;
}

void colorPicker::draw()
{
	img.draw(xPos, yPos);
    psyButton.draw();
    if (psyButton.isOn) {
        if (evenCounter == 0) psyPos+=60;
        evenCounter = evenCounter ? 0 : 1;
        colorPos = psyPos;
        if (psyPos >= 360) psyPos = 1;
        color = HsvToRgb(psyPos, 255, 255);
    } else {
        color = HsvToRgb(colorPos, 255, 255);
    }

	ofSetHexColor(color.getHex());
	ofTriangle(xPos+colorPos, yPos+img.height, 
			   xPos+colorPos-triWidth/2, yPos+img.height+20, 
			   xPos+colorPos+triWidth/2, yPos+img.height+20
	);	
}

ofColor colorPicker::HsvToRgb (float hue, float satur, float value )
{
	float r=0, g=0, b=0, fr, q, t, p;
    float p1, q1, t1;
    int i, H;
	
    ofColor rgbColor;
	
    rgbColor.set(r, g, b, 0);
	
	if (hue==0) return rgbColor;
    if (hue==360){
        rgbColor.set(255, 255, 255, 255);
        return rgbColor;
    }
	
    //  For rgbColor
    H = (int)(hue / 60);
    fr = hue/60 - H;
    p = (value * (255 - satur))/255;
    q = (value * (255 - satur * fr))/255;
    t = (value * (255 - satur * (1.0 - fr)))/255;
	
    // For Saturation select line pure hueColor with saturation 255
	
    p1 = (value * (255 - 255))/255;
    q1 = (value * (255 - 255 * fr))/255;
    t1 = (value * (255 - 255 * (1.0 - fr)))/255;
	
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

void colorPicker::setupMidi(unsigned int ident, unsigned int channel, unsigned int port) {
    midiChannel = channel;
	midiId = ident;
    if (port != 100) { // do not turn on trick
        midiIn.openPort(port);
        ofAddListener(midiIn.newMessageEvent, this, &colorPicker::receiveMidi);
    }
}

void colorPicker::receiveMidi(ofxMidiEventArgs &args)
{
	if (midiChannel == args.channel)
		if (midiId == args.byteOne)
		{
//            printf("PICKER Knob CHAN: %i ID: %i VALUE: %i \n", args.channel, midiId, midiValue) ;
            int valueMapped = ofMap(args.byteTwo, 0, 127, 0, 360);
            colorPos = valueMapped;
            color = HsvToRgb(colorPos, 255, 255);
		}
}