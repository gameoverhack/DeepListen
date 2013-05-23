//
//  AnalyzeView.h
//  DeepListen
//
//  Created by game over on 2/04/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//

#ifndef _H_ANALYZEVIEW
#define _H_ANALYZEVIEW

#include "AppModel.h"
#include "BaseView.h"

class AnalyzeView : public BaseView {
    
public:
	
    AnalyzeView();
    ~AnalyzeView();
    
    void update();

    void mousePressed(int x, int y, int button);
    
protected:
	
    ofFbo waveformFBO;
    string lastclip;
    
private:
	
};

#endif
