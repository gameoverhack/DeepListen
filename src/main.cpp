#include "AppController.h"
#include "ofAppGlutWindow.h"

#ifdef USE_FENSTER
#include "ofxFensterManager.h"

//--------------------------------------------------------------
int main(){
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(ofxFensterManager::get(), 1680/2.0f, 1050/2.0f, OF_WINDOW);
	ofRunFensterApp(new AppController()); // start the app
}

#else

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1920*2, 1080, OF_WINDOW);
	ofRunApp(new AppController()); // start the app
}

#endif