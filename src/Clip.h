//
//  Clip.h
//  DeepListen
//
//  Created by game over on 2/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_CLIP
#define _H_CLIP

#include "ofMain.h"
#include "FileList.h"
#include "ofxSamplePlayer.h"
#include "ofxThreadedVideo.h"

enum SortType{
    NAME = 0,
    CATEGORY,
    QUESTION,
    TYPE,
    TIME,
    FRAMED,
    PERSONAL,
    NUMBER,
    PERSON
};

struct ClipPosition {
    ofRectangle position;
    int videostart;
    int videoend;
    int audiostart;
    int audioend;
    int screen;
    bool isLoading;
    bool isPlaying;
    bool isPaused;
};

class Clip {

public:
    
    Clip(){
        analyzed = deleted = false;
    };
    
    Clip(string _name){
        analyzed = deleted = false;
        name = _name;
        init();
    };
    
    ~Clip(){};

    void init(){
        vector<string> nameSplit = ofSplitString(name, "_");
        
        // make sure we have 4 chunks in the clip name
        assert(nameSplit.size() == 5);
        
        // make sure we have 4 letters in the first 3 chunks
        for(int i = 0; i < 3; i++){
            assert(nameSplit[i].size() == 4);
        }
        
        // make sure we got two characters for the number
        assert(nameSplit[3].size() == 2);
        
        // make sure we got actually got a name for the person
        assert(nameSplit[4].size() > 0);
        
        category =  nameSplit[0];
        question =  nameSplit[1];
        type =      nameSplit[2][0];
        time =      nameSplit[2][1];
        framed =    nameSplit[2][2];
        personal =  nameSplit[2][3];
        number =    ofToInt(nameSplit[3]);
        person =    nameSplit[4];
        
    };
    
    string name;
    string category;
    string question;
    string type;
    string time;
    string framed;
    string personal;
    string number;
    string person;
    
    File videoFile;
    File audioFile;
    File textFile;
    
    bool analyzed;
    bool deleted;
    
    string text;
    ofRectangle rect;
    
    int frames;
    float audioinpct;
    float audioutpct;

    ClipPosition clipPosition;
    
    bool operator!=(const Clip &rhs) {
        if(name != rhs.name){
            return true;
        }else{
            return false;
        }
    }
    
    bool operator==(const Clip &rhs) {
        if(name == rhs.name){
            return true;
        }else{
            return false;
        }
    }
    
    friend ostream& operator<< (ostream &os, Clip &c);
    
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
        ar & BOOST_SERIALIZATION_NVP(name);
		ar & BOOST_SERIALIZATION_NVP(videoFile);
        ar & BOOST_SERIALIZATION_NVP(audioFile);
        ar & BOOST_SERIALIZATION_NVP(textFile);
        ar & BOOST_SERIALIZATION_NVP(analyzed);
        ar & BOOST_SERIALIZATION_NVP(deleted);
        ar & BOOST_SERIALIZATION_NVP(text);
        ar & BOOST_SERIALIZATION_NVP(rect);
        ar & BOOST_SERIALIZATION_NVP(frames);
        ar & BOOST_SERIALIZATION_NVP(audioinpct);
        ar & BOOST_SERIALIZATION_NVP(audioutpct);
	};
    
protected:
    
private:
    
};

inline ostream& operator<<(ostream& os, Clip &c){
    os <<   c.name << " :position: " << 
            c.clipPosition.position.x << ":" << 
            c.clipPosition.position.y << ":" << 
            c.clipPosition.position.width << ":" << 
            c.clipPosition.position.height << ":" <<
            c.clipPosition.screen << " frames: " << 
            c.clipPosition.videostart << ":" <<
            c.clipPosition.audiostart << ":" <<
            c.clipPosition.audioend << ":" <<
            c.clipPosition.videoend << ":" <<
            c.frames;
    return os;
};

class ClipType{
    
public:
    
    ClipType(){};
    
    ~ClipType(){
        clear();
    }
    
    void push(string value){
        types[value]++; 
    }
    
    void pop(string value){
        if(types.find(value) != types.end()){
            if(types[value] - 1 == 0){
                types.erase(types.find(value));
            }else{
                types[value]--;
            }
        }
    }
    
