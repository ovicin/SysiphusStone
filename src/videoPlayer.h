//
//  videoPlayer.h
//  kinectExample
//
//  Created by Ovidiu on 31/07/14.
//
//

#pragma once

#include "ofQTKitPlayer.h"
#include "ofxMtlMapping2D.h"
#include "BoxEvent.h"

#include "ofxSyphon.h"


class videoPlayer{
public:
    videoPlayer();
    void update(void);
    void draw(void);
     void onBoxEvent(BoxEvent & args);
    
    ofQTKitPlayer mov;
    ofxMtlMapping2D* _mapping;
    
    float speed;
    
    enum enMovieState{
        MOVIES_PLAY,
        MOVIE_FINISHED,
        MOVIE_BACKWARD,
        MOVIE_ROLL_BACK
    };
    
    enMovieState MovieState;
    ofxSyphonServer mainOutputSyphonServer;
    
    ofSoundPlayer soundObj;

};