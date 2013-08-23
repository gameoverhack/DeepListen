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
    newPlayControllerStates.addState(State(kPLAYCONTROLLER_STOP, "kPLAYCONTROLLER_STOP"));
    
    appModel->addStateGroup(newPlayControllerStates);

    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    playControllerStates.setState(kPLAYCONTROLLER_INIT);
    
}
multimap<int, int> stats;
//--------------------------------------------------------------
void PlayController::update(){
    
    StateGroup & appControllerStates = appModel->getStateGroup("AppControllerStates");
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    switch (playControllerStates.getState()) {
        case kPLAYCONTROLLER_INIT:
        {
            
            ofxLogNotice() << "PLAYCONTROLLER INIT" << endl;
            
            if(timeline.getCurrentFrame() > timeline.getTotalFrames()){
                playControllerStates.setState(kPLAYCONTROLLER_MAKE);
            }else{
                TimeLineHistory& timneLineHistory = appModel->loadTimelineHistory();
                playControllerStates.setState(kPLAYCONTROLLER_PLAY);
            }
            
        }
            break;
        case kPLAYCONTROLLER_MAKE:
        {
            
            ofxLogNotice() << "PLAYCONTROLLER MAKE" << endl;
            
            timeline.clear();
            timeline.setMusicAssets(appModel->getMusicAssetLoader());
            resetClipGroups();
            
            makeClipGroup();

            ofxLogVerbose() << "\\/---------------------\\/" << endl;
            ofxLogVerbose() << timeline.getGroup() << endl;
            ofxLogVerbose() << "/\\---------------------/\\" << endl;
            
            ofxLogVerbose() << "Total length: " << framesToMinutes(timeline.getTotalFrames()) << " num clips: " << timeline.getGroup().size() << endl;
            
            ClipGroup & statistics = appModel->getClipGroupReference("statistics");
            statistics.push(timeline.getGroup());
            
            timeline.play();
            
            playControllerStates.setState(kPLAYCONTROLLER_PLAY);
            
        }
            break;
        case kPLAYCONTROLLER_PLAY:
        {
            
            ofxLogNotice() << "PLAYCONTROLLER PLAY" << endl;
            
            StateGroup & debugViewStates = appModel->getStateGroup("DebugViewStates");

            timeline.update();
            
            if(debugViewStates.getState(kDEBUGVIEW_SHOWPROPS)){
                vector<VideoClip>& videoClips = timeline.getVideoClips();
                for(int i = 0; i < videoClips.size(); i++){
                    ofxThreadedVideo * video = videoClips[i].video;
                    ostringstream os;
                    os << ofToString(video->isLoading()) << " " << ofToString(video->isPlaying()) << " " << ofToString(video->getIsMovieDone()) << " " << ofToString(video->getFrameRate()) << " " << video->getCurrentFrame() << "/" << video->getTotalNumFrames() << " " << video->getQueueSize() << " "  << videoClips[i].clip;
                    appModel->setProperty("PlayStateV_"+ofToString(i), os.str());
                }
                
                vector<AudioClip>& audioClips = timeline.getAudioClips();
                for(int i = 0; i < audioClips.size(); i++){
                    ofxThreadedVideo * musicVideo = audioClips[i].music;
                    ostringstream os;
                    os << ofToString(musicVideo->isLoading()) << " " << ofToString(musicVideo->isPlaying()) << " " << ofToString(musicVideo->getIsMovieDone()) << " " << ofToString(musicVideo->getFrameRate()) << " " << musicVideo->getMovieName() << " " << musicVideo->getCurrentFrame() << "/" << musicVideo->getTotalNumFrames() << " " << ofToString(musicVideo->getVolume()) << " " << musicVideo->getQueueSize();
                    appModel->setProperty("PlayStateA_"+ofToString(i), os.str());
                }
                
                ostringstream os;
                vector<Clip> & currentClips = timeline.getCurrentClips();
                for(int i = 0; i < currentClips.size(); i++){
                    Clip & clip = currentClips[i];
                    os << clip.getName() << " " << clip.getClipStopping() << " " << (i == currentClips.size() - 1 ? "" : ", ");
                }
                
                appModel->setProperty("Clips", os.str());
                
                appModel->setProperty("TimeFrame", timeline.getCurrentFrame());
                appModel->setProperty("TimeFrames", timeline.getTotalFrames());
            }

            if(timeline.getCurrentFrame() > timeline.getTotalFrames()){
                playControllerStates.setState(kPLAYCONTROLLER_INIT);
            }
            
        }
            break;
        case kPLAYCONTROLLER_STOP:
        {
            if(timeline.getGroup().size() > 0) timeline.clear();
        }
            break;
    }
    
}

