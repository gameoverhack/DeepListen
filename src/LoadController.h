//
//  LoadController.h
//  DeepListen
//
//  Created by game over on 1/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_LOADCONTROLLER
#define _H_LOADCONTROLLER

#include "BaseController.h"
#include "AppModel.h"

class Clips{
public:
    Clips(){};
    ~Clips(){};
    
    void save(){
        Serializer.saveClass("clips_only", (*this), ARCHIVE_BINARY);
    };
    
    void load(){
        Serializer.loadClass("clips_only", (*this), ARCHIVE_BINARY);
    };
    
    map<string, Clip> clips;
    
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
        ar & BOOST_SERIALIZATION_NVP(clips);
    };
    
};

class LoadController : public BaseController {

public:
	
    LoadController();
    ~LoadController();
    
    void setup();
    void update();
    
protected:
	
private:
	
};

#endif