    void popall(string value){
        if(types.find(value) != types.end()){
            types.erase(types.find(value));
        }
    }
    
    string poprandom(){
        string s;
        int r = ofRandom(types.size());
        int count = 0;
        for(map<string, int>::iterator it = types.begin(); it != types.end(); it++, count++){
            if(count == r){
                s = it->first;
                break;
            }
        }
        popall(s);
        return s;
    }
    
    int count(string value){
        if(types.find(value) != types.end()){
            return types[value];
        }else{
            return 0;
        }
    }
    
    int countunique(){
        return types.size();
    }
    
    map<string, int> & getTypes(){
        return types;
    }
    
    void clear(){
        types.clear();
    }
    
    friend ostream& operator<< (ostream &os, ClipType &ct);
    
protected:
    
    map<string, int> types;
    
};

inline ostream& operator<<(ostream& os, ClipType &ct){
    for(map<string, int>::iterator it = ct.getTypes().begin(); it != ct.getTypes().end(); it++){
        os << it->first << " : " << it->second << endl;
    }
    return os;
};

class ClipGroup{
    
public:
    
    ClipGroup(){};
    ~ClipGroup(){
        clear();
    }
    ClipGroup getContains(SortType type, string value){
        ClipGroup clipGroup;
        for(int i = 0; i < group.size(); i++){
            switch (type) {
                case NAME:
                {
                    if(group[i].name == value) clipGroup.push(group[i]);
                }
                    break;
                case CATEGORY:
                {
                    if(group[i].category == value) clipGroup.push(group[i]);
                }
                    break;
                case QUESTION:
                {
                    if(group[i].question == value) clipGroup.push(group[i]);
                }
                    break;
                case TYPE:
                {
                    if(group[i].type == value) clipGroup.push(group[i]);
                }
                    break;
                case TIME:
                {
                    if(group[i].time == value) clipGroup.push(group[i]);
                }
                    break;
                case FRAMED:
                {
                    if(group[i].framed == value) clipGroup.push(group[i]);
                }
                    break;
                case PERSONAL:
                {
                    if(group[i].personal == value) clipGroup.push(group[i]);
                }
                    break;
                case NUMBER:
                {
                    if(ofToInt(group[i].number) == ofToInt(value)) clipGroup.push(group[i]);
                }
                    break;
                case PERSON:
                {
                    if(group[i].name == value) clipGroup.push(group[i]);
                }
                    break;
            }
        }
        return clipGroup;
    }
    
    ClipGroup getExcludes(SortType type, string value){
        ClipGroup clipGroup;
        for(int i = 0; i < group.size(); i++){
            switch (type) {
                case NAME:
                {
                    if(group[i].name != value) clipGroup.push(group[i]);
                }
                    break;
                case CATEGORY:
                {
                    if(group[i].category != value) clipGroup.push(group[i]);
                }
                    break;
                case QUESTION:
                {
                    if(group[i].question != value) clipGroup.push(group[i]);
                }
                    break;
                case TYPE:
                {
                    if(group[i].type != value) clipGroup.push(group[i]);
                }
                    break;
                case TIME:
                {
                    if(group[i].time != value) clipGroup.push(group[i]);
                }
                    break;
                case FRAMED:
                {
                    if(group[i].framed != value) clipGroup.push(group[i]);
                }
                    break;
                case PERSONAL:
                {
                    if(group[i].personal != value) clipGroup.push(group[i]);
                }
                    break;
                case NUMBER:
                {
                    if(ofToInt(group[i].number) != ofToInt(value)) clipGroup.push(group[i]);
                }
                    break;
                case PERSON:
                {
                    if(group[i].name != value) clipGroup.push(group[i]);
                }
                    break;
            }
        }
        return clipGroup;
    }
    
    ClipGroup getUnique(SortType type){
        ClipGroup cg;
        map<string, int> uniquetypes = getClipTypes(type).getTypes();
        for(map<string, int>::iterator it = uniquetypes.begin(); it != uniquetypes.end(); it++){
            ClipGroup subGroup = getContains(type, it->first);
            cg.push(subGroup[ofRandom(subGroup.size())]);
        }
        return cg;
    }
    
