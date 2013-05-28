//
//  PlayController.cpp
//  DeepListen
//
//  Created by game over on 3/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#include "PlayController.h"

//--------------------------------------------------------------
PlayController::PlayController(){
    ofxLogVerbose() << "Creating PlayController" << endl;
}

//--------------------------------------------------------------
PlayController::~PlayController(){
    ofxLogVerbose() << "Destroying PlayController" << endl;
}

//--------------------------------------------------------------
void PlayController::setup(){
    
    StateGroup newPlayControllerStates("PlayControllerStates", true);
    
    newPlayControllerStates.addState(State(kPLAYCONTROLLER_INIT, "kPLAYCONTROLLER_INIT"));
    newPlayControllerStates.addState(State(kPLAYCONTROLLER_MAKE, "kPLAYCONTROLLER_MAKE"));
    newPlayControllerStates.addState(State(kPLAYCONTROLLER_PLAY, "kPLAYCONTROLLER_PLAY"));
    
    appModel->addStateGroup(newPlayControllerStates);

    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    playControllerStates.setState(kPLAYCONTROLLER_INIT);
    
}
multimap<int, int> stats;
//--------------------------------------------------------------
void PlayController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    switch (playControllerStates.getState()) {
        case kPLAYCONTROLLER_INIT:
        {
            ClipTimeline & clipTimeline = appModel->getClipTimeline();
            clipTimeline.clear();
            resetClipGroups();
            playControllerStates.setState(kPLAYCONTROLLER_MAKE);
        }
            break;
        case kPLAYCONTROLLER_MAKE:
        {
            // generate clip group
            ClipTimeline & timeline = appModel->getClipTimeline();
            timeline.clear();
            makeClipGroup();

            ofxLogVerbose() << "\\/---------------------\\/" << endl;
            ofxLogVerbose() << timeline.getGroup() << endl;
            ofxLogVerbose() << "/\\---------------------/\\" << endl;
            
            timeline.play();

        }
            break;
        case kPLAYCONTROLLER_PLAY:
        {
            ClipTimeline & clipTimeline = appModel->getClipTimeline();
            
            clipTimeline.update();
            
            vector<VideoClip> & videoClips = clipTimeline.getVideoClips();
            for(int i = 0; i < videoClips.size(); i++){
                ofxThreadedVideo * video = videoClips[i].video;
                appModel->setProperty("PlayState_"+ofToString(i), ofToString(video->isPlaying()) + " " + ofToString(video->getIsMovieDone()) + " " + ofToString(video->getFrameRate()));
            }
            
            ostringstream os;
            vector<Clip> & currentClips = clipTimeline.getCurrentClips();
            for(int i = 0; i < currentClips.size(); i++){
                Clip & clip = currentClips[i];
                os << clip.getName() << " " << clip.getClipLoading() << " " << (i == currentClips.size() - 1 ? "" : ", ");
            }
            
            appModel->setProperty("Clips", os.str());
            
            appModel->setProperty("TimeFrame", clipTimeline.getCurrentFrame());
            appModel->setProperty("TimeFrames", clipTimeline.getTotalFrames());
            
            if(clipTimeline.getCurrentFrame() >= clipTimeline.getTotalFrames()){
                playControllerStates.setState(kPLAYCONTROLLER_INIT);
            }
            
        }
            break;
    }
    
}

