//
//  AppStates.h
//  DeepListen
//
//  Created by game over on 30/03/13.
//  Copyright (c) 2013 trace media. All rights reserved.
//


#ifndef _H_APPSTATES
#define _H_APPSTATES

enum {
    kAPPCONTROLLER_INIT = 0,
    kAPPCONTROLLER_LOAD,
//    kAPPCONTROLLER_SOUND,
    kAPPCONTROLLER_ANALYZE,
    kAPPCONTROLLER_PLAY
};

enum {
    kDEBUGVIEW_SHOWINFO = 0,
    kDEBUGVIEW_SHOWSTATES,
    kDEBUGVIEW_SHOWPROPS
};

enum {
    kANALYZEVIEW_SHOW = 0
};

enum {
    kPLAYCONTROLLER_INIT = 0,
    kPLAYCONTROLLER_MAKE,
    kPLAYCONTROLLER_PLAY
};

enum {
    kAPPVIEW_SHOWWARP_0 = 0,
    kAPPVIEW_SHOWWARP_1
};

#endif