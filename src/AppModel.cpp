//
//  AppModel.cpp
//  emptyExample
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "AppModel.h"

//--------------------------------------------------------------
AppModel::AppModel() {
    //ofxLogNotice() << "Constructing AppModel" << endl;
}

//--------------------------------------------------------------
AppModel::~AppModel() {
    ofxLogNotice() << "Destroying AppModel" << endl;
    clips.clear();
    reset();
}

//--------------------------------------------------------------
bool AppModel::save(string filename, ArchiveType archiveType){
    Serializer.saveClass(filename, (*this), archiveType);
    BaseModel::save(filename + "_props", archiveType);
}

//--------------------------------------------------------------
bool AppModel::load(string filename, ArchiveType archiveType){
    Serializer.loadClass(filename, (*this), archiveType);
    BaseModel::load(filename + "_props", archiveType);
}

//--------------------------------------------------------------
void AppModel::setClip(Clip clip){
    map<string, Clip>::iterator it = clips.find(clip.getName());
    if(it != clips.end()){
        ofxLogError() << "Clip already exists: " << clip.getName() << endl;
    }else{
        clips[clip.getName()] = clip;
    }
}

//--------------------------------------------------------------
Clip& AppModel::getClip(string name){
    map<string, Clip>::iterator it = clips.find(name);
    if(it != clips.end()){
        return it->second;
    }else{
        ofxLogError() << "Clip does not exist: " << name << endl;
        return dummyClip;
    }
}

//--------------------------------------------------------------
Clip& AppModel::getClip(int index){
    if(index < clips.size()){
        map<string, Clip>::iterator it = clips.begin();
        for(int i = 0; i < index; i++) it++;
        return it->second;
    }else{
        ofxLogError() << "Clip does not exist: " << index << endl;
        return dummyClip;
    }
}

//--------------------------------------------------------------
map<string, Clip>& AppModel::getClips(){
    return clips;
}

//--------------------------------------------------------------
bool AppModel::getClipExists(string name){
    map<string, Clip>::iterator it = clips.find(name);
    if(it != clips.end()){
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
void AppModel::setClipGroup(string clipGroupname, ClipGroup clipGroup){
    map<string, ClipGroup>::iterator it = clipGroups.find(clipGroupname);
    if(it != clipGroups.end()){
        ofxLogError() << "Clip group already exist: " << clipGroupname << endl;
    }else{
        clipGroups[clipGroupname] = clipGroup;
    }
}

//--------------------------------------------------------------
ClipGroup& AppModel::getClipGroupReference(string clipGroupname){
    map<string, ClipGroup>::iterator it = clipGroups.find(clipGroupname);
    if(it != clipGroups.end()){
        return it->second;
    }else{
        ofxLogError() << "Clip group ref doesn't exist: " << clipGroupname << endl;
        return dummyClipGroup;
    }
}

//--------------------------------------------------------------
ClipGroup AppModel::getClipGroup(string clipGroupname){
    map<string, ClipGroup>::iterator it = clipGroups.find(clipGroupname);
    if(it != clipGroups.end()){
        return it->second;
    }else{
        ofxLogError() << "Clip group doesn't exist: " << clipGroupname << endl;
        return dummyClipGroup;
    }
}

//--------------------------------------------------------------
bool AppModel::getClipGroupExists(string clipGroupname){
    map<string, ClipGroup>::iterator it = clipGroups.find(clipGroupname);
    if(it != clipGroups.end()){
        return true;
    }else{
        return false;
    }
}

//--------------------------------------------------------------
ClipTimeline& AppModel::getClipTimeline(){
    return clipTimeline;
}

//--------------------------------------------------------------
int AppModel::getNumClipsForAnalysis(){
    int count = 0;
    for(int i = 0; i < clips.size(); i++){
        Clip & clip = getClip(i);
        if(clip.getAnalyzed() == false && clip.getDeleted() == false) count++;
    }
    setProperty("AnalyzeClipNum", count);
    return count;
}

//--------------------------------------------------------------
Clip& AppModel::getClipForAnalysis(){
    for(int i = 0; i < clips.size(); i++){
        Clip & clip = getClip(i);
        if(clip.getAnalyzed() == false && clip.getDeleted() == false){
            setProperty("AnalyzeClip", clip.getName());
            return clip;
            break;
        }
    }
    ofxLogError() << "There are no clips to analyze - we should never get here!!" << endl;
    return dummyClip;
}

//--------------------------------------------------------------
ofVideoPlayer& AppModel::getAnalysisVideo(){
    return analysisVideo;
}

//--------------------------------------------------------------
ofxSamplePlayer& AppModel::getAnalysisAudio(){
    return analysisAudio;
}

//--------------------------------------------------------------
ofxCv::ContourFinder& AppModel::getAnalysisContourFinder(){
    return analysisContourFinder;
}

//--------------------------------------------------------------
FileList& AppModel::getAudioAssetLoader(){
    return audioAssets;
};

//--------------------------------------------------------------
FileList& AppModel::getVideoAssetLoader(){
    return videoAssets;
};

//--------------------------------------------------------------
FileList& AppModel::getTextAssetLoader(){
    return textAssets;
};