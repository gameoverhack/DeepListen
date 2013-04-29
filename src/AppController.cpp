//
//  AppController.cpp
//  emptyExample
//
//  Created by game over on 25/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AppController.h"

//--------------------------------------------------------------
AppController::AppController(){
    ofxLogVerbose() << "Creating AppController" << endl;
}

//--------------------------------------------------------------
AppController::~AppController(){
    ofxLogVerbose() << "Destroying AppController" << endl;
}
#include "ofxSamplePlayer.h"
//--------------------------------------------------------------
void AppController::setup(){
    
    // set log levels
    ofxLogSetLogLevel(LOG_VERBOSE);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // set veryical sync (does this work on all windows?)
    ofSetVerticalSync(true);
    
    // set name of the primary window
    ofxFensterManager::get()->getPrimaryWindow()->setWindowTitle("debug");
    
    ofxLogNotice() << "AppController start setup" << endl;
    
    // register key/mouse events (this way we get them from any window)
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    
    // register AppController states
    StateGroup newAppControllerStates("AppControllerStates");
    newAppControllerStates.addState(State(kAPPCONTROLLER_INIT, "kAPPCONTROLLER_INIT"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_LOAD, "kAPPCONTROLLER_LOAD"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_ANALYZE, "kAPPCONTROLLER_ANALYZE"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_PLAY, "kAPPCONTROLLER_PLAY"));
    
    // add them to the model
    appModel->addStateGroup(newAppControllerStates);

    // get them back from the model so that changes go live
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    appControllerStates.setState(kAPPCONTROLLER_INIT);

    StateGroup newAppViewStates("AppViewStates", false);
    
    newAppViewStates.addState(State(kAPPVIEW_SHOWWARP_0, "kAPPVIEW_SHOWWARP_0"));
    newAppViewStates.addState(State(kAPPVIEW_SHOWWARP_1, "kAPPVIEW_SHOWWARP_1"));
    
    appModel->addStateGroup(newAppViewStates);
    
    StateGroup & appViewStates = appModel->getStateGroup("AppViewStates");
    
    appViewStates.setState(kAPPVIEW_SHOWWARP_0, false);
    appViewStates.setState(kAPPVIEW_SHOWWARP_1, false);
    
    // load the config file
    appModel->load("config", ARCHIVE_BINARY);
    
    // set properties here (can comment once saved)
    appModel->setProperty("TextPath", (string)"/Volumes/Artaud/CODECTESTS/TEXT/");
    appModel->setProperty("VideoPath", (string)"/Volumes/Artaud/CODECTESTS/JPEG60/");
    appModel->setProperty("AudioPath", (string)"/Volumes/Artaud/CODECTESTS/WAVE/");
    
    appModel->setProperty("ContourMinArea", 10);
    appModel->setProperty("ContourMaxArea", 1200);
    appModel->setProperty("ContourThreshold", 10);
    
    appModel->setProperty("FrameJump", 100);
    
    appModel->setProperty("MonitorWidth", 1680.0f);
    appModel->setProperty("MonitorHeight", 1050.0f);
    
    appModel->setProperty("VideoWidth", 1920.0f);
    appModel->setProperty("VideoHeight", 1080.0f);
    
    appModel->setProperty("OutputWidth", 1920.0f);
    appModel->setProperty("OutputHeight", 1080.0f);
    
    appModel->setProperty("PixelFormat", (string)"  ");
    
    // create appView windows
    for(int screen = 0; screen < 2; screen++){
        appViews[screen] = new AppView();
        appViews[screen]->setup(appModel->getProperty<float>("OutputWidth"), 
                                appModel->getProperty<float>("OutputHeight"), 
                                ViewOption(VIEW_USE_BEZIERWARP | VIEW_USE_WINDOW),
                                (string)"screen_" + ofToString(screen));
        
        // setup the warp grid for this appView window
        BezierWarp & warp = appViews[screen]->getWarp<BezierWarp>();
        warp.setWarpGrid(5, 3);
        warp.setWarpGridResolution(100.0f);
        
        // load the control points for this appView Window
        string pntPropName = "warpPoints_"+ofToString(screen);
        if(appModel->hasProperty< vector<float> >(pntPropName)){
            warp.setControlPoints(appModel->getProperty< vector<float> >(pntPropName));
        }
        
        appViews[screen]->setPosition(appModel->getProperty<float>("MonitorWidth")/2.0 * screen, 
                                      appModel->getProperty<float>("MonitorHeight")/2.0, 
                                      appModel->getProperty<float>("MonitorWidth")/2.0, 
                                      appModel->getProperty<float>("MonitorHeight")/2.0);
        
        appViews[screen]->setInstanceID(screen);
        
        // TODO: manage putting onto correct screens...
        
    }
    
    
    // make a debug window
    debugView = new DebugView();
    debugView->setup(ofGetWidth(), 
                     ofGetHeight(), 
                     ViewOption(VIEW_USE_FBO),
                     (string)"debug");
    
    // Create other controllers
    
    loadController = new LoadController();
    loadController->setup();
    
    analyzeController = new AnalyzeController();
    analyzeController->setup();
    
    playController = new PlayController();
    playController->setup();
    
}

