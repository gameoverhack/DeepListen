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
        
        start();
        
        if(setNumChannels(inChannels, outChannels)){
            
            setAllChannelVolumes(1.0f);
            
            return true;
        }else{
            return false;
        }
        
    }else{
        return false;
    }
}

//--------------------------------------------------------------
bool SoundController::setNumChannels(int inChannels, int outChannels){
    
    bool ok = false;
    
    for(int channel = 1; channel < outChannels + 1; channel++){
        ok = createPort("input" + ofToString(channel), JackPortIsInput);
        if(!ok) break;
        ok = createPort("output" + ofToString(channel), JackPortIsOutput);
        if(!ok) break;
        ok = connect("emptyExampleDebu:out"+ofToString(channel), "SoundController:input" + ofToString(channel));
        if(!ok) break;
        ok = connect("SoundController:output" + ofToString(channel), "system:playback_" + ofToString(channel));
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
