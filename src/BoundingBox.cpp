//
//  BoundingBox.cpp
//  kinectExample
//
//  Created by Ovidiu on 30/07/14.
//
//

#include "BoundingBox.h"
#include "BoxEvent.h"


#define MEASURE_INTERVAL_MILLIS 200

BoundingBox::BoundingBox(string BoxName){
    pointCloudNearThreshold = 534;
    pointCloudFarThreshold = 1000;
    pointCloudFloorThreshold = -100;
    pointCloudRoofThreshold = 500;
    pointCloudRightThreshold = 100;
    pointCloudLeftThreshold = 0;
    MeasurePointX = 0;
    MeasurePointY = 0;
    MeasurePointZ = 0;
    CalibrationPoints = 0;
    Touch = false;
    gui = NULL;
    Calibrate = false;
    CreateGUI();
    gui->setVisible(false);
    this->BoxName = BoxName;
    bSaveToXml = false;
    bLoadFromXml = false;
    TouchEventActive = true;
    
    loadFromXml();
    
    MeasureTimeStamp = ofGetElapsedTimeMillis();
    
}
void BoundingBox::CreateGUI(void){
    ofVec2f posGui = ofVec2f(ofGetWidth()-OFX_UI_GLOBAL_CANVAS_WIDTH, 0);
    gui = new ofxUICanvas(posGui.x, posGui.y, OFX_UI_GLOBAL_CANVAS_WIDTH, OFX_UI_GLOBAL_CANVAS_WIDTH);
    gui->addLabel("BoundingBoxSetting");
    gui->addSpacer();
    
    gui->addIntSlider("pointCloudNearThreshold", 0, 2000, &pointCloudNearThreshold);
    gui->addIntSlider("pointCloudFarThreshold", 0, 2000, &pointCloudFarThreshold);
    gui->addSpacer();
    gui->addIntSlider("pointCloudFloorThreshold", 0, 2000, &pointCloudFloorThreshold);
    gui->addIntSlider("pointCloudRoofThreshold", 0, 2000, &pointCloudRoofThreshold);
    gui->addSpacer();
    gui->addIntSlider("pointCloudLeftThreshold", -1000, 2000, &pointCloudLeftThreshold);
    gui->addIntSlider("pointCloudRightThreshold", -1000, 2000, &pointCloudRightThreshold);
    gui->addSpacer();
    gui->addButton("Calibrate", &Calibrate);
    gui->addSpacer();
    gui->addButton("SaveToXML", &bSaveToXml);
    gui->addButton("LoadFromXML", &bLoadFromXml);
    gui->addSpacer();
    gui->addToggle("TouchEventActive", &TouchEventActive);
    gui->autoSizeToFitWidgets();

}

void BoundingBox::update(ofxKinect &kinect){
    int w = 640;
	int h = 480;
    
    if (Calibrate)
        CalibrationPoints = CountPoints;
    if (bSaveToXml)
        saveToXml();
    if (bLoadFromXml)
        loadFromXml();
    
    CountPoints = 0;
    
    
    int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if((kinect.getDistanceAt(x, y) > pointCloudNearThreshold) &&
               (kinect.getDistanceAt(x, y) < pointCloudFarThreshold) &&
               (x > pointCloudLeftThreshold) &&
               (x < pointCloudRightThreshold) &&
               (y > pointCloudFloorThreshold) &&
               (y < pointCloudRoofThreshold)
               )
                
            {

                CountPoints++;
                
			}
        }
    }
    if ((CountPoints - CalibrationPoints) > 200){
        if (!Touch)
            MeasureTimeStamp = ofGetElapsedTimeMillis();
        Touch = true;
        
        
    }
    else{
        if (Touch)
            MeasureTimeStamp = ofGetElapsedTimeMillis();
        Touch = false;
    }
    
    if ((ofGetElapsedTimeMillis() - MeasureTimeStamp) > MEASURE_INTERVAL_MILLIS){
        MeasureTimeStamp = ofGetElapsedTimeMillis();
    
    
        if ((TouchEventActive)
            ){
            BoxEvent BoxEvt;
            BoxEvt.BoxName = BoxName;
            BoxEvt.Touch = Touch;
            BoxEvt.Debug = false;
            ofNotifyEvent(BoxEvent::onBoxEvent, BoxEvt);
        }
    }
    
    
    gui->setPosition(ofGetWidth()-OFX_UI_GLOBAL_CANVAS_WIDTH, 0);

}

