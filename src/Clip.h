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

static string getTypeFromCode(string code){
    if(code == "F") return "Factual";
    if(code == "A") return "Anecdotal";
    if(code == "D") return "Descriptive";
    if(code == "X") return "Experiential";
    if(code == "E") return "Educative";
    if(code == "M") return "Emotive";
    if(code == "O") return "Other";
};

static string getFrameFromCode(string code){
    if(code == "L") return "Land";
    if(code == "P") return "Political";
    if(code == "R") return "Personal";
    if(code == "S") return "Spiritual";
    if(code == "C") return "Cultural";
    if(code == "A") return "All";
    if(code == "O") return "Other";
};

static string getTimeFromCode(string code){
    if(code == "N") return "Now";
    if(code == "P") return "Past";
    if(code == "H") return "Historical";
    if(code == "A") return "Ancestral";
    if(code == "C") return "Comparative";
    if(code == "O") return "Other";
};

static string getPersonalFromCode(string code){
    if(code == "A") return "Abstract";
    if(code == "S") return "Self";
    if(code == "F") return "FewOthers";
    if(code == "M") return "ManyOthers";
    if(code == "R") return "OurPeople";
    if(code == "V") return "ManyPeopleVisible";
    if(code == "O") return "Other";
};

static string getCategoryFromCode(string code){
    if(code == "INTR") return "Intro";
    if(code == "CNTY") return "Country";
    if(code == "IDEN") return "Identity";
    if(code == "RITE") return "Rights";
    if(code == "FAML") return "Family";
    if(code == "CULT") return "Culture";
    if(code == "STAT") return "Statement";
    if(code == "SPEC") return "Special";
    if(code == "LSTN") return "Listen";
};

static string getIntroQuestionFromCode(string code){
    if(code == "NAME") return "What is your name?";
    if(code == "NMCL") return "What is your name and language/clan affiliation?";
    if(code == "CLAN") return "What is your language/clan affiliation?";
    if(code == "ABOT") return "Tell us a bit about yourself…";
};

static string getCountryQuestionFromCode(string code){
    if(code == "TTLE") return "Title clip";
    if(code == "WHER") return "Where is your country?";
    if(code == "DESC") return "Can you describe your country to me?";
    if(code == "LOOK") return "What does it look like?";
    if(code == "SPCN") return "Something specific about your country?";
    if(code == "FEEL") return "How does it make you feel being on country?";
    if(code == "CNCT") return "What is (your) connection to country?";
    if(code == "CNMN") return "Tell me what connection to country means to you?";
    if(code == "IMPT") return "Why is where you're from so important?";
    if(code == "PLDE") return "Can describe a particular place of importance?";
    if(code == "EXCU") return "Explain clans / tribes / nations";
};

static string getIdentityQuestionFromCode(string code){
    if(code == "TTLE") return "Title clip";
    if(code == "HOWI") return "How do you identify yourself?";
    if(code == "MEAN") return "What does 'identity' mean to you?";
    if(code == "EXNO") return "How would you explain Aboriginal identity to a non-indigenous person?";
    if(code == "DNTL") return "What would you say to someone if they said 'You don't look Aboriginal'?";
    if(code == "CMMN") return "What do you think are the most comman misconceptions/stereotypes held about Aboriginal people?";
    if(code == "PRWD") return "Can you tell us a time when you felt proud of your identity?";
};

static string getRightsQuestionFromCode(string code){
    if(code == "ADIR") return "Can you tell us about advocay for indigenous rights?";
    if(code == "TTLE") return "Title clip";
    if(code == "FEEL") return "When you think about indigenous rights how do you feel?";
    if(code == "STRG") return "Can you tell us about a time or event when you felt indigenous rights were strengthened?";
    if(code == "UNDR") return "Can you tell us about a time or event when you felt indigenous rights were undermined?";
    if(code == "WTDO") return "What do you think non-aboriginal can do about inidgenous rights?";
    if(code == "DFNT") return "Definition of treaty, soveriegnty, terra nullus...";
};

static string getFamilyQuestionFromCode(string code){
    if(code == "TTLE") return "Title clip";
    if(code == "NMPS") return "Can you name as people in your family as possible?";
    if(code == "ROLE") return "What role do ancestors play in your life?";
    if(code == "DIFF") return "How are Aboriginal families different to other/mainstream/white families?";
    if(code == "IMPT") return "Who in your family has been the most important / influential in your life?";
    if(code == "LOVE") return "Can you tell us about falling in love or feeling the love?";
    if(code == "FMTU") return "What does family mean to you?";
    if(code == "DWIF") return "Can you describe your family?";
    if(code == "STOL") return "What is the stolen generation?";
    if(code == "HIER") return "Explain hierarachy, ancesters, elders and learning";
};

static string getCultureQuestionFromCode(string code){
    if(code == "TTLE") return "Title clip";
    if(code == "MEAN") return "What does culture mean to you?";
    if(code == "CNCT") return "How do you maintain or connect to your traditional or ancestral culture?";
    if(code == "SPVC") return "What is special or unique about Victorian Aboriginal cultures?";
    if(code == "LRNE") return "How do you learn about culture?";
    if(code == "ALIV") return "How do you keep culture alive?";
    if(code == "WORD") return "Can you say words in your langauge?";
    if(code == "LARO") return "What role does language play in your life?";
    if(code == "PRTO") return "What does respect/protocol play in culture?";
};

