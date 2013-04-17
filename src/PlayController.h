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
    
    ClipGroup makeClipGroup();
    void resetClipGroups();
    
protected:
	
    ClipGroup dummyClipGroup;
    
private:
	
};

#endif