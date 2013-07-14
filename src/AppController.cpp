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

//--------------------------------------------------------------
void AppController::setup(){
    
    // set log levels
    ofxLogSetLogLevel(LOG_VERBOSE);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
//    ofxLogSetLogToFile(true);
    
    system("~/../../usr/local/bin/jackd -R -d coreaudio -d 'de_IMM_driver_USBAudioEngine:1658A66' -p 512 -r 48000 'JackRouter' -i 8 -o 8 &");
    
    ofSleepMillis(10000);
    
    // set veryical sync (does this work on all windows?)
    ofSetVerticalSync(true);
//    ofSetFrameRate(30);
    
#ifdef USE_FENSTER
    // set name of the primary window
    ofxFensterManager::get()->getPrimaryWindow()->setWindowTitle("debug");
#endif
    
    ofxLogNotice() << "AppController start setup" << endl;
    
    // register key/mouse events (this way we get them from any window)
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    
    // register AppController states
    StateGroup newAppControllerStates("AppControllerStates");
    newAppControllerStates.addState(State(kAPPCONTROLLER_INIT, "kAPPCONTROLLER_INIT"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_LOAD, "kAPPCONTROLLER_LOAD"));
//    newAppControllerStates.addState(State(kAPPCONTROLLER_SOUND, "kAPPCONTROLLER_SOUND"));
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
#ifdef JPEG
    appModel->load("config_jpeg", ARCHIVE_BINARY);
#else
    appModel->load("config", ARCHIVE_BINARY);
#endif
    
//    appModel->setProperty("LogToFile", false);
    ofxLogSetLogToFile(appModel->getProperty<bool>("LogToFile"), ofToDataPath("log_" + ofGetTimestampString() + ".txt"));
    
    appModel->setProperty("mouseX", 0);
    appModel->setProperty("mouseY", 0);
    
//    appModel->setProperty("warp0_x", 0.0f);
//    appModel->setProperty("warp0_y", 0.0f);
    
//    appModel->setProperty("warp1_x", 0.0f);
//    appModel->setProperty("warp1_y", 0.0f);
    
    w0x = appModel->getProperty<float>("warp0_x");
    w0y = appModel->getProperty<float>("warp0_y");
    w1x = appModel->getProperty<float>("warp1_x");
    w1y = appModel->getProperty<float>("warp1_y");
    
#if defined(MINI)
    appModel->setProperty("TextPath", (string)"/Volumes/DeepData/TEXT/");
    appModel->setProperty("MusicPath", (string)"/Volumes/DeepData/MUSIC/");
#ifdef JPEG
    appModel->setProperty("VideoPath", (string)"/Volumes/DeepData/JPEG60/");
#else
    appModel->setProperty("VideoPath", (string)"/Volumes/DeepData/ANIME60/");
#endif
    appModel->setProperty("AudioPath", (string)"/Volumes/DeepData/WAVE/");
    appModel->setProperty("BlackPath", (string)"/Volumes/DeepData/black.mov");
#elif defined(RETINA)
    appModel->setProperty("TextPath", (string)"/Users/gameover/Desktop/DeepDataTest/TEXT/");
    appModel->setProperty("MusicPath", (string)"/Users/gameover/Desktop/DeepDataTest/MUSIC/");
#ifdef JPEG
    appModel->setProperty("VideoPath", (string)"/Users/gameover/Desktop/DeepDataTest/JPEG60/");
#else
    appModel->setProperty("VideoPath", (string)"/Users/gameover/Desktop/DeepDataTest/ANIME60/");
#endif
    appModel->setProperty("AudioPath", (string)"/Users/gameover/Desktop/DeepDataTest/WAVE/");
    appModel->setProperty("BlackPath", (string)"/Users/gameover/Desktop/DeepDataTest/black.mov");
#else
    appModel->setProperty("TextPath", (string)"/Volumes/Ersatz/CODECTESTS/TEXT/");
    appModel->setProperty("MusicPath", (string)"/Volumes/Ersatz/CODECTESTS/MUSIC/");
    appModel->setProperty("VideoPath", (string)"/Volumes/Ersatz/CODECTESTS/JPEG60/");
    appModel->setProperty("AudioPath", (string)"/Volumes/Ersatz/CODECTESTS/WAVE/");
    appModel->setProperty("BlackPath", (string)"/Volumes/Ersatz/CODECTESTS/black.mov");
#endif
    
    appModel->setProperty("OverrideVideoPath", true);
    appModel->setProperty("ImportClipRects", false);
    
    appModel->setProperty("ContourMinArea", 10);
    appModel->setProperty("ContourMaxArea", 1200);
    appModel->setProperty("ContourThreshold", 11);
    
    appModel->setProperty("FrameJump", 100);
    
    appModel->setProperty("MonitorWidth", 1680.0f);
    appModel->setProperty("MonitorHeight", 1050.0f);
    
    appModel->setProperty("VideoWidth", 1920.0f);
    appModel->setProperty("VideoHeight", 1080.0f);
    
    appModel->setProperty("OutputWidth_0", 1920.0f);
    appModel->setProperty("OutputHeight_0", 1080.0f);
    appModel->setProperty("OutputWidth_1", 1440.0f);
    appModel->setProperty("OutputHeight_1", 1080.0f);
    
#if defined(MINI) || defined(RETINA)
#ifdef JPEG
    appModel->setProperty("PixelFormat", (string)"JPEG");
#else
    appModel->setProperty("PixelFormat", (string)"BGRA");
#endif
#else
    appModel->setProperty("PixelFormat", (string)"JPEG");
#endif
    
    // create appView windows
    for(int screen = 0; screen < 2; screen++){
        appViews[screen] = new AppView();
        appViews[screen]->setup(appModel->getProperty<float>("OutputWidth_" + ofToString(screen)),
                                appModel->getProperty<float>("OutputHeight_" + ofToString(screen)),
#ifdef USE_FENSTER
                                ViewOption(VIEW_USE_BEZIERWARP | VIEW_USE_WINDOW),
#else
                                ViewOption(VIEW_USE_BEZIERWARP),
#endif
                                (string)"screen_" + ofToString(screen));
        
        // setup the warp grid for this appView window
        BezierWarp & warp = appViews[screen]->getWarp<BezierWarp>();
        warp.setWarpGrid(5, 4);
        warp.setWarpGridResolution(ceil(1920.0/80.0f), ceil(1080.0/80.0f));
        
        // load the control points for this appView Window
        string pntPropName = "warpPoints_"+ofToString(screen);
        if(appModel->hasProperty< vector<float> >(pntPropName)){
            warp.setControlPoints(appModel->getProperty< vector<float> >(pntPropName));
        }
        
#ifdef USE_FENSTER
        appViews[screen]->setPosition(appModel->getProperty<float>("MonitorWidth")/2.0 * screen, 
                                      appModel->getProperty<float>("MonitorHeight")/2.0, 
                                      appModel->getProperty<float>("MonitorWidth")/2.0, 
                                      appModel->getProperty<float>("MonitorHeight")/2.0);
#endif
        appViews[screen]->setInstanceID(screen);
        
        // TODO: manage putting onto correct screens...
        
    }
    
//    appModel->removeAllProperties();
//    appModel->setProperty("warpPoints_0", appViews[0]->getWarp<BezierWarp>().getControlPoints());
//    appModel->setProperty("warpPoints_1", appViews[1]->getWarp<BezierWarp>().getControlPoints());
//    appModel->setProperty("warp0_x", w0x);
//    appModel->setProperty("warp0_y", w0y);
//    appModel->setProperty("warp1_x", w1x);
//    appModel->setProperty("warp1_y", w1y);
    
    
    for(int i = 0; i < 16; i++){
        appModel->removeProperty<string>("PlayStateA_"+ofToString(i));
        appModel->removeProperty<string>("PlayStateV_"+ofToString(i));
    }
    
    // make a debug window
    debugView = new DebugView();
    debugView->setup(ofGetWidth(), 
                     ofGetHeight(), 
                     ViewOption(),
                     (string)"debug");
    
    analyzeView = new AnalyzeView();
    
    analyzeView->setup(appModel->getProperty<float>("VideoWidth"),
                       appModel->getProperty<float>("VideoHeight"),
#ifdef USE_FENSTER
                       ViewOption(VIEW_USE_FBO | VIEW_USE_WINDOW),
#else
                       ViewOption(),
#endif
                       (string)"analyze");
    
#ifdef USE_FENSTER
    analyzeView->setPosition(appModel->getProperty<float>("MonitorWidth")/2.0,
                             0,
                             appModel->getProperty<float>("MonitorWidth")/2.0,
                             appModel->getProperty<float>("MonitorHeight")/2.0);
#endif
    
    // Create other controllers
    loadController = new LoadController();
    loadController->setup();
    
    analyzeController = new AnalyzeController();
    analyzeController->setup();
    
    playController = new PlayController();
    playController->setup();

#ifndef NO_SOUND
    soundController->setup(16, 8);
#endif
    
    networkController = new NetworkController();
    networkController->setup();
    
    ofHideCursor();
    ofSetFullscreen(true);
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    debugViewStates.setState(kDEBUGVIEW_SHOWINFO, 0);
    analyzeViewStates.setState(kANALYZEVIEW_SHOW, 0);
    
    bShowCursor = false;
    
    system("./../../../data/Hide.sh \"TotalMix FX\"");
    
}

