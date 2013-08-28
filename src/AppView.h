//
//  AppView.h
//  DeepListen
//
//  Created by game over on 31/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_APPVIEW
#define _H_APPVIEW

#include "AppModel.h"
#include "BaseView.h"

class AppView : public BaseView{
    
public:
	
    AppView();
    ~AppView();
    
    void update();
    
    void setInstanceID(int instanceID);
    int getInstanceID();
    
protected:
	
    int instanceID;
    
private:
	
};

#endif