    ClipGroup getUnique(SortType type, int numClips){
        ClipGroup cg;
        ClipGroup clips = getUnique(type);
        for(int i = 0; i < numClips; i++){
            if(clips.size() > 0){
                Clip clip = clips.poprandom();
                cg.push(clip);
            }
        }
        return cg;
    }
    
    ClipType getClipTypes(SortType type){
        switch (type) {
            case NAME:
            {
                return nameTypes;
            }
                break;
            case CATEGORY:
            {
                return categoryTypes;
            }
                break;
            case QUESTION:
            {
                return questionTypes;
            }
                break;
            case TYPE:
            {
                return typeTypes;
            }
                break;
            case TIME:
            {
                return timeTypes;
            }
                break;
            case FRAMED:
            {
                return framedTypes;
            }
                break;
            case PERSONAL:
            {
                return personalTypes;
            }
                break;
            case PERSON:
            {
                return nameTypes;
            }
                break;
        }
    }
    
    void push(Clip & clip){
        group.push_back(clip);
        categoryTypes.push(clip.category);
        questionTypes.push(clip.question);
        typeTypes.push(clip.type);
        timeTypes.push(clip.time);
        framedTypes.push(clip.framed);
        personalTypes.push(clip.personal);
        personTypes.push(clip.name);
        nameTypes.push(clip.name);
    }
    
    void push(string & clipName){
        Clip clip = Clip(clipName);
        push(clip);
    }
    
    void push(vector<string> & clipnames){
        for(int i = 0; i < clipnames.size(); i++){
            push(clipnames[i]);
        }
    }
    
    void push(vector<Clip> & clips){
        for(int i = 0; i < clips.size(); i++){
            push(clips[i]);
        }
    }
    
    void push(ClipGroup & clips){
        for(int i = 0; i < clips.size(); i++){
            push(clips[i]);
        }
    }
    
    Clip pop(Clip & clip){
        for(int i = 0; i < group.size(); i++){
            if(group[i] == clip){
                
                group.erase(group.begin()+i, group.begin()+i+1);
                
                categoryTypes.pop(clip.category);
                questionTypes.pop(clip.question);
                typeTypes.pop(clip.type);
                timeTypes.pop(clip.time);
                framedTypes.pop(clip.framed);
                personalTypes.pop(clip.personal);
                personTypes.pop(clip.name);
                nameTypes.pop(clip.name);
                
            }
        }
        return clip;
    }
    
    Clip pop(string & clipName){
        Clip clip = Clip(clipName);
        return pop(clip);
    }
    
    ClipGroup pop(vector<string> & clipnames){
        ClipGroup cg;
        for(int i = 0; i < clipnames.size(); i++){
            Clip clip = pop(clipnames[i]);
            cg.push(clip);
        }
        return cg;
    }
    
    ClipGroup pop(vector<Clip> & clips){
        ClipGroup cg;
        for(int i = 0; i < clips.size(); i++){
            Clip clip = pop(clips[i]);
            cg.push(clip);
        }
        return cg;
    }
    
    ClipGroup pop(ClipGroup & clipGroup){
        ClipGroup cg;
        for(int i = 0; i < clipGroup.size(); i++){
            Clip clip = pop(clipGroup[i]);
            cg.push(clip);
        }
        return cg;
    }
    
    Clip poprandom(){
        Clip clip;
        if(group.size() > 0){
            clip = group[ofRandom(group.size())];
            pop(clip);
        }
        
        return clip;
    }
    
    void shuffle(){
        random_shuffle(group.begin(), group.end());
    }
    
    void getClipNamesAt(int frame, vector<string>& clipNames){
        for(int j = 0; j < group.size(); j++){
            Clip & clip = group[j];
            if(frame >= clip.clipPosition.videostart && frame <= clip.clipPosition.videoend){
                clipNames.push_back(clip.name);
            }
        }
    };
    
    Clip & getClip(string name){
        for(int j = 0; j < group.size(); j++){
            if(group[j].name == name){
                return group[j];
            }
        }
        ofxLogError() << "Clip out of bounds with name: " << name;
        return dummyClip;
    }
    