//--------------------------------------------------------------
void PlayController::makeClipGroup(){
    
    ofxLogNotice() << "Making new ClipGroup for playback" << endl;
    
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    ofSeedRandom();
    
    ClipGroup & allClips = appModel->getClipGroupReference("allClips");
    ClipGroup & allListenClips = appModel->getClipGroupReference("allListenClips");
    ClipGroup & allStatClips = appModel->getClipGroupReference("allStatClips");
    ClipGroup & allIntroClips = appModel->getClipGroupReference("allIntroClips");
    
    ClipGroup & allTitleClips = appModel->getClipGroupReference("allTitleClips");
    ClipGroup & statistics = appModel->getClipGroupReference("statistics");
    
    string category = "";
    int flip;
    int lastAudioFreeFrame = 0;
    
//    ClipTimeline & timeline = appModel->getClipTimeline();
//    
//    for (int i = 0; i < 1000; i++){
//        bool insertedOk = false;
//        int r = ofRandom(5);
//        Clip & clip = allIntroClips[r];
//        
//        // calculate frame starts and ends
//        insertedOk = timeline.insertClipAt(clip, lastAudioFreeFrame - clip.getAudioInFrameOffset());
//        
//
//        if(insertedOk){
//            lastAudioFreeFrame = timeline.getGroup()[timeline.getGroup().size() - 1].getAudioEnd();
//        }
//    }
//    playControllerStates.setState(kPLAYCONTROLLER_PLAY);
    
    
    while(true){

        // flip for inserting a special or inserting a category
        flip = getRandomDistribution(2, 0.8f, 0.2f);
        
        if(true){//flip == 0){ // do a category
            
            ClipType allCategoryTypes = allClips.getClipTypes(CATEGORY);
            
            allCategoryTypes.popall("LSTN");
            allCategoryTypes.popall("SPEC");
            allCategoryTypes.popall("STAT");
            allCategoryTypes.popall("INTR");
            allCategoryTypes.popall(category);
            
            int remainingNumCategories = allCategoryTypes.countunique();

            if (remainingNumCategories < 4){ // 5 is all categories
                // reset the clip groups by changing state
                playControllerStates.setState(kPLAYCONTROLLER_PLAY);
                return;// newClipGroup;
            }
            
            // could use a random distribution algorithm here instead
            int numberOfQuestions = (int)ofRandom(3) + 2; // min 1; max 2
            int numberOfStatements = (int)ofRandom(2) + 1; // min 1; max 2
            int numberOfIntros = (int)ofRandom(2) + 1;
            
            category = allCategoryTypes.poprandom();
            
            ClipGroup categoryGroup = allClips.getContains(CATEGORY, category);
            ClipType questionGroup = categoryGroup.getClipTypes(QUESTION);
            
            string question = questionGroup.poprandom();
            ClipGroup uniqueNameGroup = categoryGroup.getContains(QUESTION, question).getUnique(PERSON, numberOfQuestions);
            
            // mix up the order
            // TODO: might want to use a randomDist
            // to weight statements at start etc
            uniqueNameGroup.shuffle();
            
            ClipGroup introGroup;
            
            for(int j = 0; j < uniqueNameGroup.size(); j++){
                Clip introClip = allIntroClips.getContains(PERSON, uniqueNameGroup[j].getClipInfo().person).poprandom();
                cout << uniqueNameGroup[j].getClipInfo().person << " " << introClip << endl;
                introGroup.push(introClip);
            }
            
            for(int j = 0; j < numberOfIntros; j++){
                Clip introClip = introGroup.poprandom();
                uniqueNameGroup.push_front(introClip);
            }
            
            ClipGroup statementGroup = allStatClips;
            
            // make sure no statement is by someone answering a question
            for(int j = 0; j < uniqueNameGroup.size(); j++){
                statementGroup = statementGroup.getExcludes(PERSON, statementGroup[j].getClipInfo().person);
            }
            
            for(int j = 0; j < MIN(statementGroup.size(), numberOfStatements); j++){ // make sure we don't exceed statementGroup size!
                Clip statementClip;
                if(category == "CNTY"){
                    statementClip = statementGroup.getContains(QUESTION, "CNIS").poprandom();
                }
                if(category == "CULT"){
                    statementClip = statementGroup.getContains(QUESTION, "CUIS").poprandom();
                }
                if(category == "FAML"){
                    statementClip = statementGroup.getContains(QUESTION, "FAIS").poprandom();
                }
                if(category == "IDEN"){
                    statementClip = statementGroup.getContains(QUESTION, "IDIS").poprandom();
                }
                if(category == "RITE"){
                    statementClip = statementGroup.getContains(QUESTION, "RIIS").poprandom();
                }
                uniqueNameGroup.push_front(statementClip);
            }
            if(allTitleClips.size() > 0){
                Clip titleClip = allTitleClips.getContains(CATEGORY, category)[0];
                uniqueNameGroup.push_front(titleClip);
            }
            ClipTimeline & timeline = appModel->getClipTimeline();
            
            // calculate audio overlaps and pack
            for(int j = 0; j < uniqueNameGroup.size(); j++){
                
                ClipPosition clipPosition;
                Clip & clip = uniqueNameGroup[j];
                
                bool insertedOk = false;
                
                if(clip.getName().rfind("OOOO_00_TITLE") != string::npos){
                    // calculate frame starts and ends
                    insertedOk = timeline.insertClipAt(clip, lastAudioFreeFrame);
                }else{
                    // calculate frame starts and ends
                    insertedOk = timeline.insertClipAt(clip, lastAudioFreeFrame - clip.getAudioInFrameOffset());
                }
                
                if(insertedOk){
                    lastAudioFreeFrame = timeline.getGroup()[timeline.getGroup().size() - 1].getAudioEnd();
                }else{
                    ofxLogWarning() << "Rejected" << clip << endl;
                    uniqueNameGroup.pop(clip);
                    continue;
                }
                
            }

            allClips.pop(uniqueNameGroup);
            //newClipGroup.push(uniqueNameGroup);
            
        }else{ // do a special
            
        }
    }

}

