/*
 *  ofxImage.cpp
 *  lightSequencer
 *
 *  Created by tim tavli on 05/12/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxImage.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "ofURLFileLoader.h"
#include "ofGraphics.h"
#include "FreeImage.h"

template<typename PixelType>
ofxImage_<PixelType>::ofImage_(){
	
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture
	
	//----------------------- init free image if necessary
	ofInitFreeImage();
	
#ifdef TARGET_ANDROID
	registerImage(this);
#endif
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofPixels_<PixelType> & pix){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture
	
	//----------------------- init free image if necessary
	ofInitFreeImage();
	
#ifdef TARGET_ANDROID
	registerImage(this);
#endif
	
	setFromPixels(pix);
}

template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofFile & file){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture
	
	//----------------------- init free image if necessary
	ofInitFreeImage();
	
#ifdef TARGET_ANDROID
	registerImage(this);
#endif
	
	loadImage(file);
}

template<typename PixelType>
ofImage_<PixelType>::ofImage_(const string & filename){
	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture
	
	//----------------------- init free image if necessary
	ofInitFreeImage();
	
#ifdef TARGET_ANDROID
	registerImage(this);
#endif
	
	loadImage(filename);
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>& ofImage_<PixelType>::operator=(const ofImage_<PixelType>& mom) {
	if(&mom==this) return *this;
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::ofImage_(const ofImage_<PixelType>& mom) {
	clear();
	clone(mom);
	update();
}

//----------------------------------------------------------
template<typename PixelType>
ofImage_<PixelType>::~ofImage_(){
	clear();
	
#ifdef TARGET_ANDROID
	unregisterImage(this);
#endif
}

template<typename PixelType>
void ofxImage_<PixelType>::draw()
{
	draw(xPos, yPos, width, height);
	ofSetColor(color.getR(), color.getG(), color.getB());
	ofTriangle(xPos+colorPos, yPos+height, 
			   xPos+colorPos-triWidth/2, yPos+height+20, 
			   xPos+colorPos+triWidth/2, yPos+height+20
			   );
	
}

template<typename PixelType>
void ofxImage_<PixelType>::setPosition(int x, int y){
    xPos = x;
    yPos = y;
}

template<typename PixelType>
bool ofxImage_<PixelType>::isClicked(int x, int y) {
    if (y >= yPos && y <= (yPos + height) && x >= xPos && x <= xPos + (width))
	{ 
		colorPos = getClickedX(x, y);
		color = HsvToRgb(colorPos, 255, 255);
		return true;
	}
    return false;
}

template<typename PixelType>
int ofxImage_<PixelType>::getClickedX(int x, int y) {
	return x-xPos;
}

template<typename PixelType>
int ofxImage_<PixelType>::getClickedY(int x, int y) {
	return y-yPos;
}

template<typename PixelType>
ofColor ofxImage_<PixelType>::HsvToRgb (float hue, float satur, float value )
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