static string getStatementQuestionFromCode(string code){
    if(code == "CNIS") return "Country is...";
    if(code == "IDIS") return "Identity is...";
    if(code == "RIIS") return "Resilience is...";
    if(code == "FAIS") return "Family is...";
    if(code == "CUIS") return "Culture is...";
    if(code == "CCIS") return "Connection is...";
};

static string getSpecialQuestionFromCode(string code){    
    if(code == "DANC") return "Dance";
    if(code == "SONG") return "Song";
    if(code == "SNDN") return "Song and dance";
    if(code == "ITEM") return "Special item";
};

static string getListenQuestionFromCode(string code){    
    if(code == "LFPR") return "Left Profile";
    if(code == "LFTR") return "Left Three Quarter";
    if(code == "FRON") return "Front";
    if(code == "RIPR") return "Right Profile";
    if(code == "RITR") return "Right Three Quarter";
};

static string getQuestionFromCode(string categoryCode, string questionCode){
    if(categoryCode == "INTR") return getIntroQuestionFromCode(questionCode);
    if(categoryCode == "CNTY") return getCountryQuestionFromCode(questionCode);
    if(categoryCode == "IDEN") return getIdentityQuestionFromCode(questionCode);
    if(categoryCode == "RITE") return getRightsQuestionFromCode(questionCode);
    if(categoryCode == "FAML") return getFamilyQuestionFromCode(questionCode);
    if(categoryCode == "CULT") return getCultureQuestionFromCode(questionCode);
    if(categoryCode == "STAT") return getStatementQuestionFromCode(questionCode);
    if(categoryCode == "SPEC") return getSpecialQuestionFromCode(questionCode);
    if(categoryCode == "LSTN") return getListenQuestionFromCode(questionCode);
};

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
    int videoframes;
    int audiostart;
    int audioend;
    int screen;
    int cropstart;
    int cropend;
    int cropframes;
    bool isCropped;
    bool isStopping;
    bool isLoading;
};

struct ClipInfo{
    string category;
    string question;
    string type;
    string time;
    string framed;
    string personal;
    string number;
    string person;
    string readablecategory;
    string readablequestion;
    string readabletype;
    string readabletime;
    string readableframed;
    string readablepersonal;
    string readablenumber;
    string readableperson;
};

class Clip {
    
public:
    
    Clip(){
        clear();
    };
    
    Clip(string _name){
        clear();
        name = _name;
        init();
    };
    
    ~Clip(){
        clear();
    };
    
    void clear(){
        name = "";
        frames = -1;
        audioinpct = 0;
        audioutpct = 0;
        analyzed = deleted = false;
        width = height = 0.0f;
        scale = 1.0f;
        
        clipPosition.position = ofRectangle(-1, -1, -1, -1);
        clipPosition.videostart = -1;
        clipPosition.videoend = -1;
        clipPosition.videoframes = -1;
        clipPosition.audiostart = -1;
        clipPosition.audioend = -1;
        clipPosition.screen = -1;
        clipPosition.cropstart = -1;
        clipPosition.cropend = -1;
        clipPosition.cropframes = -1;
        clipPosition.isCropped = false;
        clipPosition.isStopping = false;
        clipPosition.isLoading = false;
        
        clipInfo.category = "";
        clipInfo.question = "";
        clipInfo.type = "";
        clipInfo.time = "";
        clipInfo.framed = "";
        clipInfo.personal = "";
        clipInfo.number = "";
        clipInfo.person = "";
        
        clipInfo.readablecategory = "";
        clipInfo.readablequestion = "";
        clipInfo.readabletype = "";
        clipInfo.readabletime = "";
        clipInfo.readableframed = "";
        clipInfo.readablepersonal = "";
    }
    
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
        
        clipInfo.category =  nameSplit[0];
        clipInfo.question =  nameSplit[1];
        clipInfo.type =      nameSplit[2][0];
        clipInfo.time =      nameSplit[2][1];
        clipInfo.framed =    nameSplit[2][2];
        clipInfo.personal =  nameSplit[2][3];
        clipInfo.number =    ofToInt(nameSplit[3]);
        clipInfo.person =    nameSplit[4];
        
        clipInfo.readablecategory = getCategoryFromCode(clipInfo.category);
        clipInfo.readablequestion = getQuestionFromCode(clipInfo.category, clipInfo.question);
        clipInfo.readabletype =     getTypeFromCode(clipInfo.type);
        clipInfo.readabletime =     getTimeFromCode(clipInfo.time);
        clipInfo.readableframed =   getFrameFromCode(clipInfo.framed);
        clipInfo.readablepersonal = getPersonalFromCode(clipInfo.personal);
        
        clipPosition.cropstart = 0;
        clipPosition.cropend = frames;
        
