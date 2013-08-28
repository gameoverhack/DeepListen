//
//  NetworkController.h
//  DeepListen
//
//  Created by gameover on 11/07/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//
//

#ifndef _H_NETWORKCONTROLLER
#define _H_NETWORKCONTROLLER

#include "BaseController.h"
#include "SoundController.h"
#include "AppModel.h"

#include "ofxNetwork.h"

class NetworkController : public BaseController {
    
public:
	
    NetworkController();
    ~NetworkController();
    
    void setup();
    void update();
    
protected:
    
    void send(int client, string msg);
    string recieve(int client);
    
    ofxTCPServer server;
    int heartBeatTime;
    int heartBeatLast;
    
private:
	
};

#endif
