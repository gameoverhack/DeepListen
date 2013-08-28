//
//  PlayController.h
//  DeepListen
//
//  Created by game over on 3/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_PLAYCONTROLLER
#define _H_PLAYCONTROLLER

#include "BaseController.h"
#include "AppModel.h"

class PlayController : public BaseController {
    
public:
	
    PlayController();
    ~PlayController();
    
    void setup();
    void update();
    
    void makeClipGroup();
    void resetClipGroups();
    void printTimes(ClipGroup & group);
    
protected:
	
    ClipGroup getGroupSelectionFrom(ClipGroup group, int maxNumClips, int targetMinsBetweenPeeps, int targetMaxMins = -1);
    
    ClipGroup dummyClipGroup;
    int lastAudioFreeFrame;
    string lastPerson;
    
private:
	
};

#endif