//
//  AppView.cpp
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AppView.h"

//--------------------------------------------------------------
AppView::AppView(){
    ofxLogNotice() << "Constructing AppView" << endl;
}

//--------------------------------------------------------------
AppView::~AppView(){
    ofxLogNotice() << "Destroying AppView" << endl;
}

//--------------------------------------------------------------
void AppView::update(){
    
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
    
    ClipTimeline & clipTimeline = appModel->getClipTimeline();
    
    begin();
    {
        ofSetBackgroundColor(0, 0, 0, 0);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        if(playControllerStates.getState(kPLAYCONTROLLER_PLAY)){
            ofSetColor(255, 255, 255);
            clipTimeline.draw(instanceID);
        }
        ofDisableBlendMode();
    }
    end();

}

//--------------------------------------------------------------
void AppView::setInstanceID(int _instanceID){
    instanceID = _instanceID;
}

//--------------------------------------------------------------
int AppView::getInstanceID(){
    return instanceID;
}