//--------------------------------------------------------------
void PlayController::makeClipGroup(){
    
    ofxLogNotice() << "Making new ClipGroup for playback" << endl;
    
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    ClipGroup & allClips = appModel->getClipGroupReference("allClips");
    ClipGroup & allListenClips = appModel->getClipGroupReference("allListenClips");
    ClipGroup & allStatClips = appModel->getClipGroupReference("allStatClips");
    ClipGroup & allIntroClips = appModel->getClipGroupReference("allIntroClips");
    ClipGroup & allSpecClips = appModel->getClipGroupReference("allSpecClips");
    
    ClipGroup & allTitleClips = appModel->getClipGroupReference("allTitleClips");
    ClipGroup & statistics = appModel->getClipGroupReference("statistics");
    
    string category = "";
    string question = "";
    lastAudioFreeFrame = 0;
    int lastResetFrame = 0;
    vector<int> blackspace;
    
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
    int respMin = 20;
    int statMin = 20;
    int maxAttempts = 20;
    int attempts;
    
    int specialCount = 0;
    int introCount = 4;
    lastPerson = "";
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    
    while(true){

        if(introCount == 4){
            
            ofxLogNotice() << "INTRO CLIP INSERT" << endl;
            
            introCount = 0;
            
            // insert a TITLE here
            Clip title = allTitleClips.getContains(CATEGORY, "CONN").getrandom();
            
            int titleInsertFrame;
            if(timeline.getGroup().size() > 0){
                Clip lastClip = timeline.getLastClip();
                titleInsertFrame = lastClip.getAudioEnd() - secondsToFrames(12);
            }else{
                titleInsertFrame = 0;
                //title.setCrop(12 * 25, title.getTotalFrames());
            }
            
            if(timeline.insertClipAt(title, titleInsertFrame)){
                ofxLogVerbose() << "Inserted title: " << timeline.getLastClip() << endl;
                lastAudioFreeFrame = timeline.getLastClip().getVideoStart() + secondsToFrames(27);
            }else{
                ofxLogWarning() << "Rejected title: " << title << endl;
            }
            
            // insert intros
//            int numberOfIntros = floor((int)ofRandom(3)) + 4;
//            ClipGroup introGroup = allIntroClips;
//            ClipGroup theseIntroPeople = getGroupSelectionFrom(introGroup, numberOfIntros, 5);
            
            int numberOfStatements = floor((int)ofRandom(2)) + 4; // 6
            ClipGroup statementGroup = allStatClips;
            ClipGroup theseStatementPeople = getGroupSelectionFrom(statementGroup, numberOfStatements, 5);
            
        }else if(specialCount == 7){
            ofxLogNotice() << "SPECIAL CLIP INSERT" << endl;
            specialCount = 0;
            
            ClipGroup specialGroup = allSpecClips;
            ClipGroup specialPerson = getGroupSelectionFrom(specialGroup, 1, 30);
            
        }else{// do a category
            
            ofxLogNotice() << "CATEGORY CLIP INSERT" << endl;
            
            ClipType allCategoryTypes = allClips.getClipTypes(CATEGORY);
            
            allCategoryTypes.popall("LSTN");
            allCategoryTypes.popall("SPEC");
            allCategoryTypes.popall("STAT");
            allCategoryTypes.popall("INTR");
            allCategoryTypes.popall(category);
            
            int remainingNumCategories = allCategoryTypes.size();

            if (remainingNumCategories < 4){ // 5 is all categories
                // reset the clip groups by changing state
                blackspace.push_back(timeline.getTotalFrames());
                appModel->setProperty("BlackSpace", blackspace);
                ostringstream os;
                os << blackspace;
                ofxLogNotice() << "ALLBLACKSPACE: " << os.str() << endl;
                return;// newClipGroup;
            }
            
            // could use a random distribution algorithm here instead
            int numberOfResponses = floor((int)ofRandom(2)) + 2; // min 2; max 3
            int numberOfStatements = floor((int)ofRandom(3)) + 2; // min 2; max 4
            
            respMax = MAX(respMax, numberOfResponses);
            statMax = MAX(statMax, numberOfStatements);
            respMin = MIN(respMin, numberOfResponses);
            statMin = MIN(statMin, numberOfStatements);
            
            cout << "Response min: " << respMin << " Statement min: " << statMin << endl;
            cout << "Response max: " << respMax << " Statement max: " << statMax << endl;
            
            attempts = 0;
            bool categoryOk = false;
            while(!categoryOk && attempts < maxAttempts){
                category = allCategoryTypes.getrandom();
                ofxLogVerbose() << "Checking time for category: " << category << endl;
                if(appModel->getTimer(category, framesToMillis(lastAudioFreeFrame), minutesToMillis(15))){
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
                    ofxLogVerbose() << "Choose max difference for category: " << category << " " << millisToMinutes(maxDifference) << endl;
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
                    ofxLogVerbose() << "Choose max difference for question: " << question << " " << millisToMinutes(maxDifference) << endl;
                }
                attempts++;
            }

            // insert a TITLE here
            Clip title = allTitleClips.getContains(CATEGORY, category).getrandom();
            
            int titleInsertFrame;
            if(timeline.getGroup().size() > 0){
                Clip lastClip = timeline.getLastClip();
                
                if(lastClip.getClipInfo().category == "SPEC" && framesToMinutes(timeline.getLastClip().getVideoEnd() - lastResetFrame) > 120){
                    ofxLogNotice() << "INSERT BLACKSPACE RESTART at " << timeline.getLastClip().getVideoEnd() << " after " << framesToMinutes(timeline.getLastClip().getVideoEnd() - lastResetFrame) << endl;
                    lastResetFrame = timeline.getLastClip().getVideoEnd();
                    blackspace.push_back(lastResetFrame);
                    titleInsertFrame = lastResetFrame + minutesToFrames(5);
                }else{
                    titleInsertFrame = lastClip.getAudioEnd() - secondsToFrames(12);
                }
                
            }else{
                titleInsertFrame = 0;
                //title.setCrop(12 * 25, title.getTotalFrames());
            }
            
            if(timeline.insertClipAt(title, titleInsertFrame)){
                ofxLogVerbose() << "Inserted title: " << timeline.getLastClip() << endl;
                lastAudioFreeFrame = timeline.getLastClip().getVideoStart() + secondsToFrames(27);
            }else{
                ofxLogWarning() << "Rejected title: " << title << endl;
            }
            
            // insert statements
            ClipGroup statementGroup = allStatClips.getStatements(category);
            ClipGroup theseStatementPeople = getGroupSelectionFrom(statementGroup, numberOfStatements, 5);
            
            // insert responses
            ClipGroup questionGroup = categoryGroup.getContains(QUESTION, question);
            
            for(int j = 0; j < theseStatementPeople.size(); j++){
                questionGroup = questionGroup.getExcludes(PERSON, theseStatementPeople[j].getClipInfo().person);
            }
            
            ClipGroup theseResponsePeople = getGroupSelectionFrom(questionGroup, numberOfResponses, 5, 3);
            allClips.pop(theseResponsePeople);
            
//            int responseCount = theseResponsePeople.size();
//            if(responseCount > 0){
//                
//                int responseLength = theseResponsePeople[responseCount - 1].getVideoEnd() - theseResponsePeople[0].getVideoStart();
//                
//                // insert LISTENERS here
//                ofxLogVerbose() << "Response count: " << responseCount << " " << framesToMinutes(responseLength) << endl;
//                if(framesToMinutes(responseLength) > 3.0){
//                    
//                    ClipGroup listenerGroup = allListenClips;
//                    
//                    ofxLogVerbose() << "Insert listeners from: " << listenerGroup.size() << endl;
//                    
//                    for(int j = 0; j < theseStatementPeople.size(); j++){
//                        listenerGroup = listenerGroup.getExcludes(PERSON, theseStatementPeople[j].getClipInfo().person);
//                    }
//                    for(int j = 0; j < theseResponsePeople.size(); j++){
//                        listenerGroup = listenerGroup.getExcludes(PERSON, theseResponsePeople[j].getClipInfo().person);
//                    }
//                    
//                    for(int j = 0; j < listenerGroup.size(); j++){
//                        Clip & clip = listenerGroup[j];
//                        string person = clip.getClipInfo().person;
//                        if(!appModel->getTimer(person, framesToMillis(lastAudioFreeFrame), minutesToMillis(5))) listenerGroup.pop(clip);
//                    }
//                    
//                    ofxLogVerbose() << "Number of listeners: " << listenerGroup.size() << endl;
//                    
//                    ClipGroup leftListeners;
//                    ClipGroup l1 = listenerGroup.getContains(QUESTION, "LFPR");
//                    ClipGroup l2 = listenerGroup.getContains(QUESTION, "LFTR");
//                    leftListeners.push(l1);
//                    leftListeners.push(l2);
//                    
//                    ClipGroup rightListeners;
//                    ClipGroup r1 = listenerGroup.getContains(QUESTION, "RIPR");
//                    ClipGroup r2 = listenerGroup.getContains(QUESTION, "RITR");
//                    rightListeners.push(l1);
//                    rightListeners.push(l2);
//                    
//                    ClipGroup frontListeners = listenerGroup.getContains(QUESTION, "FRON");
//                    
//                    int lastListenerFrame = theseResponsePeople[0].getVideoStart() + ((int)ofRandom(30) * 25) - (15 * 25);
//                    int listenerCount = 0;
//                    for(int j = 0; j < theseResponsePeople.size(); j++){
//                        Clip & clip = theseResponsePeople[j];
//                        ofxLogVerbose() << clip << endl;
//                        ofxLogVerbose() << "Clip is on " << (clip.getScreen() == LEFT ? "LEFT" : "RIGHT") << " screen" << endl;
//                        ofxLogVerbose() << "Clip is on " << (clip.getSide() == LEFT ? "LEFT" : "RIGHT") << " side" << endl;
//                        int tryScreen = (clip.getScreen() == LEFT ? RIGHT : LEFT);
//                        int xMin = -1;
//                        int xMax = -1;
//                        Clip listener;
//                        if((clip.getSide() == LEFT)){
//                            listener = leftListeners.getrandom();
//                        }else if((clip.getSide() == RIGHT)){
//                            listener = rightListeners.getrandom();
//                        }
//                        
//                        if((clip.getSide() == LEFT && tryScreen == clip.getScreen())){
//                            xMin = clip.getPosition().x + clip.getPosition().width;
//                        }else if((clip.getSide() == RIGHT && tryScreen == clip.getScreen())){
//                            xMax = clip.getPosition().x;
//                        }
//                        
//                        ofxLogVerbose() << "Listener attempt: " << xMin << " " << xMax << " " << listener << endl;
//                        cout << theseResponsePeople[j].getTotalFrames() << " " << lastListenerFrame << " " << listener.getTotalFrames() << endl;
//                        listener.setCrop(0, MIN(theseResponsePeople[j].getTotalFrames() + lastListenerFrame, listener.getTotalFrames()));
//                        if(timeline.insertClipAt(listener, lastListenerFrame, tryScreen, xMin, xMax)){
//                            ofxLogVerbose() << "Listener inserted: " << listener << endl;
//                            lastListenerFrame = listener.getVideoEnd();
//                            listenerCount++;
//                        }
//                        if(lastListenerFrame > theseResponsePeople[responseCount - 1].getVideoEnd() || listenerCount > 0){
//                            ofxLogVerbose() << "Bailing listeners" << endl;
//                            break;
//                        }
//                    }
//                }
//            }else{
//                ofxLogVerbose() << "No respondents" << endl;
//            }
            
            appModel->setTimer(category, framesToMillis(timeline.getLastClip().getVideoEnd()));
            appModel->setTimer(question, framesToMillis(timeline.getLastClip().getVideoEnd()));
            
            introCount++;
            specialCount++;
            
        }
    }

}