void BoundingBox::drawBoundingBox(ofxKinect &kinect){
    int w = 640;
	int h = 480;

    gui->setVisible(true);


    
    ofPlanePrimitive pointCloudNearThresholdPlain;
    pointCloudNearThresholdPlain.set(1500, 1500);
    pointCloudNearThresholdPlain.setPosition( kinect.getWorldCoordinateAt(0, 0, pointCloudNearThreshold));
    
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudNearThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();
    
    
    ofPlanePrimitive pointCloudFarThresholdPlain;
    pointCloudFarThresholdPlain.set(1500, 1500);
    pointCloudFarThresholdPlain.setPosition(kinect.getWorldCoordinateAt(0, 0, pointCloudFarThreshold));
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudFarThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();
    
    ofPlanePrimitive pointCloudFloorThresholdPlain;
    pointCloudFloorThresholdPlain.set(1500, 1500);
    pointCloudFloorThresholdPlain.setOrientation(ofVec3f(0,90,90));
    pointCloudFloorThresholdPlain.setPosition(kinect.getWorldCoordinateAt(0,pointCloudFloorThreshold, 1000));
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudFloorThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();
    
    ofPlanePrimitive pointCloudRoofThresholdPlain;
    pointCloudRoofThresholdPlain.set(1500, 1500);
    pointCloudRoofThresholdPlain.setOrientation(ofVec3f(0,90,90));
    pointCloudRoofThresholdPlain.setPosition(kinect.getWorldCoordinateAt(0, pointCloudRoofThreshold, 1000 ));
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudRoofThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();
    
    ofPlanePrimitive pointCloudLeftThresholdPlain;
    pointCloudLeftThresholdPlain.set(1500, 1500);
    pointCloudLeftThresholdPlain.setOrientation(ofVec3f(90,90,0));
    pointCloudLeftThresholdPlain.setPosition(kinect.getWorldCoordinateAt( pointCloudLeftThreshold, 0, 1000 ));
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudLeftThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();
    
    ofPlanePrimitive pointCloudRightThresholdPlain;
    pointCloudRightThresholdPlain.set(1500, 1500);
    pointCloudRightThresholdPlain.setOrientation(ofVec3f(90,90,0));
    pointCloudRightThresholdPlain.setPosition(kinect.getWorldCoordinateAt( pointCloudRightThreshold, 0, 1000 ));
    ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
    pointCloudRightThresholdPlain.drawWireframe();
	ofDisableDepthTest();
	ofPopMatrix();

	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if((kinect.getDistanceAt(x, y) > pointCloudNearThreshold) &&
               (kinect.getDistanceAt(x, y) < pointCloudFarThreshold) &&
               (x > pointCloudLeftThreshold) &&
               (x < pointCloudRightThreshold) &&
               (y > pointCloudFloorThreshold) &&
               (y < pointCloudRoofThreshold)
               )
            
            {
				mesh.addColor(kinect.getColorAt(x,y));
                //mesh.addColor(texture.getColor(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
                
			}
            if(kinect.getDistanceAt(x, y) == pointCloudNearThreshold) {
                MeasurePointX = kinect.getWorldCoordinateAt(x, y).x;
                MeasurePointY = kinect.getWorldCoordinateAt(x, y).y;
                MeasurePointZ = kinect.getWorldCoordinateAt(x, y).z;
                
			}
		}
	}

	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();

    

    
}

bool BoundingBox::isTouched(){
    return Touch;
}
void BoundingBox::saveToXml(void){
    
    ofxXmlSettings boxSettings;
    
    boxSettings.addTag("box");
    boxSettings.pushTag("box");
    
    boxSettings.addValue("pointCloudNearThreshold", pointCloudNearThreshold);
    boxSettings.addValue("pointCloudFarThreshold", pointCloudFarThreshold);
    boxSettings.addValue("pointCloudFloorThreshold", pointCloudFloorThreshold);
    boxSettings.addValue("pointCloudRoofThreshold", pointCloudRoofThreshold);
    boxSettings.addValue("pointCloudRightThreshold", pointCloudRightThreshold);
    boxSettings.addValue("pointCloudLeftThreshold", pointCloudLeftThreshold);
    boxSettings.addValue("CalibrationPoints", CalibrationPoints);
    
    boxSettings.popTag();
    boxSettings.saveFile(BoxName+".xml");
}

void BoundingBox::loadFromXml(void){
     ofxXmlSettings boxSettings;
    if (boxSettings.loadFile(BoxName+".xml")){
         boxSettings.pushTag("box");
        pointCloudNearThreshold = boxSettings.getValue("pointCloudNearThreshold",0);
        pointCloudFarThreshold = boxSettings.getValue("pointCloudFarThreshold",0);
        pointCloudFloorThreshold = boxSettings.getValue("pointCloudFloorThreshold",0);
        pointCloudRoofThreshold = boxSettings.getValue("pointCloudRoofThreshold",0);
        pointCloudRightThreshold = boxSettings.getValue("pointCloudRightThreshold",0);
        pointCloudLeftThreshold = boxSettings.getValue("pointCloudLeftThreshold",0);
        CalibrationPoints = boxSettings.getValue("CalibrationPoints",0);
        boxSettings.popTag();
        
        
    }
    else{
    
        cout<<"Error opening xml"<<endl;
    }
}

