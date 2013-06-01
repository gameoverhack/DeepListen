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
            StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");
            ClipTimeline & clipTimeline = appModel->getClipTimeline();

            clipTimeline.update();
            
            if(debugViewStates.getState(kDEBUGVIEW_SHOWPROPS)){
                vector<VideoClip>& videoClips = clipTimeline.getVideoClips();
                for(int i = 0; i < videoClips.size(); i++){
                    ofxThreadedVideo * video = videoClips[i].video;
                    ostringstream os;
                    os << ofToString(video->isLoading()) << " " << ofToString(video->isPlaying()) << " " << ofToString(video->getIsMovieDone()) << " " << ofToString(video->getFrameRate()) << " " << videoClips[i].clip;
                    appModel->setProperty("PlayState_"+ofToString(i), os.str());
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
            }

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
    string question = "";
    int flip;
    int lastAudioFreeFrame = 0;
    
//    ClipTimeline & timeline = appModel->getClipTimeline();
//    
//    for (int i = 0; i < 10000; i++){
//        bool insertedOk = false;
//        int r = ofRandom(5);
//        Clip & clip = allIntroClips[r];
//        lastAudioFreeFrame = lastAudioFreeFrame + ofRandom(100);
//        // calculate frame starts and ends
//        insertedOk = timeline.insertClipAt(clip, lastAudioFreeFrame);
//    }
//    playControllerStates.setState(kPLAYCONTROLLER_PLAY);
    
    int respMax = 0;
    int statMax = 0;
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
            
            int remainingNumCategories = allCategoryTypes.size();

            if (remainingNumCategories < 4){ // 5 is all categories
                // reset the clip groups by changing state
                playControllerStates.setState(kPLAYCONTROLLER_PLAY);
                return;// newClipGroup;
            }
            
            // could use a random distribution algorithm here instead
            int numberOfResponses = (int)ofRandom(3) + 3; // min 2; max 5
            int numberOfStatements = (int)ofRandom(2) + 2; // min 2; max 3
            
            respMax = MAX(respMax, numberOfResponses);
            statMax = MAX(statMax, numberOfStatements);
            
            cout << "Response max: " << respMax << " Statement max: " << statMax << endl;
            
            int maxAttempts = 20;
            int attempts;
            
            attempts = 0;
            bool categoryOk = false;
            while(!categoryOk && attempts < maxAttempts){
                category = allCategoryTypes.getrandom();
                ofxLogVerbose() << "Checking time for category: " << category << endl;
                if(appModel->getTimer(category, framesToMillis(lastAudioFreeFrame), minutesToMillis(30))){
                    allCategoryTypes.popall(category);
                    categoryOk = true;
                }
                if(attempts == maxAttempts - 1){
                    ofxLogVerbose() << "Attempt fail category: " << attempts << " " << maxAttempts << endl;
                    int maxDifference = -INFINITY;
                    map<int, string> differences;
                    for(int k = 0; k < allCategoryTypes.size(); k++){
                        category = allCategoryTypes[k];
                        int difference = appModel->getTimerDifference(category, framesToMillis(lastAudioFreeFrame));
                        differences[difference] = category;
                        maxDifference = MAX(maxDifference, difference);
                    }
                    category = differences[maxDifference];
                    ofxLogVerbose() << "Choose max difference for category: " << category << " " << maxDifference << endl;
                }
                attempts++;
            }
            
            ClipGroup categoryGroup = allClips.getContains(CATEGORY, category);
            ClipType questionTypes = categoryGroup.getClipTypes(QUESTION);

            attempts = 0;
            bool questionOk = false;
            while(!questionOk && attempts < maxAttempts){
                question = questionTypes.getrandom();
                ofxLogVerbose() << "Checking time for question: " << question << endl;
                if(appModel->getTimer(question, framesToMillis(lastAudioFreeFrame), minutesToMillis(30))){
                    questionTypes.popall(question);
                    questionOk = true;
                }
                if(attempts == maxAttempts - 1){
                    ofxLogVerbose() << "Attempt fail question: " << attempts << " " << maxAttempts << endl;
                    int maxDifference = -INFINITY;
                    map<int, string> differences;
                    for(int k = 0; k < questionTypes.size(); k++){
                        question = questionTypes[k];
                        int difference = appModel->getTimerDifference(question, framesToMillis(lastAudioFreeFrame));
                        differences[difference] = question;
                        maxDifference = MAX(maxDifference, difference);
                    }
                    question = differences[maxDifference];
                    ofxLogVerbose() << "Choose max difference for question: " << question << " " << maxDifference << endl;
                }
                attempts++;
            }
            
            ClipTimeline & timeline = appModel->getClipTimeline();
            
            // insert a TITLE here
            
            
            // insert statements
            ClipGroup statementGroup = allStatClips.getStatements(category);

            for(int j = 0; j < MIN(statementGroup.size(), numberOfStatements); j++){ // make sure we don't exceed statementGroup size!
                
                bool personOk = false;
                attempts = 0;
                while(!personOk && attempts < maxAttempts){
                    Clip clip = statementGroup.getrandom();
                    string person = clip.getClipInfo().person;
                    ofxLogVerbose() << "Checking person for statement type: " << getStatementQuestionFromCategory(category) << endl;
                    if(appModel->getTimer(person, framesToMillis(lastAudioFreeFrame), minutesToMillis(5)) || attempts == maxAttempts - 1){
                        if(attempts == maxAttempts - 1){
                            ofxLogVerbose() << "Attempt fail statement: " << attempts << " " << maxAttempts << endl;
                            int maxDifference = -INFINITY;
                            map<int, Clip> differences;
                            for(int k = 0; k < statementGroup.size(); k++){
                                clip = statementGroup.at(k);
                                person = clip.getClipInfo().person;
                                int difference = appModel->getTimerDifference(person, framesToMillis(lastAudioFreeFrame));
                                differences[difference] = clip;
                                maxDifference = MAX(maxDifference, difference);
                            }
                            clip = differences[maxDifference];
                            person = clip.getClipInfo().person;
                            ofxLogVerbose() << "Choose max difference for statement: " << person << " " << maxDifference << endl;
                        }
                        int insertFrame = lastAudioFreeFrame - clip.getAudioInFrameOffset();
                        ofxLogVerbose() << "Attempting to insert clip at: " << insertFrame << endl;
                        if(timeline.insertClipAt(clip, insertFrame)){
                            ofxLogVerbose() << "Inserted: " << timeline.getLastClip() << endl;
                            lastAudioFreeFrame = timeline.getLastClip().getAudioEnd();
                            appModel->setTimer(person, framesToMillis(timeline.getLastClip().getVideoEnd()));
                            statementGroup.pop(clip);
                            personOk = true;
                        }else{
                            ofxLogWarning() << "Rejected: " << clip << endl;
                            statementGroup.pop(clip);
                        }
                    }
                    attempts++;
                    cout << "attempts: " << attempts << endl;
                }
            }
            
            // insert responses
            ClipGroup questionGroup = categoryGroup.getContains(QUESTION, question);
            
            int responseLength = lastAudioFreeFrame;
            int responseCount = 0;
            
            for(int j = 0; j < MIN(questionGroup.size(), numberOfResponses); j++){ // make sure we don't exceed questionGroup size!
                
                bool personOk = false;
                attempts = 0;
                while(!personOk && attempts < maxAttempts){
                    Clip clip = questionGroup.getrandom();
                    string person = clip.getClipInfo().person;
                    ofxLogVerbose() << "Checking person for response type: " << question << endl;
                    if(appModel->getTimer(person, framesToMillis(lastAudioFreeFrame), minutesToMillis(5)) || attempts == maxAttempts - 1){
                        if(attempts == maxAttempts - 1){
                            ofxLogVerbose() << "Attempt fail response: " << attempts << " " << maxAttempts << endl;
                            int maxDifference = -INFINITY;
                            map<int, Clip> differences;
                            for(int k = 0; k < questionGroup.size(); k++){
                                clip = questionGroup.at(k);
                                person = clip.getClipInfo().person;
                                int difference = appModel->getTimerDifference(person, framesToMillis(lastAudioFreeFrame));
                                differences[difference] = clip;
                                maxDifference = MAX(maxDifference, difference);
                            }
                            clip = differences[maxDifference];
                            person = clip.getClipInfo().person;
                            ofxLogVerbose() << "Choose max difference for response: " << person << " " << maxDifference << endl;
                        }

                        int insertFrame = lastAudioFreeFrame - clip.getAudioInFrameOffset();
                        ofxLogVerbose() << "Attempting to insert clip at: " << insertFrame << endl;
                        if(timeline.insertClipAt(clip, insertFrame)){
                            ofxLogVerbose() << "Inserted: " << timeline.getLastClip() << endl;
                            lastAudioFreeFrame = timeline.getLastClip().getAudioEnd();
                            responseLength += lastAudioFreeFrame;
                            responseCount++;
                            appModel->setTimer(person, framesToMillis(timeline.getLastClip().getVideoEnd()));
                            questionGroup.pop(clip);
                            allClips.pop(clip);
                            personOk = true;
                        }else{
                            ofxLogWarning() << "Rejected: " << clip << endl;
                            questionGroup.pop(clip);
                        }
                    }
                    attempts++;
                    cout << "attempts: " << attempts << endl;
                }
            }
            
            // insert LISTENERS here
            cout << "Response count: " << responseCount << " " << framesToMinutes(responseLength) << endl;
            if(framesToMinutes(responseLength) > 2.0){
                cout << "Insert listener" << endl;
            }
            
            appModel->setTimer(category, framesToMillis(timeline.getLastClip().getVideoEnd()));
            appModel->setTimer(question, framesToMillis(timeline.getLastClip().getVideoEnd()));
            
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
