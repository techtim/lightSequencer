#include "ofImage.h"

template<typename PixelType>
class ofxImage_ : public ofImage_<PixelType>{
	public :
	
		ofxImage_();
		virtual ~ofxImage_();

		void	setPosition(int x, int y);
		bool	isClicked(int x, int y);
		int		getClickedX(int x, int y);
		int		getClickedY(int x, int y);
		void	draw();
		
		ofColor HsvToRgb (float hue, float satur, float value );
		
		float xPos;
		float yPos;
		ofColor color;
		unsigned int colorPos;
		unsigned int triWidth;
}

template class ofxImage_<unsigned char>;
template class ofxImage_<float>;
template class ofxImage_<unsigned short>;