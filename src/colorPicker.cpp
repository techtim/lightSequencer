#include "colorPicker.h"

colorPicker::colorPicker(){

        width    = 0;
        height   = 0;

        xPos     = 0;
        yPos     = 0;
		
		colorPos = 0;
		triWidth = 30;
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

    hueControl.setup(xPos, yPos+35, 127, 15, true, 0, 127, true);
    hueControl.setActictiveText("");

}

bool colorPicker::isClicked(int x, int y, bool isDragged) {
    psyButton.isClicked(x, y);
    hueControl.isClicked(x, y);
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
    printf("POS =%i, LEFT=%i, POS RIGHT=%i\n", colorPos, colorPosStart, colorPosEnd);
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
    hueControl.draw();
    int psyLimitLeft = colorPosEnd > 0 ? colorPosStart : 0; // check if PosEnd > 0 means active color select region
    int psyLimitRight = colorPosEnd > 0 ? colorPosEnd : 360;
    if (psyButton.isOn) {
        if (evenCounter == 0) psyPos+=hueControl.getValue();
        evenCounter = evenCounter ? 0 : 1;
        if (psyPos >= psyLimitRight) psyPos = psyLimitLeft;
        colorPos = psyPos;
//        color.setHsb(psyPos, 255.0f, 255.0f);
        color = HsvToRgb(psyPos, 255, 255);
    } else {
//        color.setHue(colorPos);
        color = HsvToRgb(colorPos, 255, 255);
    }

	ofSetHexColor(color.getHex());
	ofTriangle(xPos+colorPos, yPos+img.height, 
			   xPos+colorPos-triWidth/2, yPos+img.height+15, 
			   xPos+colorPos+triWidth/2, yPos+img.height+15
	);	
}

ofColor colorPicker::HsvToRgb (float hue, float satur, float value )
{
	float r=0, g=0, b=0, fr, q, t, p;
    float p1, q1, t1;
    int i, H;
	
    ofColor rgbColor;
	
    rgbColor.set(r, g, b, 0);
	
//	if (hue==0) return rgbColor;
//    if (hue==360){
//        rgbColor.set(255, 255, 255, 255);
//        return rgbColor;
//    }
	
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
        hueControl.setupMidi(71);
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
    hueControl.receiveMidi(args);
}