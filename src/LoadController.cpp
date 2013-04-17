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
    
    ofVideoPlayer vid;
    vid.loadMovie("/Volumes/Artaud/CODECTESTS/JPEG60/FAML_ROLE_XNSS_00_MARBEEW.mov");
    
    if(appModel->getProperty<string>("PixelFormat") == "JPEG"){
        clipTimeline.setup(4, OF_PIXELS_2YUV);
    }else{
        clipTimeline.setup(4, OF_PIXELS_BGRA);
    }
    
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
            
            ofxLogNotice() << "CHECK clip: " << clip.name << endl;
            
            // TODO: check if clip has been temporarily removed (ie., deleted == true)
            // TODO: check dates and sizes or overide dates and sizes
            // actually maybe this just happens in the next check ??
            
        }else{
            
            // if the clip doesn't exist check if we have 
            // both an audio and a text file for this video
            
            if(audioFiles.getFileExists(name) && textFiles.getFileExists(name)){
                
                ofxLogNotice() << "ADDING clip: " << name << endl;
                
                // create a new clip
                
                Clip clip = Clip(name);
                clip.videoFile = videoFiles[name];
                clip.audioFile = audioFiles[name];
                clip.textFile = textFiles[name];
                clip.analyzed = false;
                clip.deleted = false;
                
                appModel->setClip(clip);
                
            }
            
        }
        
    }
    
    // reverse check against clips to see if a video has been removed
    
    for(int i = 0; i < appModel->getClips().size(); i++){
        
        Clip & clip = appModel->getClip(i);
        
        // init the clip so that category, question etc are all generated
        clip.init();
        
        // no need to check if this clip needs analysis and is not deleted
        if(clip.analyzed == false && clip.deleted == false){
            
            ofxLogNotice() << "Clip " << clip.name << " is just being ADDED" << endl;
            
            continue;
        }
        
        // check if Files exist
        if(!videoFiles.getFileExists(clip.name) || 
           !audioFiles.getFileExists(clip.name) || 
           !textFiles.getFileExists(clip.name)){
            
            // if any one of these files does not exist then
            // mark the clip as deleted in case we're just
            // temporarily removing the file
            
            ofxLogNotice() << "Clip " << clip.name << " marked for DELETION" << endl;
            
            clip.deleted = true;
            continue;
        }
        
        // check if date and size are the same (by using overload == operator
        
        // ... don't worry about text files - just accept the new one
        if(textFiles.getFile(clip.name) != clip.textFile){
            clip.textFile = textFiles[clip.name];
        }
        
        // ...but re-analyze if the audio or video are different
        if(videoFiles.getFile(clip.name) != clip.videoFile ||
           audioFiles.getFile(clip.name) != clip.audioFile){
            
            // if any of these have changed we need to re-analyze this clip
            // TODO: make checks so we only analyse if necessary...hmmm?
            
            clip.videoFile = videoFiles[clip.name];
            clip.audioFile = audioFiles[clip.name];
            
            ofxLogNotice() << "Clip " << clip.name << " marked for ANALYSIS" << endl;
            
            clip.analyzed = false;
        }
        
    }
    
    if(appModel->getNumClipsForAnalysis() > 0){
        appControllerStates.setState(kAPPCONTROLLER_ANALYZE);
        return;
    }else{
        appControllerStates.setState(kAPPCONTROLLER_PLAY);
    }
    
}