    int size(){
        return group.size();
    }
    
    void clear(){
        group.clear();
        categoryTypes.clear();
        questionTypes.clear();
        typeTypes.clear();
        timeTypes.clear();
        framedTypes.clear();
        personalTypes.clear();
        personTypes.clear();
        nameTypes.clear();
    }
    
    Clip & operator[](int i){
        return group[i];
    }
    
    friend ostream& operator<< (ostream &os, ClipGroup &cg);
    
protected:
    
    vector<Clip> group;
    Clip dummyClip;
    
    ClipType categoryTypes;
    ClipType questionTypes;
    ClipType typeTypes;
    ClipType timeTypes;
    ClipType framedTypes;
    ClipType personalTypes;
    ClipType personTypes;
    ClipType nameTypes;
    
};

inline ostream& operator<<(ostream& os, ClipGroup &cg){
    for(int i = 0; i < cg.size(); i++){
        os << cg[i] << endl;
    }
    return os;
};

class ClipTimeline {
    
public:
    
    ClipTimeline(){
        totalframes = 0;
        currentframe = -2000;
    };
    
    ~ClipTimeline(){
        clear();
    };
    
    void setup(int numVideos, ofPixelFormat _pixelFormat){
        
        pixelFormat = _pixelFormat;
        
        for(int i = 0; i < numVideos; i++){
            
            ofxThreadedVideo * video = new ofxThreadedVideo();
            ofAddListener(video->threadedVideoEvent, this, &ClipTimeline::threadedVideoEvent);
            video->setPixelFormat(pixelFormat);
            video->setLoopState(OF_LOOP_NONE);
            video->setUseQueue(true);
            videos.push_back(video);
            
        }
        
        if(pixelFormat == OF_PIXELS_2YUV){
            
            shader.load(ofToDataPath("yuyvtorgba"));
            render.allocate(1920, 1080);
            
        }
        
    }
    
    void clear(){
        
        for(int i = 0; i < videos.size(); i++){
            ofxThreadedVideo * video = videos[i];
            video->stop();
        }
        
        totalframes = 0;
        currentframe = -2000;
        groups.clear();
        currentClipNames.clear();
        startFrames.clear();
        
    }
    
    void update(){
        
        if(currentframe == -2000) currentframe = groups[0][0].clipPosition.videostart; // bad form?
        
        for(map<int, vector<string> >::reverse_iterator it = startFrames.rbegin(); it != startFrames.rend(); ++it){
            if(currentframe >= it->first){
                currentClipNames = it->second;
                break;
            }
        }

        for(int i = 0; i < videos.size(); i++){
            
            ofxThreadedVideo * video = videos[i];
            video->update();
            
            if(video->getIsMovieDone()){
                
                Clip & clip = getClipFromPath(video->getPath());
                
                //ofxLogVerbose() << "Stop clip ( " << i << " ): " << clip.name << endl;
                
                video->stop();
                
                clip.clipPosition.isLoading = false;
                clip.clipPosition.isPlaying = false;
                clip.clipPosition.isPaused = false;
                
            }
            
            if(video->isPlaying()){
                Clip & clip = getClipFromPath(video->getPath());
                currentframe = clip.clipPosition.videostart + video->getCurrentFrame();
                syncClipName = clip.name;
            }
            
        }
        
        for(int i = 0; i < currentClipNames.size(); i++){
            
            Clip & clip = getClip(currentClipNames[i]);
            
            if(currentframe >= clip.clipPosition.videostart &&
               currentframe <= clip.clipPosition.videostart + 10 &&
               !clip.clipPosition.isLoading && 
               !clip.clipPosition.isPlaying){
                
                ofxLogVerbose() << "Load clip ( " << i << " ): " << clip.name << endl;
                
                ofxThreadedVideo * video = getFreeVideoPlayer();
                
                if(video->loadMovie(clip.videoFile.path)){
                    video->update();
                    clip.clipPosition.isLoading = true;
                    clip.clipPosition.isPlaying = false;
                    clip.clipPosition.isPaused = false;
                }
            }
        }
        
    }
    
