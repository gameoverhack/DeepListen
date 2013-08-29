//
//  AppController.cpp
//  emptyExample
//
//  Created by game over on 25/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AppController.h"

static int appTimer = 0;
static bool started = false;

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
    
#ifndef NO_SOUND
    system("~/../../usr/local/bin/jackd -R -d coreaudio -d 'de_IMM_driver_USBAudioEngine:1658A66' -p 512 -r 48000 'JackRouter' -i 8 -o 8 &");
    ofSleepMillis(2000);
#else
    system("~/../../usr/local/bin/jackd -R -d coreaudio -p 512 -r 48000 'JackRouter' -i 2 -o 2 &");
    ofSleepMillis(2000);
#endif
    
    ofxLogNotice() << "AppController start setup" << endl;
    
    // register key/mouse events (this way we get them from any window)
    ofRegisterMouseEvents(this);
    ofRegisterKeyEvents(this);
    
    // register AppController states
    StateGroup newAppControllerStates("AppControllerStates");
    newAppControllerStates.addState(State(kAPPCONTROLLER_INIT, "kAPPCONTROLLER_INIT"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_LOAD, "kAPPCONTROLLER_LOAD"));
    newAppControllerStates.addState(State(kAPPCONTROLLER_SOUND, "kAPPCONTROLLER_SOUND"));
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
    
//    appModel->setProperty("VerticalSync", false);
    ofSetVerticalSync(appModel->getProperty<bool>("VerticalSync"));
    
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
    
    string rootPath = "";
    
#if defined(RMBP_INTERNAL_SSD)
    rootPath = "/Users/deeplistening/Desktop/DeepData/";
#elif defined(RMBP_PEGASUS_SSD)
    rootPath = "/Volumes/Deep00/";
#elif defined(RETINA)
    rootPath = "/Users/gameover/Desktop/DeepDataTest/";
#elif defined(BLACKCAVIAR)
    rootPath = "/Volumes/Love/DeepData/";
#else
    rootPath = "/Volumes/Ersatz/CODECTESTS/";
#endif
    
    appModel->setProperty("TextPath", (string)rootPath + "TEXT/");
    appModel->setProperty("MusicPath", (string)rootPath + "MUSIC/");
    appModel->setProperty("AudioPath", (string)rootPath + "WAVE/");
    appModel->setProperty("BlackPath", (string)rootPath + "black.mov");
#ifdef JPEG
    appModel->setProperty("VideoPath", (string)rootPath + "JPEG60/");
#else
    
#ifdef RMBP_PEGASUS_SSD
    appModel->setProperty("VideoPath", (string)"/Volumes/Deep0");
#else
    appModel->setProperty("VideoPath", (string)rootPath + "ANIME60/");
#endif
    
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
    
//    appModel->setProperty("VolumePeople", 1.0f);
//    appModel->setProperty("VolumeMusic", 1.0f);
    
#if defined(RMBP_INTERNAL_SSD) || defined(RMBP_PEGASUS_SSD) || defined(RETINA) || defined(BLACKCAVIAR)
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
                                ViewOption(VIEW_USE_BEZIERWARP),
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
    
    
    for(int i = 0; i < 50; i++){
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
                       ViewOption(),
                       (string)"analyze");
    
    // Create other controllers
    loadController = new LoadController();
    loadController->setup();
    
    analyzeController = new AnalyzeController();
    analyzeController->setup();
    
    playController = new PlayController();
    playController->setup();
    
    networkController = new NetworkController();
    networkController->setup();
    

    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    debugViewStates.setState(kDEBUGVIEW_SHOWINFO, 0);
    analyzeViewStates.setState(kANALYZEVIEW_SHOW, 0);
    
    bShowCursor = false;

    CGPoint p;
    p.x = 1980 * 2; p.y = 1200;
    CGPostMouseEvent( p, 1, 1, 1 );   
    CGPostMouseEvent( p, 1, 1, 0 );
    
    ofHideCursor();
    ofSetFullscreen(true);
    
    system("./../../../appswitch -h -a \"Finder\"");
    system("./../../../appswitch -h -a \"TotalMix FX\"");
    system("./../../../appswitch -f -a \"DeepListen\"");
    
}

//--------------------------------------------------------------
void AppController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    networkController->update();
    
    switch (appControllerStates.getState()) {
        
        case kAPPCONTROLLER_INIT:
        {
            system("./../../../appswitch -f -a \"DeepScreenBlocker\"");
            appControllerStates.setState(kAPPCONTROLLER_LOAD);
        }
            break;
        case kAPPCONTROLLER_LOAD:
        {
            loadController->update();
            playController->resetClipGroups();
        }
            break;
        case kAPPCONTROLLER_SOUND:
        {
            soundController->setup(16,8);
            appControllerStates.setState(kAPPCONTROLLER_PLAY);
            appTimer = ofGetElapsedTimeMillis();
        }
            break;
        case kAPPCONTROLLER_ANALYZE:
        {
            analyzeController->update();
        }
            break;
        case kAPPCONTROLLER_PLAY:
        {
            if(!started){
                
                if(ofGetElapsedTimeMillis() - appTimer > 2000){
                    started = true;
                    system("./../../../appswitch -h -a \"Finder\"");
                    system("./../../../appswitch -f -a \"DeepListen\"");
                    system("./../../../appswitch -h -a \"DeepScreenBlocker\"");
                    ofSleepMillis(500);
                    system("pkill DeepScreenBlocker");
                }
                
            }
            playController->update();
            
            if(timeline.getRestart()){
                restart();
            }
        }
            break;
    }
    
}

