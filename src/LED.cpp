#include "LED.h"

LED::LED()
{
    color.set(0, 0, 0, 0);
}

LED::LED(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
};

void LED::position(int xL, int yL, int xR, int yR)
{
    xLeft = xL, yLeft = yL, xRight = xR, yRight = yR;
    isSelected = false;
};

bool LED::isClicked(int x, int y) {
    if (y <= yRight && y >= yLeft && x <= xRight && x >= xLeft) {
        isSelected = (isSelected == true ? false : true);
//        printf("isSelected=%i; num=%i",isSelected, num);
        printf("X=%i; Y=%i",x, y);
        return true;
    }
    return false;
};