    void threadedVideoEvent(ofxThreadedVideoEvent & e){
        ofxLogVerbose() << "VideoEvent: " << e.video->getEventTypeAsString(e.eventType) << " for " << e.video->getPath() << endl;
        Clip & clip = getClipFromPath(e.video->getPath());
        switch(e.eventType){
            case VIDEO_EVENT_LOAD_OK:
            {
                ofxLogNotice() << "Loaded clip: " << clip.name << endl;
                
                e.video->setLoopState(OF_LOOP_NONE);

                clip.clipPosition.isLoading = false;
                clip.clipPosition.isPlaying = true;
                clip.clipPosition.isPaused = false;
                
//                if(currentframe - clip.clipPosition.videostart > 2){
//                    cout << "SEEKING" << endl;
//                    e.video->setFrame(currentframe - clip.clipPosition.videostart);
//                }

            }
                break;
            default:
                
                ofxLogError() << "Could not load clip: " << clip.name << endl;
                
                clip.clipPosition.isLoading = false;
                clip.clipPosition.isPlaying = false;
                clip.clipPosition.isPaused = false;
                
                break;
        }
    }
    
    void draw(int screen){
        
        for(int i = 0; i < videos.size(); i++){
            
            ofxThreadedVideo * video = videos[i];
            
            if(video->isPlaying()){
                
                Clip & clip = getClipFromPath(video->getPath());
                
                if(clip.clipPosition.screen == screen){
                    
                    if(video->getPixelFormat() == OF_PIXELS_2YUV){
                        
                        render.begin();
                        ofClear(0.0f, 0.0f, 0.0f, 1.0f);
                        render.end();
                        shader.begin();
                        shader.setUniformTexture("yuvTex", video->getTextureReference(), 1);
                        shader.setUniform1i("conversionType", (false ? 709 : 601));
                        render.draw(clip.clipPosition.position.x, clip.clipPosition.position.y);
                        shader.end();
                        
                    }else{
                        
                        video->draw(clip.clipPosition.position.x, clip.clipPosition.position.y);
                        
                    }
                }
            }
            
        }
    }
    
    void drawTimeline(){
        
        glPushMatrix();
        
        float framescale = (1.0/totalframes) * 1920.0f * 32;
        float pixelscale = (1.0f/1920.0f) * 1080.0f/2.0f;
        
        glScalef(framescale, pixelscale, 1.0f);
        
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofLine(2000, 0, 2000, 1920.0f * 2.0f);
        ofTranslate(2000 - currentframe, 800.0f, 0.0f);
        
        for(int i = 0; i < groups.size(); i++){
            
            ClipGroup & group = groups[i];
            for(int j = 0; j < group.size(); j++){
                
                Clip & clip = group[j];
                
                ofFill();
                if(clip.name == syncClipName){
                    ofSetColor(64, 64, 128, 64);
                    ofRect(clip.clipPosition.videostart,
                           clip.clipPosition.position.x + (clip.clipPosition.screen * 1920.0f),
                           clip.frames, 
                           clip.clipPosition.position.width);
                    ofSetColor(0, 0, 255);
                }else{
                    ofSetColor(64, 64, 64, 64);
                    ofRect(clip.clipPosition.videostart,
                           clip.clipPosition.position.x + (clip.clipPosition.screen * 1920.0f),
                           clip.frames, 
                           clip.clipPosition.position.width);
                    ofSetColor(255, 255, 255);
                    
                }
                
                ofNoFill();
                ofRect(clip.clipPosition.videostart,
                       clip.clipPosition.position.x + (clip.clipPosition.screen * 1920.0f),
                       clip.frames, 
                       clip.clipPosition.position.width);
                ofSetColor(255, 0, 0);
                ofRect(clip.clipPosition.audiostart,
                       clip.clipPosition.position.x + (clip.clipPosition.screen * 1920.0f),
                       clip.clipPosition.audioend - clip.clipPosition.audiostart, 
                       clip.clipPosition.position.width);
                ofSetColor(255, 255, 255);
                
            }
        }
        
        glPopMatrix();
        
    }
    
    void play(){
        
    }
    
    void pause(){
        
    }
    
    void setFrame(int frame){
        
    }
    
    int getCurrentFrame(){
        return currentframe;
    }
    
