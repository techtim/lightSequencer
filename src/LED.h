#ifndef _LED
#define _LED

#include "ofMain.h"
#include "ofColor.h"

class LED {

	public:
		LED();
		LED(int xL, int yL, int xR, int yR);

		void set(int xLeft, int yLeft, int xRight, int yRight);
		void setColor(ofColor col) {
		   color = col;
		};
		void position(int xL, int yL, int xR, int yR);
        bool isClicked(int x, int y);
        
        LED operator=(const LED led) {
            isSelected = led.isSelected;
            color.set(led.color);
        }
    
        bool isSelected;
        int xLeft;
        int yLeft;
        int xRight;
        int yRight;
		int num;
		int numColumn;
		int numRow;
		ofColor color;
};

#endif
