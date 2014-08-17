#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
#ifdef TARGET_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
#else
    int main(){
#endif
        ofAppGlutWindow window; // create a window
        // set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
        ofSetupOpenGL(&window, 1000, 650, OF_WINDOW);
        
        
        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp(new testApp());
        
    }