//--------------------------------------------------------------
ClipGroup PlayController::getGroupSelectionFrom(ClipGroup group, int maxNumClips, int targetMinsBetweenPeeps, int targetMaxMins){
    
    ClipTimeline & timeline = appModel->getClipTimeline();
    ClipGroup thisSelection;
    int startAudioFreeFrame = lastAudioFreeFrame;
    
    for(int j = 0; j < MIN(group.size(), maxNumClips); j++){ // make sure we don't exceed statementGroup size!
        
        bool personOk = false;
        int attempts = 0;
        int maxAttempts = 20;
        
        if(targetMaxMins != -1){
            ofxLogVerbose() << "Check target minutes" << endl;
            if(framesToMinutes(lastAudioFreeFrame - startAudioFreeFrame) > targetMaxMins){
                ofxLogVerbose() << "Aborting selection as we're over target minutes: " << targetMaxMins << endl;
                break;
            }
        }
        
        while(!personOk && attempts < maxAttempts){
            Clip clip = group.getrandom();
            string person = clip.getClipInfo().person;
            ofxLogVerbose() << "Checking person: " << person << endl;
            if(appModel->getTimer(person, framesToMillis(lastAudioFreeFrame), minutesToMillis(targetMinsBetweenPeeps)) || attempts == maxAttempts - 1){
                if(attempts == maxAttempts - 1){
                    ofxLogVerbose() << "Attempt fail statement: " << attempts << " " << maxAttempts << endl;
                    int maxDifference = -INFINITY;
                    map<int, Clip> differences;
                    for(int k = 0; k < group.size(); k++){
                        clip = group.at(k);
                        person = clip.getClipInfo().person;
                        int difference = appModel->getTimerDifference(person, framesToMillis(lastAudioFreeFrame));
                        differences[difference] = clip;
                        maxDifference = MAX(maxDifference, difference);
                    }
                    clip = differences[maxDifference];
                    person = clip.getClipInfo().person;
                    ofxLogVerbose() << "Choose max difference for person: " << person << " " << millisToMinutes(maxDifference) << endl;
                    if(person == lastPerson){
                        ofxLogVerbose() << "Same last person: " << clip << endl;
                        break;
                    }
                }
                int insertFrame = lastAudioFreeFrame - clip.getAudioInFrameOffset();
                ofxLogVerbose() << "Attempting to insert clip at: " << insertFrame << endl;
                if(timeline.insertClipAt(clip, insertFrame)){
                    ofxLogVerbose() << "Inserted: " << timeline.getLastClip() << endl;
                    lastAudioFreeFrame = timeline.getLastClip().getAudioEnd();
                    appModel->setTimer(person, framesToMillis(timeline.getLastClip().getVideoEnd()));
                    group.pop(clip);
                    thisSelection.push(clip);
                    lastPerson = person;
                    personOk = true;
                }else{
                    ofxLogWarning() << "Rejected: " << clip << endl;
                    group.pop(clip);
                }
            }
            
            attempts++;
            ofxLogVerbose() << "Selection attempts: " << attempts << endl;
            
        }
    }
    return thisSelection;
}

