//
//  AnalyzeView.cpp
//  DeepListen
//
//  Created by game over on 2/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AnalyzeView.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
AnalyzeView::AnalyzeView(){
    ofxLogNotice() << "Constructing AnalyzeView" << endl;
    StateGroup newAnalyzeViewStates("AnalyzeViewStates", false);
    
    newAnalyzeViewStates.addState(State(kANALYZEVIEW_SHOW, "kANALYZEVIEW_SHOW"));
    
    appModel->addStateGroup(newAnalyzeViewStates);
    
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    
    analyzeViewStates.setState(kANALYZEVIEW_SHOW, true);
}

//--------------------------------------------------------------
AnalyzeView::~AnalyzeView(){
    ofxLogNotice() << "Destroying AnalyzeView" << endl;
}

//--------------------------------------------------------------
void AnalyzeView::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
#ifdef USE_FENSTER
    StateGroup & analyzeViewStates = appModel->getStateGroup("AnalyzeViewStates");
    if(playControllerStates.getState(kPLAYCONTROLLER_PLAY) && analyzeViewStates.getState(kANALYZEVIEW_SHOW)){
#else
    if(playControllerStates.getState(kPLAYCONTROLLER_PLAY)){
#endif
        
        ClipTimeline & clipTimeline = appModel->getClipTimeline();
        begin();
        {
            ofSetBackgroundColor(0, 0, 0);
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            if(playControllerStates.getState(kPLAYCONTROLLER_PLAY)){
                clipTimeline.drawTimeline(0, 0, getWidth(), getHeight());
            }
            ofDisableBlendMode();
        }
        end();
    }
    
    
    if(appControllerStates.getState(kAPPCONTROLLER_ANALYZE)){
        if(!waveformFBO.isAllocated()) waveformFBO.allocate(width, 200);
        
        // get a clip for analysis
        Clip & clip = appModel->getClipForAnalysis();
        
        // get refs to the analysis tools
        ofVideoPlayer & video = appModel->getAnalysisVideo();
        ofxSamplePlayer & audio = appModel->getAnalysisAudio();
        ofxCv::ContourFinder& contourFinder = appModel->getAnalysisContourFinder();
        
        if(lastclip != clip.getName()){
            waveformFBO.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT);
                audio.drawWaveForm(0, 50, waveformFBO.getWidth(), 100);
            }
            waveformFBO.end();
            lastclip = clip.getName();
        }
        
        if(!video.isLoaded()) return;
        
        begin();
        {
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            
            ofSetColor(255, 255, 255, 255);
            
            ofPushMatrix();
            ofTranslate((width - clip.getWidth())/2.0f, (height - clip.getHeight())/2.0f);
            
            video.draw(0, 0, clip.getWidth(), clip.getHeight());
            contourFinder.draw();
            
            ofSetColor(255, 0, 0);
            ofNoFill();
            
            ofRect(clip.getRect());
            
            ofPopMatrix();
            
            ofSetColor(255, 255, 255, 255);
            
            waveformFBO.draw(0, 0);
            
            ofDisableBlendMode();
        }
        end();
    }

}

//--------------------------------------------------------------
void AnalyzeView::mousePressed(int x, int y, int button){
    ClipTimeline & timeline = appModel->getClipTimeline();
    cout << timeline.getClipAt(x, y, ofGetWidth(), ofGetHeight()) << endl;
}