//
//  AppController.h
//
//  Created by game over on 25/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_APPCONTROLLER
#define _H_APPCONTROLLER

#include "BaseController.h"
#include "AppModel.h"
#include "AppView.h"
#include "DebugView.h"
#include "AnalyzeView.h"
#include "LoadController.h"
#include "AnalyzeController.h"
#include "PlayController.h"
#include "SoundController.h"

class AppController : public BaseController {
    
public:
	
    AppController();
    ~AppController();
    
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(ofKeyEventArgs & e);
    void keyReleased(ofKeyEventArgs & e);
    
    void mouseMoved(ofMouseEventArgs & e);
    void mouseDragged(ofMouseEventArgs & e);
    void mousePressed(ofMouseEventArgs & e);
    void mouseReleased(ofMouseEventArgs & e);
    
//    void keyPressed(int key);
//    void keyReleased(int key);
//    void mouseMoved(int x, int y);
//    void mouseDragged(int x, int y, int button);
//    void mousePressed(int x, int y, int button);
//    void mouseReleased(int x, int y, int button);
    
protected:
	
private:
	
    float w0x, w0y, w1x, w1y;
    
    float mouseDownX;
    float mouseDownY;
    
    LoadController * loadController;
    AnalyzeController * analyzeController;
    PlayController * playController;
    
    AppView * appViews[2];
    DebugView * debugView;
    AnalyzeView * analyzeView;

    int soundTimeout;
    
    bool bShowCursor;
    
};

#endif
