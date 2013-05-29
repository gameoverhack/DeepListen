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
    
//    ClipGroup allClips = appModel->getClipGroup("originalClips");
//    
//    int minLength = INFINITY;
//    int maxLength = 0;
//    
//    for(int i = 0; i < allClips.size(); i++){
//        minLength = MIN(minLength, allClips[i].frames);
//        maxLength = MAX(maxLength, allClips[i].frames);
//    }
//    
//    int deltaLength = maxLength - minLength;
//    int bins = ceil((float) deltaLength/1500);
//    if(bins < 0) return;
//    vector<int> counts;
//    vector<float> times;
//    times.resize(bins);
//    counts.assign(bins, 0);
//    
//    
//    for(int i = 0; i < allClips.size(); i++){
//        for(int j = 0; j < bins; j++){
//            times[j] = (float)(minLength + ((float)deltaLength/bins) * j + 1)/25.0/60.0;
//            if(allClips[i].frames > minLength + ((float)deltaLength/bins) * j && allClips[i].frames <= minLength + ((float)deltaLength/bins) * (j+1)){
//                counts[j]++;
//            }
//        }
//    }
//    
//    cout << minLength << "  :  " << maxLength << "  :  " << (float)deltaLength/bins << " ";// << endl;
//    //cout << times << endl;
//    cout << counts << "   " << allClips.size() << endl;
    
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