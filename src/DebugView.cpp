//
//  DebugView.cpp
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "DebugView.h"

//--------------------------------------------------------------
DebugView::DebugView(){
    ofxLogNotice() << "Constructing DebugView" << endl;
    
    StateGroup newDebugViewStates("DebugViewStates", false);
    
    newDebugViewStates.addState(State(kDEBUGVIEW_SHOWINFO, "kDEBUGVIEW_SHOWINFO"));
    newDebugViewStates.addState(State(kDEBUGVIEW_SHOWSTATES, "kDEBUGVIEW_SHOWSTATES"));
    newDebugViewStates.addState(State(kDEBUGVIEW_SHOWPROPS, "kDEBUGVIEW_SHOWPROPS"));
    
    appModel->addStateGroup(newDebugViewStates);
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    
    debugViewStates.setState(kDEBUGVIEW_SHOWINFO, true);
    debugViewStates.setState(kDEBUGVIEW_SHOWSTATES, true);
    debugViewStates.setState(kDEBUGVIEW_SHOWPROPS, true);
    
}

//--------------------------------------------------------------
DebugView::~DebugView(){
    ofxLogNotice() << "Destroying AppView" << endl;
}

//--------------------------------------------------------------
void DebugView::update(){
    
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    
    ostringstream os;
    
    if(debugViewStates.getState(kDEBUGVIEW_SHOWINFO)) os << ofGetFrameRate() << endl << endl;
    if(debugViewStates.getState(kDEBUGVIEW_SHOWPROPS)) os << appModel->getAllPropsAsString() << endl;
    if(debugViewStates.getState(kDEBUGVIEW_SHOWSTATES)) os << appModel->getAllStatesAsString() << endl;
    
    begin();
    {
        ofSetBackgroundColor(0, 0, 0);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        ofSetColor(0, 255, 0);
        ofDrawBitmapString(os.str(), 20, 20);
        ofSetColor(255, 255, 255);
        ofDisableBlendMode();
    }
    end();
    
}