        setFrame(0);
        setPosition(0, 0, -1);
        
    };
    
    void setFrame(int frame){
        clipPosition.videostart = frame;
        clipPosition.videoend = clipPosition.videostart + getTotalFrames();
        clipPosition.audiostart = clipPosition.videostart + getAudioInFrameOffset();
        clipPosition.audioend = clipPosition.videostart + getAudioOutFrameOffset();
    }
    
    void setPosition(float x, float y, int screen){
        clipPosition.screen = screen;
        clipPosition.position.x = x;
        clipPosition.position.y = y;
        clipPosition.position.width = rect.width;
        clipPosition.position.height = rect.width;
    }
    
    void setCrop(int startframe, int endframe){
        assert(endframe > startframe);
        assert(endframe - startframe < frames);
        if(startframe != 0 && endframe != frames){
            clipPosition.isCropped = true;
        }else{
            clipPosition.isCropped = false;
        }
        clipPosition.cropstart = startframe;
        clipPosition.cropend = endframe;
        clipPosition.cropframes = endframe - startframe;
        setFrame(clipPosition.videostart);
    }
    
    string& getName(){
        return name;
    }
    
    string getVideoPath(){
        return videoFile.path;
    }
    
    ClipInfo& getClipInfo(){
        return clipInfo;
    }
    
    ofRectangle& getRect(){ // actual size + pos of movement in video (relative to 0,0)
        return rect;
    }
    
    void setRect(ofRectangle r){
        rect = r;
    }
    
    ofRectangle& getPosition(){ // transformed rect position (where to draw things)
        return clipPosition.position;
    }
    
    void setSize(float w, float h){
        width = w;
        height = h;
    }
    
    float getWidth(){
        return width;
    }
    
    float getHeight(){
        return height;
    }
    
    void setScale(float s){
        scale = s;
    }
    
    float getScale(){
        return scale;
    }
    
    int getScreen(){
        return clipPosition.screen;
    }
    
    int getVideoStart(){
        return clipPosition.videostart;
    }
    
    int getVideoEnd(){
        return clipPosition.videoend;
    }
    
    int getCropStart(){
        return clipPosition.cropstart;
    }
    
    int getCropEnd(){
        return clipPosition.cropend;
    }
    
    bool getIsCropped(){
        return clipPosition.isCropped;
    }
    
    int getAudioStart(){
        return clipPosition.audiostart;
    }
    
    int getAudioEnd(){
        return clipPosition.audioend;
    }
    
    int getTotalFrames(){
        return (clipPosition.isCropped ? clipPosition.cropframes : frames);
    }
    
    void setTotalFrames(int f){
        frames = f;
    }
    
    int getAudioInFrameOffset(){
        int frameoffset = frames * audioinpct;
        if(clipPosition.isCropped){
            if(frameoffset < clipPosition.cropstart || frameoffset > clipPosition.cropend){
                frameoffset = 0;
            }else{
                frameoffset -= clipPosition.cropstart;
            }
        }
        return frameoffset;
    }
    
    int getAudioOutFrameOffset(){
        int frameoffset = frames * audioutpct;
        if(clipPosition.isCropped){
            if(frameoffset > clipPosition.cropend){
                frameoffset = clipPosition.cropframes;
            }else{
                frameoffset -= clipPosition.cropstart;
            }
        }
        return frameoffset;
    }
    
    float getAudioInPct(){
        return audioinpct;
    }
    
    void setAudioInPct(float pct){
        audioinpct = pct;
    }
    
    float getAudioOutPct(){
        return audioutpct;
    }
    
    void setAudioOutPct(float pct){
        audioutpct = pct;
    }
    
    File getVideoFile(){
        return videoFile;
    }
    
    void setVideoFile(File file){
        videoFile = file;
    }

    File getAudioFile(){
        return audioFile;
    }
    
    void setAudioFile(File file){
        audioFile = file;
    }
    
    File getTextFile(){
        return textFile;
    }
    
    void setTextFile(File file){
        textFile = file;
    }
    
    string getText(){
        return text;
    }
    
    void setText(string t){
        text = t;
    }
    
    bool getClipLoading(){
        return clipPosition.isLoading;
    }
    
    bool setClipLoading(bool b){
        clipPosition.isLoading = b;
    }
    
    bool getClipStopping(){
        return clipPosition.isStopping;
    }
    
    bool setClipStopping(bool b){
        clipPosition.isStopping = b;
    }
    
    bool getAnalyzed(){
        return analyzed;
    }
    
    bool setAnalyzed(bool b){
        analyzed = b;
    }

    bool getDeleted(){
        return deleted;
    }
    
    bool setDeleted(bool b){
        deleted = b;
    }
    
    bool operator!=(const Clip &rhs) {
        if(name != rhs.name ||
           clipPosition.position != rhs.clipPosition.position ||
           clipPosition.videostart != rhs.clipPosition.videostart ||
           clipPosition.videoend != rhs.clipPosition.videoend ||
           clipPosition.videoframes != rhs.clipPosition.videoframes ||
           clipPosition.audiostart != rhs.clipPosition.audiostart ||
           clipPosition.audioend != rhs.clipPosition.audioend ||
           clipPosition.screen != rhs.clipPosition.screen ||
           clipPosition.cropstart != rhs.clipPosition.cropstart ||
           clipPosition.cropend != rhs.clipPosition.cropend ||
           clipPosition.cropframes != rhs.clipPosition.cropframes ||
           clipPosition.isStopping != rhs.clipPosition.isStopping ||
           clipPosition.isLoading != rhs.clipPosition.isLoading){
            return true;
        }else{
            return false;
        }
    }
    
    bool operator==(const Clip &rhs) {
        if(name == rhs.name &&
           clipPosition.position == rhs.clipPosition.position &&
           clipPosition.videostart == rhs.clipPosition.videostart &&
           clipPosition.videoend == rhs.clipPosition.videoend &&
           clipPosition.videoframes == rhs.clipPosition.videoframes &&
           clipPosition.audiostart == rhs.clipPosition.audiostart &&
           clipPosition.audioend == rhs.clipPosition.audioend &&
           clipPosition.screen == rhs.clipPosition.screen &&
           clipPosition.cropstart == rhs.clipPosition.cropstart &&
           clipPosition.cropend == rhs.clipPosition.cropend &&
           clipPosition.cropframes == rhs.clipPosition.cropframes &&
           clipPosition.isStopping == rhs.clipPosition.isStopping &&
           clipPosition.isLoading == rhs.clipPosition.isLoading){
            return true;
        }else{
            return false;
        }
    }
    
    friend ostream& operator<< (ostream &os, const Clip &c);
    
