//
//  NetworkController.cpp
//  DeepListen
//
//  Created by gameover on 11/07/13.
//
//

#include "NetworkController.h"

//--------------------------------------------------------------
NetworkController::NetworkController(){
    ofxLogVerbose() << "Creating NetworkController" << endl;
}

//--------------------------------------------------------------
NetworkController::~NetworkController(){
    ofxLogVerbose() << "Destroying NetworkController" << endl;
}

//--------------------------------------------------------------
void NetworkController::setup(){
    server.setup(6666);
    server.setMessageDelimiter("\n");
    heartBeatLast = ofGetElapsedTimeMillis();
    heartBeatTime = 2000;
}

//--------------------------------------------------------------
void NetworkController::update(){
    
    StateGroup & playControllerStates = appModel->getStateGroup("PlayControllerStates");
    
    for(int i = 0; i < server.getLastID(); i++){
        
        if(!server.isClientConnected(i)) continue;
        
        if(ofGetElapsedTimeMillis() - heartBeatLast >= heartBeatTime){
            send(i, "deep");
            heartBeatLast = ofGetElapsedTimeMillis();
        }
        
        vector<string> command = ofSplitString(recieve(i), ":");
        
        if(command.size() > 0){
            
            if(command[0] == "start"){
                ofxLogVerbose() << "start" << endl;
                playControllerStates.setState(kPLAYCONTROLLER_INIT);
                send(i, command[0]);
            }
            
            if(command[0] == "stop"){
                ofxLogVerbose() << "stop" << endl;
                playControllerStates.setState(kPLAYCONTROLLER_STOP);
                send(i, command[0]);
            }
            
            if(command[0] == "volume" && command.size() > 1){
                ofxLogVerbose() << "volume:" << command[1] << endl;
                float volume = CLAMP(ofToFloat(command[1]), 0.0f, 100.0f);
                soundController->setMasterVolume(volume/100.0f);
                send(i, command[0] + ":" + command[1]);
            }
            
            if(command[0] == "log" && command.size() > 1){
                ofxLogVerbose() << "log:" << command[1] << endl;
                if(command[1] == "file"){
                    appModel->setProperty("LogToFile", true);
                }else{
                    appModel->setProperty("LogToFile", false);
                }
                ofxLogSetLogToFile(appModel->getProperty<bool>("LogToFile"), ofToDataPath("log_" + ofGetTimestampString() + ".txt"));
                send(i, command[0] + ":" + command[1]);
            }
            
        }
    }

}

//--------------------------------------------------------------
void NetworkController::send(int client, string msg){
    //ofxLogVerbose() << "Send: " << msg << " to " << server.getClientIP(client) << ":" << server.getClientPort(client) << endl;
    server.send(client, msg + "\n"); //:"+ofToString(server.getClientPort(i)) );
}

//--------------------------------------------------------------
string NetworkController::recieve(int client){
    string msg = server.receive(client);
    if(msg.length() > 0){
        ofxLogVerbose() << "Recieve: " << msg << " from " << server.getClientIP(client) << ":" << server.getClientPort(client) << endl;
        return msg;
    }else{
        return "";
    }
}