//--------------------------------------------------------------
void AppController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    switch (appControllerStates.getState()) {
        case kAPPCONTROLLER_INIT:
        {
            appControllerStates.setState(kAPPCONTROLLER_LOAD);
        }
            break;
        case kAPPCONTROLLER_LOAD:
        {
            loadController->update();
        }
            break;
        case kAPPCONTROLLER_ANALYZE:
        {
            analyzeController->update();
        }
            break;
        case kAPPCONTROLLER_PLAY:
        {
            playController->update();
        }
            break;
    }
    
    appViews[0]->update();
    appViews[1]->update();
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) debugView->update();
}

//--------------------------------------------------------------
void AppController::draw(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    switch (appControllerStates.getState()) {
        case kAPPCONTROLLER_INIT:
        {
            // nothing to do
        }
            break;
        case kAPPCONTROLLER_LOAD:
        {
            // nothing to do
        }
            break;
        case kAPPCONTROLLER_ANALYZE:
        {
            // nothing to do
        }
            break;
        case kAPPCONTROLLER_PLAY:
        {
            // nothing to do
        }
            break;
    }
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) debugView->draw();
    
}

//--------------------------------------------------------------
void AppController::exit(){
    appModel->save("config", ARCHIVE_BINARY);
}

//--------------------------------------------------------------
void AppController::keyPressed(ofKeyEventArgs & e){
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & appViewStates = appModel->getStateGroup("AppViewStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    BezierWarp & warp0 = appViews[0]->getWarp<BezierWarp>();
    BezierWarp & warp1 = appViews[1]->getWarp<BezierWarp>();
    
    int key = e.key;
    
    switch (key) {
        case ' ':
            playControllerStates.setState(kPLAYCONTROLLER_INIT);
            break;
        case 'd':
            debugViewStates.toggleState(kDEBUGVIEW_SHOWINFO);
            break;
        case 'p':
            debugViewStates.toggleState(kDEBUGVIEW_SHOWPROPS);
            break;
        case 's':
            debugViewStates.toggleState(kDEBUGVIEW_SHOWSTATES);
            break;
        case '1':
            appViewStates.toggleState(kAPPVIEW_SHOWWARP_0);
            warp0.setShowWarpGrid(appViewStates.getState(kAPPVIEW_SHOWWARP_0));
            break;
        case '2':
            appViewStates.toggleState(kAPPVIEW_SHOWWARP_1);
            warp1.setShowWarpGrid(appViewStates.getState(kAPPVIEW_SHOWWARP_1));
            break;
        case 'r':
            if(appViewStates.getState(kAPPVIEW_SHOWWARP_0)) warp0.resetWarpGrid();
            if(appViewStates.getState(kAPPVIEW_SHOWWARP_1)) warp1.resetWarpGrid();
            break;
        case '`':
            appModel->setProperty("warpPoints_0", warp0.getControlPoints());
            appModel->setProperty("warpPoints_1", warp1.getControlPoints());
            break;
        case 'u':
            appModel->setProperty("ContourMinArea", appModel->getProperty<int>("ContourMinArea") + 10);
            break;
        case 'j':
            appModel->setProperty("ContourMinArea", appModel->getProperty<int>("ContourMinArea") - 10);
            break;
        case 'i':
            appModel->setProperty("ContourMaxArea", appModel->getProperty<int>("ContourMaxArea") + 10);
            break;
        case 'k':
            appModel->setProperty("ContourMaxArea", appModel->getProperty<int>("ContourMaxArea") - 10);
            break;
        case 'o':
            appModel->setProperty("ContourThreshold", appModel->getProperty<int>("ContourThreshold") + 1);
            break;
        case 'l':
            appModel->setProperty("ContourThreshold", appModel->getProperty<int>("ContourThreshold") - 1);
            break;
    }

//    ofxLogVerbose() << e.key << endl;
    
}

//--------------------------------------------------------------
void AppController::keyReleased(ofKeyEventArgs & e){
//    ofxLogVerbose() << e.key << endl;
}

//--------------------------------------------------------------
void AppController::mouseMoved(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
}

//--------------------------------------------------------------
void AppController::mouseDragged(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
}

//--------------------------------------------------------------
void AppController::mousePressed(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
}

//--------------------------------------------------------------
void AppController::mouseReleased(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
}