    int getTotalFrames(){
        return totalframes;
    }
    
    int updateTotalFrames(){
        int tframes = 0;
        for(int i = 0; i < groups.size(); i++){
            ClipGroup & group = groups[i];
            int grouptotalframes = group[0].frames;
            for(int j = 1; j < group.size(); j++){
                grouptotalframes = grouptotalframes - (group[j-1].clipPosition.videoend - group[j].clipPosition.videostart) + group[j].frames;
            }
            tframes = MAX(totalframes, grouptotalframes);
        }
        return tframes;
    }
    
    int getNextStartTime(){
        for(map<int, vector<string> >::reverse_iterator it = startFrames.rbegin(); it != startFrames.rend(); ++it){
            if(currentframe >= it->first){
                return it->first;
                break;
            }
        }
        return 0; // should never get here
    }
    
    void updateStartFrames(){
        startFrames.clear();
        totalframes = updateTotalFrames();
        for(int i = 0; i < groups.size(); i++){
            ClipGroup & group = groups[i];
            for(int j = 0; j < group.size(); j++){
                Clip & clip = group[j];
                map<int, vector<string> >::iterator it = startFrames.find(clip.clipPosition.videostart);
                if(it != startFrames.end()){
                    getClipNamesAt(clip.clipPosition.videostart, it->second);
                    //it->second.push_back(clip.name);
                }else{
                    vector<string> v;
                    getClipNamesAt(clip.clipPosition.videostart, v);
                    //v.push_back(clip.name);
                    startFrames[clip.clipPosition.videostart] = v;
                }
            }
        }
    }
    
    ofxThreadedVideo* getFreeVideoPlayer(){
        for(int i = 0; i < videos.size(); i++){
            ofxThreadedVideo * video = videos[i];
            if(!video->isLoading() && !video->isPlaying()){
                return video;
            }
        }
        
        ofxLogWarning() << "ADDING A NEW VIDEO PLAYER -> did something go wrong?!" << endl;
        
        ofxThreadedVideo * video = new ofxThreadedVideo();
        ofAddListener(video->threadedVideoEvent, this, &ClipTimeline::threadedVideoEvent);
        video->setPixelFormat(pixelFormat);
        video->setLoopState(OF_LOOP_NONE);
        video->setUseQueue(true);
        videos.push_back(video);
        
        return video;
    }
    
    void getClipNamesAt(int frame, vector<string>& clipNames){
        for(int i = 0; i < groups.size(); i++){
            ClipGroup & group = groups[i];
            groups[i].getClipNamesAt(frame, clipNames);
        }
    };
    
    void push(ClipGroup & group){
        ofxLogVerbose() << "Adding group to clip timeline with " << group.size() << " clips" << endl;
        groups.push_back(group);
        updateStartFrames();
    }
    
    Clip & getClip(string name){
        for(int i = 0; i < groups.size(); i++){
            ClipGroup & group = groups[i];
            for(int j = 0; j < group.size(); j++){
                if(group[j].name == name){
                    return group[j];
                }
            }
        }
        ofxLogError() << "Clip out of bounds with name: " << name;
        return dummyClip;
    }
    
    Clip & getClipFromPath(string path){
        vector<string> pathParts = ofSplitString(path, "/");
        return getClip(ofSplitString(pathParts[pathParts.size() - 1], ".")[0]);
    }
    
    vector<string>& getCurrentClipNames(){
        return currentClipNames;
    }
    
    vector<ClipGroup>& getClipGroups(){
        return groups;
    };
    
    ClipGroup& getClipGroup(int index){
        return groups[index];
    };
    
    vector<ofxThreadedVideo*>& getVideos(){
        return videos;
    };
    
protected:
    
    ofShader shader;
    ofFbo render;
    ofPixelFormat pixelFormat;
    
    map<int, vector<string> > startFrames;
    vector<string> currentClipNames;
    
    vector<ClipGroup> groups;
    vector<ofxThreadedVideo*> videos;
    
    int lastFrameTime;
    string syncClipName;
    
    Clip dummyClip;
    
    bool bIsPlaying;
    
    int totalframes;
    int currentframe;
    
};

#endif