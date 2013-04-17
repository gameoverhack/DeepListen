#include "AppController.h"
#include "ofxFensterManager.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(ofxFensterManager::get(), 1680/2.0f, 1050/2.0f, OF_WINDOW);
	ofRunFensterApp(new AppController()); // start the app
}