//--------------------------------------------------------------
void AppController::draw(){
    
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    ofSetColor(0, 0, 0);
    ofFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
    
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
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            
            appViews[0]->draw(w0x,
                              w0y,
                              1920.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              1920.0f * (ofGetWidth()/2.0f) / 1920.0f / 16.0 * 9.0);
            
            appViews[1]->draw(w1x + 1920.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              w1y,
                              1440.0f * (ofGetWidth()/2.0f) / 1920.0f,
                              (1440.0f * (ofGetWidth()/2.0f) / 1920.0f) / 4.0 * 3.0);
            
        }
            break;
    }
    
    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) debugView->draw();

    if(analyzeViewStates.getState(kANALYZEVIEW_SHOW)) analyzeView->draw();
    
    ofDisableBlendMode();
    
    glFlush();
    
}

//--------------------------------------------------------------
void AppController::exit(){
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    timeline.stop();
    
    appModel->saveTimelineHistory();
    
    timeline.clear();
    
    appModel->save("config", ARCHIVE_BINARY);
    
    system("pkill DeepScreenBlocker");
    system("pkill jackdmp");
    system("pkill jackd");
}

//--------------------------------------------------------------
void AppController::restart(){
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    timeline.stop();
    appModel->saveTimelineHistory();
    timeline.clear();
    
    system("./../../../DeepScreenBlocker.app/Contents/MacOS/DeepScreenBlocker &");
    ofSleepMillis(500);
    system("./../../../appswitch -f -a \"DeepScreenBlocker\"");
    system("./DeepListen &");
    system("./../../../appswitch -h -a \"Finder\"");
    system("./../../../appswitch -f -a \"DeepScreenBlocker\"");
    ofExit();
    
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
        case 'y':
            restart();
            break;
        case 'x':
        {
            vector<int> blackspace = appModel->getProperty< vector<int> >("BlackSpace");
            for(int i = 0; i < blackspace.size(); i++){
                if(blackspace[i] > timeline.getCurrentFrame() && timeline.getCurrentFrame() < blackspace[i] - 500){
                    timeline.stop();
                    timeline.setFrame(blackspace[i] - 500);
                    timeline.setPaused(false);
                    break;
                }
            }
        }
            break;
        case ' ':
            appModel->resetHistory();
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
        case 'v':
            appModel->setProperty("VerticalSync", !appModel->getProperty<bool>("VerticalSync"));
            ofSetVerticalSync(appModel->getProperty<bool>("VerticalSync"));
            break;
        case 't':
            appModel->setProperty("LogToFile", !appModel->getProperty<bool>("LogToFile"));
            ofxLogSetLogToFile(appModel->getProperty<bool>("LogToFile"), ofToDataPath("log_" + ofGetTimestampString() + ".txt"));
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
        case '9':
        {
            float v = appModel->getProperty<float>("VolumePeople");
            v -= 0.05;
            v = CLAMP(v, 0.0f, 1.0f);
            timeline.setVolumePeople(v);
            appModel->setProperty("VolumePeople", v);
        }
            break;
        case '0':
        {
            float v = appModel->getProperty<float>("VolumePeople");
            v += 0.05;
            v = CLAMP(v, 0.0f, 1.0f);
            timeline.setVolumePeople(v);
            appModel->setProperty("VolumePeople", v);
        }
            break;
        case '-':
        {
            float v = appModel->getProperty<float>("VolumeMusic");
            v -= 0.05;
            v = CLAMP(v, 0.0f, 1.0f);
            timeline.setVolumeMusic(v);
            appModel->setProperty("VolumeMusic", v);
        }
            break;
        case '=':
        {
            float v = appModel->getProperty<float>("VolumeMusic");
            v += 0.05;
            v = CLAMP(v, 0.0f, 1.0f);
            timeline.setVolumeMusic(v);
            appModel->setProperty("VolumeMusic", v);
        }
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
            timeline.forward();
            break;
        case OF_KEY_LEFT:
            timeline.rewind();
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
    string clipMouse = timeline.getClipAt(e.x, e.y, ofGetWidth(), ofGetHeight()).getName();
    appModel->setProperty("ClipMouse", clipMouse);
    cout << clipMouse << endl;
    
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