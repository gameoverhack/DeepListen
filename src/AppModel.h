//
//  AppModel.h
//
//  Created by game over on 26/02/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_APPMODEL
#define _H_APPMODEL

#include "Config.h"
#include "BaseModel.h"
#include "AppStates.h"
#include "FileList.h"
#include "Clip.h"

#include "ofxCv.h"

#define MAX_VIDEOS 4

class AppModel: public BaseModel {
    
public:
    
	AppModel();
    ~AppModel();

    // clip functions
    void setClip(Clip clip);
    Clip& getClip(string name);
    Clip& getClip(int index);
    
    map<string, Clip>& getClips();
    bool getClipExists(string name);
    
    // clip group functions
    void setClipGroup(string clipGroupname, ClipGroup clipGroup);
    ClipGroup& getClipGroupReference(string clipGroupname);
    ClipGroup getClipGroup(string clipGroupname);
    bool getClipGroupExists(string clipGroupname);
    
    ClipTimeline& getClipTimeline();
    
    // timer functions
    void setTimer(string timerName, int timeMillis);
    bool getTimer(string timerName, int timeMillis, int timeOutMillis);
    int getTimerDifference(string timerName, int timeMillis);
    
    // analysis functions
    int getNumClipsForAnalysis();
    Clip& getClipForAnalysis();
    
    ofVideoPlayer& getAnalysisVideo();
    ofxSamplePlayer& getAnalysisAudio();
    ofxCv::ContourFinder & getAnalysisContourFinder();
    
    FileList& getAudioAssetLoader();
    FileList& getVideoAssetLoader();
    FileList& getTextAssetLoader();
    
    bool save(string filname, ArchiveType archiveType);
    bool load(string filname, ArchiveType archiveType);
    
protected:
    
    ClipTimeline            clipTimeline;
    
    ofVideoPlayer           analysisVideo;
    ofxSamplePlayer         analysisAudio;
    ofxCv::ContourFinder    analysisContourFinder;
    
    map<string, Clip>       clips;
    Clip                    dummyClip;
    
    map<string, ClipGroup>  clipGroups;
    ClipGroup               dummyClipGroup;
    
    map<string, int>        timers;
    
    FileList audioAssets;
    FileList videoAssets;
    FileList textAssets;
    
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & BOOST_SERIALIZATION_NVP(clips);
	};
    
private:
	
};

namespace boost {
    namespace serialization {
        
        template<class Archive>
        void serialize(Archive & ar, MiniMaxima & m, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(m.minL);
            ar & BOOST_SERIALIZATION_NVP(m.minR);
            ar & BOOST_SERIALIZATION_NVP(m.maxL);
            ar & BOOST_SERIALIZATION_NVP(m.maxR);
        };
        
        template<class Archive>
        void serialize(Archive & ar, ofRectangle & r, const unsigned int version) {
            ar & BOOST_SERIALIZATION_NVP(r.x);
            ar & BOOST_SERIALIZATION_NVP(r.y);
            ar & BOOST_SERIALIZATION_NVP(r.height);
            ar & BOOST_SERIALIZATION_NVP(r.width);
        };
    };
};


typedef Singleton<AppModel> AppModelSingleton;					// Global declaration

static AppModel * appModel	= AppModelSingleton::Instance();

#endif
