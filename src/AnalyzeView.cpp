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
    ofSetBackgroundColor(0, 0, 0);
}

//--------------------------------------------------------------
AnalyzeView::~AnalyzeView(){
    ofxLogNotice() << "Destroying AnalyzeView" << endl;
}

//--------------------------------------------------------------
void AnalyzeView::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    if(playControllerStates.getState(kPLAYCONTROLLER_PLAY) && appModel->getProperty<bool>("ShowTimeline")){
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
            video.draw(0, 0, width, height);
            contourFinder.draw();
            ofSetColor(255, 0, 0);
            ofNoFill();
            ofRect(clip.getPosition());
            ofSetColor(255, 255, 255, 255);
            waveformFBO.draw(0, 0);
            ofDisableBlendMode();
        }
        end();
    }
    
    
    
}