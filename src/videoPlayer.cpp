//
//  videoPlayer.cpp
//  kinectExample
//
//  Created by Ovidiu on 31/07/14.
//
//

#include "videoPlayer.h"



videoPlayer::videoPlayer(){
    
    // ----
    _mapping = new ofxMtlMapping2D();
    _mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
    

    
    
    mov.setPixelFormat(OF_PIXELS_RGBA);
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
	
	mov.loadMovie("MAIN.mov", decodeMode);
    
    mov.setLoopState(OF_LOOP_NONE);
    
    MovieState = MOVIE_FINISHED;
    
    mov.play();
    mov.setSynchronousSeeking(false);
    mov.setVolume(0.5f);
    mainOutputSyphonServer.setName("Screen Output");
    
    soundObj.loadSound("rock10.wav");
    soundObj.setVolume(0.5f);
    soundObj.setMultiPlay(false);
    
    ofAddListener(BoxEvent::onBoxEvent,this, &videoPlayer::onBoxEvent);
    
    
}

void videoPlayer::update(void){
    _mapping->update();

    switch(MovieState){
        case MOVIES_PLAY:
            speed = 0.7;
            mov.setPaused(false);
            if ((mov.getPositionInSeconds() > (mov.getDuration() - 0.2))&&
                (mov.getPositionInSeconds() < (mov.getDuration() - 0.1)))
                MovieState = MOVIE_ROLL_BACK;
 

            break;
            
        case MOVIE_FINISHED:
           mov.setFrame(1);
            break;
        case MOVIE_BACKWARD:
        case MOVIE_ROLL_BACK:
            speed = -5;
            
            mov.setPaused(false);
            if (mov.getPositionInSeconds() < 0.9){
                MovieState = MOVIE_FINISHED;
               
                mov.setFrame(1);
              
                mov.setPaused(true);
            }
            break;
            
    }

    mov.setSpeed(speed);

    if (mov.isLoaded())
    mov.update();
        
  
    
}


void videoPlayer::draw(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ----
    //_mapping->bind();
    
    if (mov.isLoaded())
    mov.draw(0,0);
    /*ofSetColor(50);
    ofDrawBitmapString("Speed"+ofToString(mov.getSpeed()), 20,20);*/

     //_mapping->unbind();
    
    //_mapping->draw();
    
    mainOutputSyphonServer.publishScreen();
}

void videoPlayer::onBoxEvent(BoxEvent & args){
    
    if (args.Debug){

        mov.setPosition(0);
        MovieState = MOVIES_PLAY;
    }else{
        if (args.Touch){
            switch(MovieState){
                case MOVIES_PLAY:
                    break;
                case MOVIE_FINISHED:
                    MovieState = MOVIES_PLAY;
                    soundObj.play();
                    break;
                case MOVIE_BACKWARD:
                    MovieState = MOVIES_PLAY;
                    soundObj.play();
                    break;
                case MOVIE_ROLL_BACK:
                    break;
            }
            //cout<<"Touch"<<endl;
        
        }
        else
        {
            switch(MovieState){
                case MOVIES_PLAY:
                    MovieState = MOVIE_BACKWARD;
                    break;
                case MOVIE_FINISHED:
                    break;
                case MOVIE_BACKWARD:
                    break;
                case MOVIE_ROLL_BACK:
                    break;
            }
            
            //cout<<"NotTouch"<<endl;
        }
    }
    
}