//--------------------------------------------------------------
void PlayController::resetClipGroups(){
    
    ofSeedRandom();
    appModel->resetTimers();
    
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
            if(!clip.getDeleted()){
                cout << "USING    : " << clip << endl; 
                originalClips.push(clip);
            }else{
                cout << "NOT USING: " << clip << endl; 
            }
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
        
        cout << "CLIPSTATISTICS" << endl;
        
        ClipType categoryTypes = originalClips.getClipTypes(CATEGORY);
        
        cout << "Total clips: " << originalClips.size() << " + " << allTitleClips.size() << " of " << appModel->getVideoAssetLoader().size() << endl << endl;
        
        printTimes(originalClips);
        
        cout << endl << "CATSTATISTICS" << endl << endl;
        
        for(int i = 0; i < categoryTypes.size(); i++){
            cout << categoryTypes[i] << " [" << getCategoryFromCode(categoryTypes[i]) << "] " << categoryTypes[categoryTypes[i]] << " clips"<< endl;
            ClipGroup categoryGroup = originalClips.getContains(CATEGORY, categoryTypes[i]);
            cout << endl;
            printTimes(categoryGroup);
            cout << endl;
            ClipType questionTypes = originalClips.getContains(CATEGORY, categoryTypes[i]).getClipTypes(QUESTION);
            for(int j = 0; j < questionTypes.size(); j++){
                cout << "   " << questionTypes[j] << " [" << getQuestionFromCode(categoryTypes[i], questionTypes[j]) << "] " << questionTypes[questionTypes[j]] << endl;
            }
            cout << endl;
        }
        
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

void PlayController::printTimes(ClipGroup & group){
    int minLength = INFINITY;
    int maxLength = 0;
    
    for(int i = 0; i < group.size(); i++){
        minLength = MIN(minLength, group[i].getTotalFrames());
        maxLength = MAX(maxLength, group[i].getTotalFrames());
    }
    
    int deltaLength = maxLength - minLength;
    int bins = ceil((float) deltaLength/1500);
    if(bins < 0) return;
    vector<int> counts;
    vector<float> times;
    times.resize(bins + 1);
    counts.assign(bins, 0);
    
    for(int i = 0; i < bins + 1; i++){
        times[i] = (float)(minLength + ((float)deltaLength/bins) * i);
    }
    
    for(int j = 0; j < group.size(); j++){
        for(int i = 0; i < bins; i++){
            if(group[j].getTotalFrames() > times[i] &&
               group[j].getTotalFrames() <= times[i + 1]){
                counts[i]++;
            }
        }
    }

    //cout << "   min length: " << Poco::DateTimeFormatter::format(Poco::Timespan(framesToMillis(minLength)/1000.0f, 0), "%M:%S") << " max length: " << Poco::DateTimeFormatter::format(Poco::Timespan(framesToMillis(maxLength)/1000.0f, 0), "%M:%S") << " bin size: " << Poco::DateTimeFormatter::format(Poco::Timespan(framesToMillis((float)deltaLength/bins)/1000.0f, 0), "%M:%S") << endl;
    
    for(int i = 0; i < bins; i++){
        cout << "   " << Poco::DateTimeFormatter::format(Poco::Timespan(framesToMillis(times[i + 0])/1000.0f, 0), "%M:%S") << " - "
             << Poco::DateTimeFormatter::format(Poco::Timespan(framesToMillis(times[i + 1])/1000.0f, 0), "%M:%S") << " (mins) == "
             << counts[i] << endl;
    }
}