//--------------------------------------------------------------
void PlayController::resetClipGroups(){
    
    // check if we've already made the clip groups
    if(!appModel->getClipGroupExists("originalClips")){ // assume that if we have this one we have all the clipGroups!
        
        ofxLogNotice() << "Creating ClipGroups" << endl;
        
        // create clip groups
        ClipGroup allClips, originalClips;
        ClipGroup allListenClips, originalListenClips;
        ClipGroup allIntroClips, originalIntroClips;
        ClipGroup allStatClips, originalStatClips;
        ClipGroup allSpecClips, originalSpecClips;
        ClipGroup allCategoryClips, originalCategoryClips;
        ClipGroup allTitleClips;
        
        ClipGroup statistics, playGroup;
        
        // iterate through all clips and add to an "all" clips ClipGroup
        for(int i = 0; i < appModel->getClips().size(); i++){
            Clip & clip = appModel->getClip(i);
            if(!clip.getDeleted()) originalClips.push(clip);
        }
        
        // get all the title clips and delete them from the original list
        allTitleClips = originalClips.getContains(QUESTION, "TTLE");
        originalClips.pop(allTitleClips);
        
        // now make a copy
        allClips = originalClips;
        
        // filter into Listen, Intro, Statement and Special clip groups
        originalListenClips = originalClips.getContains(CATEGORY, "LSTN");
        originalIntroClips = originalClips.getContains(CATEGORY, "INTR");
        originalStatClips = originalClips.getContains(CATEGORY, "STAT");
        originalSpecClips = originalClips.getContains(CATEGORY, "SPEC");
        
        originalCategoryClips = originalClips;
        originalCategoryClips.pop(originalListenClips);
        originalCategoryClips.pop(originalIntroClips);
        originalCategoryClips.pop(originalStatClips);
        originalCategoryClips.pop(originalSpecClips);
        
        allCategoryClips = originalCategoryClips;
        
        // make copies of these
        allListenClips = originalListenClips;
        allIntroClips = originalIntroClips;
        allStatClips = originalStatClips;
        allSpecClips = originalSpecClips;
        
        // add them all to the model
        appModel->setClipGroup("originalClips", originalClips);
        appModel->setClipGroup("originalListenClips", originalListenClips);
        appModel->setClipGroup("originalIntroClips", originalIntroClips);
        appModel->setClipGroup("originalStatClips", originalStatClips);
        appModel->setClipGroup("originalSpecClips", originalSpecClips);
        appModel->setClipGroup("originalCategoryClips", originalCategoryClips);
        
        appModel->setClipGroup("allClips", allClips);
        appModel->setClipGroup("allListenClips", allListenClips);
        appModel->setClipGroup("allIntroClips", allIntroClips);
        appModel->setClipGroup("allStatClips", allStatClips);
        appModel->setClipGroup("allSpecClips", allSpecClips);
        appModel->setClipGroup("allCategoryClips", allCategoryClips);
        appModel->setClipGroup("allTitleClips", allTitleClips);
        
        
        appModel->setClipGroup("statistics", statistics);
        appModel->setClipGroup("playGroup", playGroup);
        
    }else{
        
        ofxLogNotice() << "Re-populating ClipGroups" << endl;
        
        // make copies from "original" to "all" groups so we can pop clipGroups as we go
        appModel->getClipGroupReference("allClips") = appModel->getClipGroup("originalClips");
        appModel->getClipGroupReference("allListenClips") = appModel->getClipGroup("originalListenClips");
        appModel->getClipGroupReference("allIntroClips") = appModel->getClipGroup("originalIntroClips");
        appModel->getClipGroupReference("allStatClips") = appModel->getClipGroup("originalStatClips");
        appModel->getClipGroupReference("allSpecClips") = appModel->getClipGroup("originalSpecClips");
        appModel->getClipGroupReference("allCategoryClips") = appModel->getClipGroup("originalCategoryClips");
        
    }
}
