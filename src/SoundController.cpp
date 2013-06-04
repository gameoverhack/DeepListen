//
//  SoundController.cpp
//  DeepListen
//
//  Created by game over on 23/05/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "SoundController.h"

//--------------------------------------------------------------
SoundController::SoundController(){
    ofxLogVerbose() << "Creating AppController" << endl;
}

//--------------------------------------------------------------
SoundController::~SoundController(){
    ofxLogVerbose() << "Destroying AppController" << endl;
    stop();
    volumes.clear();
}

//--------------------------------------------------------------
bool SoundController::setup(int inChannels, int outChannels){
    
    if(ofxJackClient::setup((string)"SoundController", true)){
        
        ofxLogVerbose() << "Starting ofxJackClient" << endl;
        
        start();
        
        if(setNumChannels(inChannels, outChannels)){
            
            ofxLogVerbose() << "Created " << inChannels << " inChannels and " << outChannels << " outChannels" << endl;
            
            setAllChannelVolumes(1.0f);
            
            return true;
        }else{
            
            ofxLogError() << "Could not create " << inChannels << " inChannels and " << outChannels << " outChannels" << endl;
            
            return false;
        }
        
    }else{
        
        ofxLogError() << "Could not start ofxJackClient" << endl;
        assert(false);
        
        return false;
    }
    

}

//--------------------------------------------------------------
bool SoundController::setNumChannels(int inChannels, int outChannels){
    
    bool ok = false;
    
    for(int channel = 1; channel < outChannels + 1; channel++){
        ofxLogVerbose() << "Creating input: " << channel << " ..." << endl;
        ok = createPort("input" + ofToString(channel), JackPortIsInput);
        ofxLogVerbose() << "..." << (string)(ok ? "ok" : "failed") << endl;
        if(!ok) break;
        ofxLogVerbose() << "Creating putput: " << channel << " ..." << endl;
        ok = createPort("output" + ofToString(channel), JackPortIsOutput);
        ofxLogVerbose() << "..." << (string)(ok ? "ok" : "failed") << endl;
        if(!ok) break;
        ofxLogVerbose() << "Connecting input: " << appModel->getApplicationName() + ":output"+ofToString(channel)
                        << " to " << "SoundController:input" + ofToString(channel) << " ..." << endl;
        ok = connect(appModel->getApplicationName() + ":output"+ofToString(channel), "SoundController:input" + ofToString(channel));
        ofxLogVerbose() << "..." << (string)(ok ? "ok" : "failed") << endl;
        if(!ok) break;
        ofxLogVerbose() << "Connecting input: " << "SoundController:output" + ofToString(channel)
                        << " to " << "system:playback_" + ofToString(channel) << " ..." << endl;
        ok = connect("SoundController:output" + ofToString(channel), "system:playback_" + ofToString(channel));
        ofxLogVerbose() << "..." << (string)(ok ? "ok" : "failed") << endl;
        if(!ok) break;
    }
    
    if(ok){
        volumes.resize(inChannels);
        for(int channel = 0; channel < volumes.size(); channel++){
            volumes[channel].resize(outChannels);
        }
    }
    
    return ok;
}

//--------------------------------------------------------------
int SoundController::getNumInChannels(){
    return volumes.size();
}

//--------------------------------------------------------------
int SoundController::getNumOutChannels(int inChannel){
    return volumes[inChannel].size();
}

//--------------------------------------------------------------
void SoundController::setAllChannelVolumes(float volume){
    for(int inChannel = 0; inChannel < volumes.size(); inChannel++){
        for(int outChannel = 0; outChannel < volumes[inChannel].size(); outChannel++){
            volumes[inChannel][outChannel] = volume;
        }
    }
}

//--------------------------------------------------------------
bool SoundController::setChannelVolume(int inChannel, int outChannel, float volume){
    if(inChannel < getNumInChannels() && outChannel < getNumOutChannels(inChannel)){
        volumes[inChannel][outChannel] = volume;
    }else{
        ofLogError() << "Channel index out of bounds";
    }
}

//--------------------------------------------------------------
float SoundController::getChannelVolume(int inChannel, int outChannel){
    if(inChannel < getNumInChannels() && outChannel < getNumOutChannels(inChannel)){
        return volumes[inChannel][outChannel];
    }else{
        ofLogError() << "Channel index out of bounds";
    }
}

//--------------------------------------------------------------
int SoundController::process(jack_nframes_t nframes){
    
    for (int inChannel = 0; inChannel < volumes.size(); inChannel++) {
        
        jack_default_audio_sample_t *in;
        in = (jack_default_audio_sample_t *)jack_port_get_buffer(inPorts[inChannel], nframes);
        
        for(int outChannel = 0; outChannel < volumes[inChannel].size(); outChannel++){
            
            jack_default_audio_sample_t *out;
            out = (jack_default_audio_sample_t *)jack_port_get_buffer(outPorts[outChannel], nframes);
            
            for(jack_nframes_t i = 0; i < nframes; i++){
                out[i] = (out[i] + in[i] * volumes[inChannel][outChannel]) / 1.2; // magic number!
            }
        }
    }
    
    return 0;
}