protected:
    
    // derived variables
    ClipInfo clipInfo;
    ClipPosition clipPosition;
    
    // serialized variables
    string name; // actually the filename
    
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
    
    float width;
    float height;
    float scale;
    
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
        ar & BOOST_SERIALIZATION_NVP(width);
        ar & BOOST_SERIALIZATION_NVP(height);
        ar & BOOST_SERIALIZATION_NVP(scale);
	};
    
private:
    
};

inline ostream& operator<<(ostream& os, const Clip &c){
    os <<   c.name << " :position: " << 
    c.clipPosition.position.x << ", " << 
    c.clipPosition.position.y << ", " << 
    c.clipPosition.position.width << ", " << 
    c.clipPosition.position.height << " (" <<
    c.clipPosition.screen << ") frames: " << 
    c.clipPosition.videostart << ", " <<
    c.clipPosition.audiostart << ", " <<
    c.clipPosition.audioend << ", " <<
    c.clipPosition.videoend << " (" <<
    c.frames << ") " << " [" <<
    c.clipPosition.cropstart << ":" <<
    c.clipPosition.cropend << "]" << " " <<
    c.clipPosition.isLoading << " " <<
    c.clipPosition.isStopping;// << " [" <<
    //            c.frames * c.audioinpct << ":" <<
    //            c.frames * c.audioutpct<< "]" << "  " <<
    //            c.clipPosition.videostart - c.clipPosition.videostart << ", " <<
    //            c.clipPosition.audiostart - c.clipPosition.videostart << ", " <<
    //            c.clipPosition.audioend - c.clipPosition.videostart << ", " <<
    //            c.clipPosition.videoend - c.clipPosition.videostart;
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
    
    friend ostream& operator<< (ostream &os, const ClipType &ct);
    
    map<string, int> types;
    
protected:
    
};

