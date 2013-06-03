//
//  LoadController.cpp
//  DeepListen
//
//  Created by game over on 1/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "LoadController.h"

//--------------------------------------------------------------
LoadController::LoadController(){
    ofxLogVerbose() << "Creating LoadController" << endl;
}

//--------------------------------------------------------------
LoadController::~LoadController(){
    ofxLogVerbose() << "Destroying LoadController" << endl;
}

//--------------------------------------------------------------
void LoadController::setup(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");

    ClipTimeline & clipTimeline = appModel->getClipTimeline();
    
#ifdef VIDEO_TIMECODE
    if(appModel->getProperty<string>("PixelFormat") == "JPEG"){
        clipTimeline.setup(appModel->getProperty<string>("BlackPath"), OF_PIXELS_2YUV);
    }else{
        clipTimeline.setup(appModel->getProperty<string>("BlackPath"), OF_PIXELS_BGRA);
    }
#else
    if(appModel->getProperty<string>("PixelFormat") == "JPEG"){
        clipTimeline.setup(OF_PIXELS_2YUV);
    }else{
        clipTimeline.setup(OF_PIXELS_BGRA);
    }
#endif
    
}

//--------------------------------------------------------------
void LoadController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    
    FileList & videoFiles = appModel->getVideoAssetLoader();
    FileList & audioFiles = appModel->getAudioAssetLoader();
    FileList & textFiles = appModel->getTextAssetLoader();
    
    videoFiles.allowExt(".mov");
    audioFiles.allowExt(".wav");
    textFiles.allowExt(".txt");
    
    videoFiles.listDir(appModel->getProperty<string>("VideoPath"), false);
    audioFiles.listDir(appModel->getProperty<string>("AudioPath"), false);
    textFiles.listDir(appModel->getProperty<string>("TextPath"), true);
    
    // iterate through all video assets
    for(int i = 0; i < videoFiles.size(); i++){
        
        string name = videoFiles[i].name;
        
        // check if we have a clip by this name
        
        if(appModel->getClipExists(name)){
            
            // if the clip exists check if anything has changed
            Clip & clip = appModel->getClip(name);
            
            ofxLogNotice() << "CHECK clip: " << clip.getName() << endl;
//            if(name.rfind("OOOO_00_TITLE") != string::npos){
//                clip.setAnalyzed(false);
//            }
            // TODO: check if clip has been temporarily removed (ie., deleted == true)
            // TODO: check dates and sizes or overide dates and sizes
            // actually maybe this just happens in the next check ??
            
        }else{
            
            // if the clip doesn't exist check if we have 
            // both an audio and a text file for this video
            
            if(name.rfind("OOOO_00_TITLE") != string::npos){
                Clip clip = Clip(name);
                clip.setVideoFile(videoFiles[name]);
                clip.setAnalyzed(false);
                clip.setDeleted(false);
                appModel->setClip(clip);
            }
            
            if(audioFiles.getFileExists(name) && textFiles.getFileExists(name)){
                
                ofxLogNotice() << "ADDING clip: " << name << endl;
                
                // create a new clip
                
                Clip clip = Clip(name);
                clip.setVideoFile(videoFiles[name]);
                clip.setAudioFile(audioFiles[name]);
                clip.setTextFile(textFiles[name]);
                clip.setAnalyzed(false);
                clip.setDeleted(false);
                
                appModel->setClip(clip);
                
            }else{
                ofxLogNotice() << "IGNORING clip: " << name << endl;
            }
            
        }
        
    }
    
    // reverse check against clips to see if a video has been removed
    
    for(int i = 0; i < appModel->getClips().size(); i++){
        
        Clip & clip = appModel->getClip(i);
        
        // init the clip so that category, question etc are all generated
        clip.init();
        //clip.setCrop(100, MIN(500, clip.getTotalFrames()));
        
        // no need to check if this clip needs analysis and is not deleted
        if(clip.getAnalyzed() == false && clip.getDeleted() == false){
            
            ofxLogNotice() << "Clip " << clip.getName() << " is just being ADDED" << endl;
            
            continue;
        }
        
        // check if Files exist
        if(!videoFiles.getFileExists(clip.getName()) || 
           !audioFiles.getFileExists(clip.getName()) || 
           !textFiles.getFileExists(clip.getName())){
            
            // if any one of these files does not exist then
            // mark the clip as deleted in case we're just
            // temporarily removing the file
            
            if(clip.getName().rfind("OOOO_00_TITLE") != string::npos && videoFiles.getFileExists(clip.getName())) continue;
            
            ofxLogNotice() << "Clip " << clip.getName() << " marked for DELETION" << endl;
            
            clip.setDeleted(true);
            continue;
        }
        
        // check if date and size are the same (by using overload == operator
        
        // ... don't worry about text files - just accept the new one
        if(textFiles.getFile(clip.getName()) != clip.getTextFile()){
            clip.setTextFile(textFiles[clip.getName()]);
        }
        
        // ...but re-analyze if the audio or video are different
        if(videoFiles.getFile(clip.getName()) != clip.getVideoFile() ||
           audioFiles.getFile(clip.getName()) != clip.getAudioFile()){
            
            // if any of these have changed we need to re-analyze this clip
            // TODO: make checks so we only analyse if necessary...hmmm?
            
            clip.setVideoFile(videoFiles[clip.getName()]);
            clip.setAudioFile(audioFiles[clip.getName()]);
            
            ofxLogNotice() << "Clip " << clip.getName() << " marked for ANALYSIS" << endl;

            clip.setAnalyzed(false);

        }
        
    }
    
    Clips clips;
    clips.clips = appModel->getClips();
    clips.save();
    
    if(appModel->getNumClipsForAnalysis() > 0){
        appControllerStates.setState(kAPPCONTROLLER_ANALYZE);
        return;
    }else{
        appControllerStates.setState(kAPPCONTROLLER_PLAY);
    }
    
}