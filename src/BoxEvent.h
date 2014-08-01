//
//  BoxEvent.h
//  kinectExample
//
//  Created by Ovidiu on 31/07/14.
//
//

#pragma once
#include "ofMain.h"
class BoxEvent : public ofEventArgs{
public:
    
    string BoxName;
    bool Touch;
    bool Debug;
    
    static ofEvent <BoxEvent> onBoxEvent;
    
};