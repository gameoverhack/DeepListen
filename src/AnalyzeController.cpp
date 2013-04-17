//
//  AnalyzeController.cpp
//  DeepListen
//
//  Created by game over on 1/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AnalyzeController.h"

//--------------------------------------------------------------
AnalyzeController::AnalyzeController(){
    ofxLogVerbose() << "Creating AnalyzeController" << endl;
}

//--------------------------------------------------------------
AnalyzeController::~AnalyzeController(){
    ofxLogVerbose() << "Destroying AnalyzeController" << endl;
}

//--------------------------------------------------------------
void AnalyzeController::setup(){
    analyzeView = new AnalyzeView();
    
    analyzeView->setup(appModel->getProperty<float>("VideoWidth"), 
                       appModel->getProperty<float>("VideoHeight"), 
                       ViewOption(VIEW_USE_FBO | VIEW_USE_WINDOW),
                       (string)"analyze");
    
    analyzeView->setPosition(appModel->getProperty<float>("MonitorWidth")/2.0, 
                             0, 
                             appModel->getProperty<float>("MonitorWidth")/2.0, 
                             appModel->getProperty<float>("MonitorHeight")/2.0);
}

//--------------------------------------------------------------
void AnalyzeController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    if(appModel->getNumClipsForAnalysis() > 0){
        
        // get a clip for analysis
        Clip & clip = appModel->getClipForAnalysis();
        
        // get refs to the analysis tools
        ofVideoPlayer & video = appModel->getAnalysisVideo();
        ofxSamplePlayer & audio = appModel->getAnalysisAudio();
        ofxCv::ContourFinder& contourFinder = appModel->getAnalysisContourFinder();
        
        // check if we're analysing this video by seeing if we've loaded the video
        if(video.getMoviePath() != clip.videoFile.path){
            
            ofxLogNotice() << "Start Analysing: " << clip.name << endl;
            
            // load the text
            ofFile file(clip.textFile.path);
            ofBuffer buffer(file);
            clip.text = buffer.getText();
            
            appModel->setProperty("AnalyzeText", clip.text);
            
            // load the audio and analyze the waveform
            audio.load(clip.audioFile.path);
            audio.generateWaveForm();
            
            // find the start of audio
            for(int i = 0; i < audio.getWaveForm().size(); i++){
                if(ABS((float)(audio.getWaveForm()[i].maxL)) > 0 ||
                   ABS((float)(audio.getWaveForm()[i].maxR)) > 0){
                    clip.audioinpct = (float)i/audio.getWaveForm().size();
                    break;
                }
            }
            
            // find the end of audio
            for(int i = audio.getWaveForm().size() - 1; i >= 0; i--){
                if(ABS((float)(audio.getWaveForm()[i].maxL)) > 0 ||
                   ABS((float)(audio.getWaveForm()[i].maxR)) > 0){
                    clip.audioutpct = (float)i/audio.getWaveForm().size();
                    break;
                }
            }
            
            // load the video
            video.loadMovie(clip.videoFile.path);
            clip.frames = video.getTotalNumFrames();
            
            // set rect to opposite minimum and maximums
            clip.rect = ofRectangle(appModel->getProperty<float>("OutputWidth"), appModel->getProperty<float>("OutputHeight"), 0, 0);
            
            ofxLogNotice() << "Frames: " << clip.frames << " audio in: " << clip.frames * clip.audioinpct << " out: " << clip.frames * clip.audioutpct << endl;
            
        }else{
            
            // push through the frames of the video
            if(video.getCurrentFrame() + appModel->getProperty<int>("FrameJump") <= video.getTotalNumFrames()){
                
                // set the frame and update video
                video.setFrame(video.getCurrentFrame() + appModel->getProperty<int>("FrameJump"));
                video.update();
                
                // set contour finder props - bit overzealous doing it every frame?
                contourFinder.setMinAreaRadius(appModel->getProperty<int>("ContourMinArea"));
                contourFinder.setMaxAreaRadius(appModel->getProperty<int>("ContourMaxArea"));
                contourFinder.setThreshold(appModel->getProperty<int>("ContourThreshold"));
                
                // find the contour
                contourFinder.findContours(video);
                
                // get max and min for every point in every contour to find total bounding rect
                // TODO: find out if there's a quicker more ofxCv way to do this

                for(int i = 0; i < contourFinder.getContours().size(); i++){
                    vector<cv::Point> pts = contourFinder.getContours()[i];
                    for(int j = 0; j < pts.size(); j++){
                        clip.rect.x = MIN(pts[j].x, clip.rect.x);
                        clip.rect.y = MIN(pts[j].y, clip.rect.y);
                        clip.rect.width = MAX(pts[j].x, clip.rect.width + clip.rect.x) - clip.rect.x;
                        clip.rect.height = MAX(pts[j].y, clip.rect.height + clip.rect.y) - clip.rect.y;
                        
                    }
                }

            }else{ // the video is at or near last frame
                
                ofxLogNotice() << "Finsished Analysing: " << clip.name << endl;
                
                // mark clip as analysed and close the video
                clip.analyzed = true;
                video.close();
                
                // save the app model so we don't lose data (is this a good idea?)
                appModel->save("config", ARCHIVE_BINARY);
            }
            
        }

        // update the view
        analyzeView->update();

    }else{
    
        ofxLogNotice() << "All analysis is done!" << endl;
        appControllerStates.setState(kAPPCONTROLLER_PLAY);
   
    }
    
}

//--------------------------------------------------------------
void AnalyzeController::draw(){
    analyzeView->draw();
}