//--------------------------------------------------------------
void AppController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    networkController->update();
    
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
//        case kAPPCONTROLLER_SOUND:
//        {
//
//        }
//            break;
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
    
}

//--------------------------------------------------------------
void AppController::draw(){
    
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");

#ifndef USE_FENSTER
    ofSetColor(0, 0, 0);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
#endif
    
    appViews[0]->update();
    appViews[1]->update();

    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) debugView->update();
    if(analyzeViewStates.getState(kANALYZEVIEW_SHOW)) analyzeView->update();
    
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
#ifndef USE_FENSTER
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
//            appViews[0]->draw(0, 0, 1920, 1080);
//            appViews[1]->draw(1920, 0, 1440, 1080);
            appViews[0]->draw(w0x,
                              w0y,
                              1920.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              1920.0f * (ofGetWidth()/2.0f) / 1920.0f / 16.0 * 9.0);
            
            appViews[1]->draw(w1x + 1920.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              w1y,
                              1440.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              (1440.0f * (ofGetWidth()/2.0f) / 1920.0f) / 4.0 * 3.0);
#endif
        }
            break;
    }
    
    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) debugView->draw();

#ifndef USE_FENSTER
    
    if(analyzeViewStates.getState(kANALYZEVIEW_SHOW)) analyzeView->draw();
    ofDisableBlendMode();