inline ostream& operator<<(ostream& os, const ClipType &ct){
    for(map<string, int>::const_iterator it = ct.types.begin(); it != ct.types.end(); it++){
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
                    if(group[i].getName() == value) clipGroup.push(group[i]);
                }
                    break;
                case CATEGORY:
                {
                    if(group[i].getClipInfo().category == value) clipGroup.push(group[i]);
                }
                    break;
                case QUESTION:
                {
                    if(group[i].getClipInfo().question == value) clipGroup.push(group[i]);
                }
                    break;
                case TYPE:
                {
                    if(group[i].getClipInfo().type == value) clipGroup.push(group[i]);
                }
                    break;
                case TIME:
                {
                    if(group[i].getClipInfo().time == value) clipGroup.push(group[i]);
                }
                    break;
                case FRAMED:
                {
                    if(group[i].getClipInfo().framed == value) clipGroup.push(group[i]);
                }
                    break;
                case PERSONAL:
                {
                    if(group[i].getClipInfo().personal == value) clipGroup.push(group[i]);
                }
                    break;
                case NUMBER:
                {
                    if(ofToInt(group[i].getClipInfo().number) == ofToInt(value)) clipGroup.push(group[i]);
                }
                    break;
                case PERSON:
                {
                    if(group[i].getClipInfo().person == value) clipGroup.push(group[i]);
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
                    if(group[i].getName() != value) clipGroup.push(group[i]);
                }
                    break;
                case CATEGORY:
                {
                    if(group[i].getClipInfo().category != value) clipGroup.push(group[i]);
                }
                    break;
                case QUESTION:
                {
                    if(group[i].getClipInfo().question != value) clipGroup.push(group[i]);
                }
                    break;
                case TYPE:
                {
                    if(group[i].getClipInfo().type != value) clipGroup.push(group[i]);
                }
                    break;
                case TIME:
                {
                    if(group[i].getClipInfo().time != value) clipGroup.push(group[i]);
                }
                    break;
                case FRAMED:
                {
                    if(group[i].getClipInfo().framed != value) clipGroup.push(group[i]);
                }
                    break;
                case PERSONAL:
                {
                    if(group[i].getClipInfo().personal != value) clipGroup.push(group[i]);
                }
                    break;
                case NUMBER:
                {
                    if(ofToInt(group[i].getClipInfo().number) != ofToInt(value)) clipGroup.push(group[i]);
                }
                    break;
                case PERSON:
                {
                    if(group[i].getClipInfo().person != value) clipGroup.push(group[i]);
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
                return personTypes;
            }
                break;
        }
    }
    
    void push_front(Clip & clip){
        std::reverse(group.begin(), group.end());
        group.push_back(clip);
        std::reverse(group.begin(), group.end());
        pushCategoryTypes(clip);
    }
    
    void push(Clip & clip){
        group.push_back(clip);
        pushCategoryTypes(clip);
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
    
    Clip pop(Clip clip){
        for(int i = 0; i < group.size(); i++){
            if(group[i] == clip){
                popCategoryTypes(clip);
                group.erase(group.begin()+i, group.begin()+i+1);
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
    
    Clip & getClip(string name){
        for(int j = 0; j < group.size(); j++){
            if(group[j].getName() == name){
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
    
    friend ostream& operator<< (ostream &os, const ClipGroup &cg);
    
    vector<Clip> group;
    
protected:
    
    void pushCategoryTypes(Clip & clip){
        ClipInfo & info = clip.getClipInfo(); 
        nameTypes.push(clip.getName());
        categoryTypes.push(info.category);
        questionTypes.push(info.question);
        typeTypes.push(info.type);
        timeTypes.push(info.time);
        framedTypes.push(info.framed);
        personalTypes.push(info.personal);
        personTypes.push(info.person);
    }
    
    void popCategoryTypes(Clip & clip){
        ClipInfo & info = clip.getClipInfo(); 
        nameTypes.pop(clip.getName());
        categoryTypes.pop(info.category);
        questionTypes.pop(info.question);
        typeTypes.pop(info.type);
        timeTypes.pop(info.time);
        framedTypes.pop(info.framed);
        personalTypes.pop(info.personal);
        personTypes.pop(info.person);
    }
    
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

inline ostream& operator<<(ostream& os, const ClipGroup &cg){
    for(int i = 0; i < cg.group.size(); i++){
        os << cg.group[i] << endl;
    }
    return os;
};

struct VideoClip {
    ofxThreadedVideo* video;
    Clip clip;
};
    
class ClipTimeline {

public:
        
    ClipTimeline(){
        clear();
    };
    
    ~ClipTimeline(){
        clear();
    };
    
    void setup(string blackpath, ofPixelFormat pixelformat){
        
        ofxLogNotice() << "Setup timeline: " << blackpath << endl;
        
        dummyVideoClip.video = NULL;
        dummyVideoClip.clip = dummyClip;
        
        pixelFormat = pixelformat;
        
        ofxThreadedVideo * video = new ofxThreadedVideo;
        video->setPixelFormat(pixelFormat);
        video->setUseAutoPlay(true);
        video->loadMovie(blackpath);
        ofAddListener(video->threadedVideoEvent, this, &ClipTimeline::threadedVideoEvent);
        
        while(!video->isLoaded()){
            video->update();
        }
        
        video->setVolume(0.0f);
        video->setLoopState(OF_LOOP_NORMAL);
        
        VideoClip vc;
        vc.video = video;
        vc.clip = dummyClip;
        
        videoClips.push_back(vc);
        
        if(pixelFormat == OF_PIXELS_2YUV){
            shader.load(ofToDataPath("yuyvtorgba"));
            renderer.allocate(1920, 1080);
        }
        
    }
    
    void update(){
        
        if(videoClips.size() == 0) return;
        
        if(!bPaused){
            
            for(int i = 0; i < videoClips.size(); i++){
                //if(videos[i]->isLoading() || videos[i]->isPlaying())
                videoClips[i].video->update();
            }
            
            currentClips = getClipsFrom(currentFrame, currentFrame);
            
            if(currentClips.size() > 0){
                
                for(int i = 0; i < currentClips.size(); i++){
                    
                    VideoClip & videoClip = isClipAssigned(currentClips[i]);
                    
                    Clip & clip = videoClip.clip;
                    ofxThreadedVideo * video = videoClip.video;
                    
                    //cout << video->isLoaded() << " " << video->isPlaying() << " " << clip.getClipLoading() << " " << currentFrame << " " << clip.getVideoEnd() << " " << currentClips[i].getVideoEnd() << endl;
                    
                    if(!video->isLoaded() && !video->isPlaying() && !clip.getClipLoading() && !clip.getClipStopping()){// && !clip.getClipStopping()){
                        //ofxLogVerbose() << currentFrame << " " << clip.getVideoStart() << " " << clip.getVideoEnd() << " " << clip.getCropStart() << " " << clip.getCropEnd() << endl;
                        ofxLogVerbose() << "Loading " << clip << endl;
                        if(video->loadMovie(clip.getVideoPath())) clip.setClipLoading(true);
                        while(!video->isLoading()) video->update();
                    }
                    
                    if(video == NULL) continue;
                    
                    if(video->isLoaded() && !video->isPlaying() && !clip.getClipStopping()){
                        ofxLogVerbose() << "Playing " << clip << endl;
                        ofxLogVerbose() << video->getIsMovieDone() << " " << clip.getIsCropped() << " " << currentFrame << " " << clip.getVideoEnd() - 2 << endl;
//                        if(video->getIsMovieDone() || (clip.getIsCropped() && currentFrame >= clip.getVideoEnd() - 2)){
//                            ofxLogVerbose() << "continue" << endl;
//                            continue;
//                        }
                        
                        if(clip.getCropStart() + currentFrame - clip.getVideoStart() > 3){
                            ofxLogVerbose() << "Setting frame: " << clip.getCropStart() + currentFrame - clip.getVideoStart() << endl;
                            video->setFrame(clip.getCropStart() + currentFrame - clip.getVideoStart());
                        }
                        video->setLoopState(OF_LOOP_NONE);
                        video->play();
                        clip.setClipLoading(false);
                    }

                    if((video->getIsMovieDone() || (clip.getIsCropped() && currentFrame >= clip.getVideoEnd() - 2)) && !clip.getClipStopping()){
                        ofxLogVerbose() << "Stopping " << clip << endl;
                        ofxLogVerbose() << video->getIsMovieDone() << " " << clip.getIsCropped() << " " << currentFrame << " " << clip.getVideoEnd() - 2 << endl;
                        video->stop();
                        clip.setClipLoading(false);
                        clip.setClipStopping(true);
                    }
                }
            }
            for(int i = videoClips.size() - 1; i >= 0 ; i--){
                if(videoClips[i].video->isPlaying()){
                    if(i == 0){
                        ofxLogVerbose() << "Black timestamp" << endl;
                        if(videoClips[0].video->isFrameNew()) currentFrame++;
                    }else{
                        Clip & clip = videoClips[i].clip;
                        ofxThreadedVideo * video = videoClips[i].video;
                        if(video->getCurrentFrame() < clip.getCropStart() || video->getIsMovieDone()) continue;
                        //cout << currentFrame << " " << clip.getVideoStart() + video->getCurrentFrame() - clip.getCropStart() << " " << video->getCurrentFrame() << endl;
                        currentFrame = clip.getVideoStart() + video->getCurrentFrame() - clip.getCropStart();
                        syncClipName = clip.getName();
                        break;
                    }
                }
            }
        }
    }
    
    void threadedVideoEvent(ofxThreadedVideoEvent & e){
        ofxLogVerbose() << e << endl;
    }
    
   VideoClip& assignVideoClip(Clip & clip){
        assert(videoClips.size() > 0);
        int freeIndex = -1;
        for(int i = 0; i < videoClips.size(); i++){
            if((!videoClips[i].video->isLoading() && !videoClips[i].video->isPlaying())){
                freeIndex = i;
                break;
            }
        }
        if(freeIndex != -1){
            videoClips[freeIndex].clip = clip;
            return videoClips[freeIndex];
        }else{
            ofxThreadedVideo * video = new ofxThreadedVideo;
            video->setPixelFormat(pixelFormat);
            video->setUseAutoPlay(false);
            video->setUseQueue(false);
            ofAddListener(video->threadedVideoEvent, this, &ClipTimeline::threadedVideoEvent);
            VideoClip vc;
            vc.video = video;
            vc.clip = clip;
            videoClips.push_back(vc);
            return videoClips[videoClips.size() - 1];
        }
    }
    
    VideoClip& isClipAssigned(Clip & clip){
        assert(videoClips.size() > 0);
        for(int i = 1; i < videoClips.size(); i++){
            string clipPath = clip.getVideoPath();
            if(videoClips[i].clip == clip &&
               (videoClips[i].video->getMoviePath() == clipPath ||
                videoClips[i].video->isQueued(clipPath))){
                return videoClips[i];
            }
        }
        return assignVideoClip(clip);
    }
    
    void previousClip(){
//        int previousClipFrame = currentFrame;
//        vector<string> clipsNow = getClipsFrom(previousClipFrame, previousClipFrame);
//        int numDiffs = 0;
//        for(int i = previousClipFrame; i >= 0; i--){
//            vector<string> clipsThen = getClipsFrom(i, i);
//            if(clipsThen != clipsNow){
//                if(numDiffs == 1){
//                    previousClipFrame = i;
//                    break;
//                }else{
//                    numDiffs++;
//                    clipsNow = clipsThen;
//                }
//            }
//        }
//        setFrame(previousClipFrame);
    }
    
    void nextClip(){
//        int nextClipFrame = currentFrame;
//        vector<string> clipsNow = getClipsFrom(nextClipFrame, nextClipFrame);
//        for(int i = nextClipFrame; i < getTotalFrames(); i++){
//            vector<string> clipsThen = getClipsFrom(i, i);
//            if(clipsThen != clipsNow){
//                nextClipFrame = i;
//                break;
//            }
//        }
//        //cout << "MOFO2: " << nextClipFrame << endl;
//        setFrame(nextClipFrame);
    }
    
    void setFrame(int frame){
        bool cPaused = bPaused;
        stop();
        currentFrame = frame;
        setPaused(cPaused);
    }
    
    void stop(){
        if(videoClips.size() > 1){
            for(int i = 1; i < videoClips.size(); i++){
                videoClips[i].video->stop();
            }
        }
        for(int i = 0; i < group.size(); i++){
            Clip & clip = group[i];
            clip.setClipLoading(false);
        }
        setPaused(true);
    }
    
    void play(){
        setPaused(false);
    }
    
    void setPaused(bool b){
        bPaused = b;
        if(bPaused){
            for(int i = 0; i < videoClips.size(); i++){
                videoClips[i].video->setPaused(true);
            }
        }else{
            for(int i = 0; i < videoClips.size(); i++){
                videoClips[i].video->setPaused(false);
            }
        }
    }
    
    void togglePaused(){
        setPaused(!bPaused);
    }
    
//    void draw(){
//        if(videos.size() == 0) return;
//        videos[0]->draw(0,0,0,0);
//        glPushMatrix();
//        ofFill();
//        ofSetColor(0, 0, 0, 255);
//        ofRect(0, 200, 1920.0 * 6, 1080.0/2.0);
//        ofSetColor(255, 255, 255, 255);
//        //ofEnableBlendMode(OF_BLENDMODE_SCREEN);
//        for(int i = 1; i < videos.size(); i++){
//            videos[i]->draw(200 + 480 * (i - 2), 200, 1920.0/2.0, 1080.0/2.0);
//        }
//        //ofDisableBlendMode();
//        glPopMatrix();
//    }
    
    void draw(int screen){
        if(videoClips.size() < 2) return;
        
        videoClips[0].video->draw(0,0,0,0);
        
        for(int i = 1; i < videoClips.size(); i++){
            
            Clip & clip = videoClips[i].clip;
            ofxThreadedVideo * video = videoClips[i].video;
            
            if(video->isPlaying()){
                
                if(clip.getScreen() == screen){
                    
                    float fade = 1.0f;
                    int fadeInSeconds = 3;
                    int currentFadeFrame = MAX(currentFrame - clip.getVideoStart(), video->getCurrentFrame() - clip.getCropStart());
                    
                    if(video->getCurrentFrame() < clip.getCropStart()) continue;
                    
                    if(currentFadeFrame >= 0 && currentFadeFrame < fadeInSeconds * 25){
                        fade = (float)currentFadeFrame / (float)(fadeInSeconds * 25);
                    }else if(currentFadeFrame > clip.getTotalFrames() - fadeInSeconds * 25 && currentFadeFrame < clip.getTotalFrames()){
                        fade = (((float)clip.getTotalFrames() - currentFadeFrame) / (float)(fadeInSeconds * 25));
                    }else if(currentFadeFrame >= clip.getTotalFrames()){
                        fade = 0.0f;
                    }
                    
                    //cout << fade << " " << currentFrame << " " <<  currentFadeFrame << " " << clip.getName() << " " << video->getCurrentFrame() << " " << clip.getCropStart() << " " << clip.getVideoEnd() << " " << clip.getTotalFrames() << endl;
                    
                    if(video->getPixelFormat() == OF_PIXELS_2YUV){
                        
                        renderer.begin();
                        ofClear(0.0f, 0.0f, 0.0f, 1.0f);
                        renderer.end();
                        shader.begin();
                        shader.setUniformTexture("yuvTex", video->getTextureReference(), 1);
                        shader.setUniform1i("conversionType", (false ? 709 : 601));
                        shader.setUniform1f("fade", fade);
                        renderer.draw(clip.getPosition().x - clip.getRect().x, clip.getPosition().y);
                        shader.end();
                        
                    }else{
                        
                        ofSetColor(255 * fade, 255 * fade, 255 * fade, 255 * fade);
                        video->draw(clip.getPosition().x - clip.getRect().x, clip.getPosition().y);
                        
                    }
                    ofSetColor(255, 255, 255, 255);
                }
            }
            
        }
    }
    
    void drawTimeline(){
        drawTimeline(0, 0, ofGetWidth(), ofGetHeight());
    };
    
    void drawTimeline(float x, float y, float width, float height){
        
//        glPushMatrix();
//        
//        ostringstream os;
//        os << "FRAME: " << getCurrentFrame() << " / " << getTotalFrames() << endl;
//        os << "FPS: " << ofGetFrameRate() << endl;
//        
//        for(int i = 0; i < videos.size(); i++){
//            os << "VIDEO_" << i << ": " << videos[i]->getFrameRate() << "  " << videos[i]->getMoviePath() << endl;
//        }
//        
//        ofDrawBitmapString(os.str(), 20, 20);
//        
//        glPopMatrix();
        
        glPushMatrix();
        float framescale = (1.0/(float)getTotalFrames()) * width * 16;
        float pixelscale = (height/(2 * 1920.0f));
        
        glScalef(framescale, pixelscale, 1.0f);
        
        ofNoFill();
        ofSetColor(0, 255, 0);
        ofLine(2000, 0, 2000, 1920 + 1440);
        ofRect(0, 0, (float)getTotalFrames(), 1920.0f);
        ofRect(0, 1919.0f, (float)getTotalFrames(), 1440.0f);
        ofTranslate(2000 - getCurrentFrame(), 0.0f, 0.0f);
        
        for(int i = 0; i < group.size(); i++){
            
            Clip & clip = group[i];
            
            ofFill();
            glPushMatrix();
            ofTranslate(0.0f, 1920.0f * (clip.getScreen()), 0.0f);
            if(clip.getName() == syncClipName){
                ofSetColor(64, 64, 128, 64);
                ofRect(clip.getVideoStart(),
                       clip.getPosition().x,
                       clip.getTotalFrames(),
                       clip.getPosition().width);
                ofSetColor(0, 0, 255);
            }else{
                ofSetColor(64 * clip.getScreen(), 64, 64, 32);
                ofRect(clip.getVideoStart(),
                       clip.getPosition().x,
                       clip.getTotalFrames(),
                       clip.getPosition().width);
                ofSetColor(255, 255, 255);
                
            }
            
            ofNoFill();
            ofRect(clip.getVideoStart(),
                   clip.getPosition().x,
                   clip.getTotalFrames(),
                   clip.getPosition().width);
            ofSetColor(255, 0, 0);
            ofRect(clip.getAudioStart(),
                   clip.getPosition().x + 1,
                   clip.getAudioEnd() - clip.getAudioStart(),
                   clip.getPosition().width - 2);
            ofSetColor(255, 255, 255);
            
            glPopMatrix();
            
        }
        
        glPopMatrix();
        
    };
    
    void clear(){
        group.clear();
        calculateFrames();
        currentFrame = 0;
        bPaused = true;
        if(videoClips.size() > 1){
            stop();
            videoClips.erase(videoClips.begin() + 1, videoClips.end());
        }
    };
    
    bool insertClipAt(Clip clip, int frame){
        
        int tries = 0;
        bool fitted = false;
        bool possible = true;
        
        int screen;
        float x, y, width;
        
        while(!fitted && possible){
            
            tries++;
            
            screen = getRandomDistribution(2, 0.7f, 0.3f);
            
            ofRectangle & originalRect = clip.getRect();
            
            x = ofRandom(0, (screen == 0 ? 1920.0f : 1440.0f) - originalRect.width);
            y = (1080.0f - 200.0f) - originalRect.height - originalRect.y;
            width = originalRect.width;
            
            fitted = !getAnyClipAt(frame,
                                   x,
                                   frame + clip.getTotalFrames(),
                                   width,
                                   screen);
            
            if(!fitted && tries > 44){
                possible = false;
                fitted = true;
            }
            
        }
        
        if(fitted && possible){
            clip.setFrame(frame);
            clip.setPosition(x, y, screen);
            group.push(clip);
            calculateFrames();
            return true;
        }else{
            return false;
        }
        
        
    };
    
    Clip getClipAt(float x, float y, float width, float height){
        
        float framescale = (1.0/(float)getTotalFrames()) * width * 16;
        float pixelscale = (height/(2 * 1920.0f));

        for(int i = 0; i < group.size(); i++){
            
            Clip clip = group[i];
            
            ofRectangle r = ofRectangle((clip.getVideoStart() + 2000 - getCurrentFrame()) * framescale,
                                        (clip.getPosition().x * pixelscale) + (clip.getScreen() * 1920.0f * pixelscale),
                                        clip.getTotalFrames() * framescale,
                                        clip.getPosition().width * pixelscale);
            
            ofPoint p = ofPoint(x, y);
            
            if(r.inside(p)){
                return clip;
            }
            
        }
        
        return dummyClip;
    }
    
    vector<Clip> getClipsFrom(int startFrame, int endFrame){
        vector<Clip> clips;
        getClipsFrom(startFrame, endFrame, clips);
        return clips;
    };
    
    void getClipsFrom(int startFrame, int endFrame, vector<Clip> & clips){
        
        ofRectangle f = ofRectangle(startFrame,
                                    0,
                                    endFrame - startFrame,
                                    1);
        
        for(int i = 0; i < group.size(); i++){
            
            Clip & clip = group[i];
            
            ofRectangle r = ofRectangle(clip.getVideoStart(),
                                        0,
                                        clip.getTotalFrames(),
                                        1);
            
            if(r.intersects(f)){
                clips.push_back(clip);
            }
        }
    };
    
    //    vector<Clip> getClipsFrom(ofRectangle & r, int screen){
    //        vector<Clip> clips;
    //        getClipsFrom(r.x, r.y, r.width, r.height, screen, clips);
    //        return clips;
    //    };
    
    void getClipsFrom(int startFrame, float x, int endFrame, float width, int screen, vector<Clip> & clips){
        
        ofRectangle f = ofRectangle(startFrame,
                                    x,
                                    endFrame - startFrame,
                                    width);
        
        for(int i = 0; i < group.size(); i++){
            
            Clip & clip = group[i];
            
            if(clip.getScreen() != screen) continue;
            
            ofRectangle r = ofRectangle(clip.getVideoStart(),
                                        clip.getPosition().x,
                                        clip.getTotalFrames(),
                                        clip.getPosition().width);
            
            if(r.intersects(f)){
                clips.push_back(clip);
            }
        }
    };
    
    //    bool getAnyClipAt(ofRectangle & r, int screen){
    //        return getAnyClipAt(r.x, r.y, r.width, r.height, screen);
    //    };
    
    bool getAnyClipAt(int startFrame, float x, int endFrame, float width, int screen){
        
        ofRectangle f = ofRectangle(startFrame,
                                    x,
                                    endFrame - startFrame,
                                    width);
        
        for(int i = 0; i < group.size(); i++){
            
            Clip & clip = group[i];
            
            if(clip.getScreen() != screen) continue;
            
            ofRectangle r = ofRectangle(clip.getVideoStart(),
                                        clip.getPosition().x,
                                        clip.getTotalFrames(),
                                        clip.getPosition().width);
            
            if(r.intersects(f)){
                return true;
            }
        }
        return false;
    };
    
//    Clip & getClipFromName(string name){
//        for(int i = 0; i < group.size(); i++){
//            if(group[i].getName() == name){
//                return group[i];
//            }
//        }
//        ofxLogError() << "Clip out of bounds with name: " << name;
//        return dummyClip;
//    }
//    
//    Clip & getClipFromPath(string path){
//        vector<string> pathParts = ofSplitString(path, "/");
//        return getClipFromName(ofSplitString(pathParts[pathParts.size() - 1], ".")[0]);
//    }
    
    int getCurrentFrame(){
        return currentFrame;
    }
    
    int getTotalFrames(){
        return totalFrames;
    }
    
    void calculateFrames(){
        totalFrames = 0;
        for(int i = 0; i < group.size(); i++){
            totalFrames = MAX(totalFrames, group[i].getVideoEnd());
        }
    };
    
    vector<VideoClip>& getVideoClips(){
        return videoClips;
    }
    
    vector<Clip>& getCurrentClips(){
        return currentClips;
    }

    ClipGroup& getGroup(){
        return group;
    }
    
protected:
        
    ClipGroup group;
    Clip dummyClip;
    
    bool bPaused;
    
    int currentFrame;
    int totalFrames;
    
    string syncClipName;
    
    ofPixelFormat pixelFormat;
    vector<VideoClip> videoClips;
    VideoClip dummyVideoClip;
    
    vector<Clip> currentClips;
    
    ofShader shader;
    ofFbo renderer;
};

#endif