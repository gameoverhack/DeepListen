//
//  SoundController.h
//  DeepListen
//
//  Created by game over on 23/05/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_SOUNDCONTROLLER
#define _H_SOUNDCONTROLLER
#define NO_SOUND
#include "ofxLogger.h"
#include "BaseController.h"
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
    void setAllChannelVolumes(int inChannel, float volume);
    
    bool setChannelVolume(int inChannel, int outChannel, float volume);
    float getChannelVolume(int inChannel, int outChannel);
    
    int process(jack_nframes_t nframes);
    
    ofPoint getPan(float value, float maxValue, int numSpeakers);
    int getChannelLabel(int channel);
    
    void setMasterVolume(float volume);
    float getMasterVolume();
    
protected:
	
    vector< vector<float> > volumes;
    
    float masterVolume;
    
private:
	
};

typedef Singleton<SoundController> SoundControllerSingleton;					// Global declaration

static SoundController * soundController = SoundControllerSingleton::Instance();

#endif
