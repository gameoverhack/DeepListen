//
//  SoundController.h
//  DeepListen
//
//  Created by game over on 23/05/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

// Singleton definition
#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_
#include "assert.h"
#include <cstdlib>
template <class T>
class Singleton{
public:
    static T* Instance() {
        if(!m_pInstance) m_pInstance = new T;
        assert(m_pInstance !=NULL);
        return m_pInstance;
    };
protected:
    Singleton();
    ~Singleton();
private:
    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);
    static T* m_pInstance;
};
template <class T> T* Singleton<T>::m_pInstance=NULL;
#endif

#ifndef _H_SOUNDCONTROLLER
#define _H_SOUNDCONTROLLER

#include "Config.h"
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
	
    string instanceName;
    
    vector< vector<float> > volumes;
    
    float masterVolume;
    
private:
	
};

typedef Singleton<SoundController> SoundControllerSingleton;					// Global declaration

static SoundController * soundController = SoundControllerSingleton::Instance();

#endif
