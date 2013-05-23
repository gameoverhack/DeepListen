//
//  SoundController.h
//  DeepListen
//
//  Created by game over on 23/05/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_SOUNDCONTROLLER
#define _H_SOUNDCONTROLLER

#include "BaseController.h"
#include "AppModel.h"
#include "PlayController.h"

#include "ofxJackClient.h"

class SoundController : public BaseController, public ofxJackClient {
    
public:
	
    SoundController();
    ~SoundController();
    
    bool setup(int inChannels = 8, int outchannels = 8);
    
    bool setNumChannels(int inChannels, int outchannels);
    
    int getNumInChannels();
    int getNumOutChannels(int inChannel);
    
    void setAllChannelVolumes(float volume);
    void setAllChannelOutVolumes(int inChannel, float volume);
    
    bool setChannelVolume(int inChannel, int outChannel, float volume);
    float getChannelVolume(int inChannel, int outChannel);
    
    int process(jack_nframes_t nframes);
    
protected:
	
    vector< vector<float> > volumes;
    
private:
	
};

#endif
