//
//  BoundingBox.h
//  kinectExample
//
//  Created by Ovidiu on 30/07/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"


class BoundingBox{
public:
    BoundingBox(string BoxName);
    void drawBoundingBox(ofxKinect &kinect);
    void update(ofxKinect &kinect);
    bool isTouched();
    void CreateGUI(void);
    void loadFromXml(void);
    void saveToXml(void);
    
    int pointCloudNearThreshold, pointCloudFarThreshold, pointCloudFloorThreshold, pointCloudRoofThreshold,MeasurePointX, MeasurePointY, MeasurePointZ,pointCloudLeftThreshold, pointCloudRightThreshold;
	double CountPoints, CalibrationPoints;
    
    bool Touch;
    bool Calibrate;
    bool bSaveToXml;
    bool bLoadFromXml;
    
    double MeasureTimeStamp;
   

    bool TouchEventActive;
    
    ofxUICanvas *gui;
    
    string BoxName;
};