#endif
    
    glFlush();
    
}

//--------------------------------------------------------------
void AppController::exit(){
#ifdef JPEG
    appModel->save("config_jpeg", ARCHIVE_BINARY);
#else
    appModel->save("config", ARCHIVE_BINARY);
#endif
}

//--------------------------------------------------------------
void AppController::keyPressed(ofKeyEventArgs & e){
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    StateGroup & appViewStates = appModel->getStateGroup("AppViewStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    BezierWarp & warp0 = appViews[0]->getWarp<BezierWarp>();
    BezierWarp & warp1 = appViews[1]->getWarp<BezierWarp>();
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    int key = e.key;
    
    switch (key) {
        case ' ':
            playControllerStates.setState(kPLAYCONTROLLER_INIT);
            break;
        case 'z':
            playControllerStates.setState(kPLAYCONTROLLER_STOP);
            break;
        case 'm':
            soundController->setMasterVolume(0.0f);
            break;
        case 'n':
            soundController->setMasterVolume(1.0f);
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
        case 'a':
            analyzeViewStates.toggleState(kANALYZEVIEW_SHOW);
            break;
        case '1':
            appViewStates.toggleState(kAPPVIEW_SHOWWARP_0);
            warp0.setShowWarpGrid(appViewStates.getState(kAPPVIEW_SHOWWARP_0));
            break;
        case '2':
            appViewStates.toggleState(kAPPVIEW_SHOWWARP_1);
            warp1.setShowWarpGrid(appViewStates.getState(kAPPVIEW_SHOWWARP_1));
            break;
        case 'w':
            warp0.toggleDoWarp();
            warp1.toggleDoWarp();
            break;
        case 'r':
            if(appViewStates.getState(kAPPVIEW_SHOWWARP_0)){
                warp0.resetWarpGrid();
                appModel->setProperty("warp0_x", 0.0f);
                appModel->setProperty("warp0_y", 0.0f);
                w0x = appModel->getProperty<float>("warp0_x");
                w0y = appModel->getProperty<float>("warp0_y");
            }
            if(appViewStates.getState(kAPPVIEW_SHOWWARP_1)){
                warp1.resetWarpGrid();
                appModel->setProperty("warp1_x", 0.0f);
                appModel->setProperty("warp1_y", 0.0f);
                w1x = appModel->getProperty<float>("warp1_x");
                w1y = appModel->getProperty<float>("warp1_y");
            }
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
        case ',':
            timeline.previousClip();
            break;
        case '.':
            timeline.nextClip();
            break;
        case '/':
            timeline.togglePaused();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            bShowCursor = !bShowCursor;
            if(bShowCursor){
                ofShowCursor();
            }else{
                ofHideCursor();
            }
            break;
        case OF_KEY_RIGHT:
            timeline.nextClip();
            break;
        case OF_KEY_LEFT:
            timeline.previousClip();
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
    appModel->setProperty("mouseX", e.x);
    appModel->setProperty("mouseY", e.y);
}

//--------------------------------------------------------------
void AppController::mouseDragged(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
    
    if(e.button != 2) return;
    
    float diffX = e.x - mouseDownX;
    float diffY = e.y - mouseDownY;
    
    BezierWarp & warp0 = appViews[0]->getWarp<BezierWarp>();
    BezierWarp & warp1 = appViews[1]->getWarp<BezierWarp>();
    
    if(warp0.getShowWarpGrid()){
        w0x = appModel->getProperty<float>("warp0_x") + diffX;
        w0y = appModel->getProperty<float>("warp0_y") + diffY;
        warp0.setWarpGridPosition(w0x, w0y, warp0.getWidth(), warp0.getHeight());
    }
    
    if(warp1.getShowWarpGrid()){
        w1x = appModel->getProperty<float>("warp1_x") + diffX;
        w1y = appModel->getProperty<float>("warp1_y") + diffY;
        warp1.setWarpGridPosition(w1x + 1920.0f * (ofGetWidth()/2.0f) / 1920.0f, w1y, warp1.getWidth(), warp1.getHeight());
    }
    
}

//--------------------------------------------------------------
void AppController::mousePressed(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
    ClipTimeline & timeline = appModel->getClipTimeline();
    cout << timeline.getClipAt(e.x, e.y, ofGetWidth() / 2.0f, ofGetHeight()) << endl;
    
    mouseDownX = e.x;
    mouseDownY = e.y;
}

//--------------------------------------------------------------
void AppController::mouseReleased(ofMouseEventArgs & e){
//    ofxLogVerbose() << e.x << " " << e.y << " " << e.button << endl;
    if(e.button != 2) return;

    BezierWarp & warp0 = appViews[0]->getWarp<BezierWarp>();
    BezierWarp & warp1 = appViews[1]->getWarp<BezierWarp>();
    
    if(warp0.getShowWarpGrid()){
        appModel->setProperty("warp0_x", w0x);
        appModel->setProperty("warp0_y", w0y);
    }
    
    if(warp1.getShowWarpGrid()){
        appModel->setProperty("warp1_x", w1x);
        appModel->setProperty("warp1_y", w1y);
    }
    
    mouseDownX = mouseDownY = 0;
}