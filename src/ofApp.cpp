#include "ofApp.h"

ofApp::ofApp() :
    box1("box1")/*,
    box2("box2")*/
{
    
}

//--------------------------------------------------------------
void ofApp::setup()
{
    bvideo = true;
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    
    //texture.loadImage("stoneTex.jpg");
    

    //fulscreen = false;
    currentBox = 1;
    
    //ofVec2f posGui = ofVec2f(ofGetWidth()-OFX_UI_GLOBAL_CANVAS_WIDTH, ofGetHeight()-200);
    //gui = new ofxUICanvas(posGui.x, posGui.y, OFX_UI_GLOBAL_CANVAS_WIDTH, OFX_UI_GLOBAL_CANVAS_WIDTH);
    //gui->addLabel("SelectBox");
    //gui->addSpacer();
    /*vector <string> names;
    names.push_back("BOX1");
    names.push_back("BOX2");
    gui->addRadio("BoundingBox", names);*/
    //gui->addSpacer();
    //gui->addToggle("VIDEO/BOX CONFIG", &bvideo);
    //gui->addToggle("FULLSCREEN", &fulscreen);
    
    //ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    //	int kind = e.widget->getKind();
    if (name == "BoundingBox"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        
       currentBox = radio->getValue() + 1;
        
        /*if (currentBox == 1)
        box2.gui->setVisible(false);
        if (currentBox == 2)
        box1.gui->setVisible(false);*/
    }
    
}
//--------------------------------------------------------------
void ofApp::update() {
	
	ofBackground(100, 100, 100);
	
	kinect.update();
#if 0
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
	}
#endif
	
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
    
    box1.update(kinect);
    //box2.update(kinect);
    //ofSetFullscreen( fulscreen);
   
    //gui->setPosition(ofGetWidth()-OFX_UI_GLOBAL_CANVAS_WIDTH, ofGetHeight()-200);
   
    vid.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	
    
	ofSetColor(255, 255, 255);
	
	if(!bvideo) {
		easyCam.begin();
		drawPointCloud();
        
		easyCam.end();
        kinect.draw(420, 10, 250, 120);
        
        // draw instructions
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        
        if(kinect.hasAccelControl()) {
            reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
            << ofToString(kinect.getMksAccel().y, 2) << " / "
            << ofToString(kinect.getMksAccel().z, 2) << endl;
        } else {
            reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
            << "motor / led / accel controls are not currently supported" << endl << endl;
        }
        
        reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
        << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
        << "set near threshold " << nearThreshold << " (press: + -)" << endl
        << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
        << ", fps: " << ofGetFrameRate() << endl
        << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
        << "measure " << ofToString( box1.MeasurePointX)<< ","<< ofToString( box1.MeasurePointY)<< ","<< ofToString( box1.MeasurePointZ)<<endl
        << "point cloud threshold Near " << ofToString( box1.pointCloudNearThreshold)<< endl
        << "point cloud threshold Far " << ofToString( box1.pointCloudFarThreshold)<< endl
        << "CountPoints " << ofToString( box1.CountPoints)<< endl
        //<< "CountPoints2 " << ofToString( box2.CountPoints)<< endl
        
        << "Touch " << ofToString( box1.Touch)<< endl/*<< "Touch2 " << ofToString( box2.Touch)<< endl*/;
        if(kinect.hasCamTiltControl()) {
            reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
            << "press 1-5 & 0 to change the led mode" << endl;
        }
        
        ofDrawBitmapString(reportStream.str(), 20, 600);

	} else {
        box1.gui->setVisible(false);
        //box2.gui->setVisible(false);
        
        vid.draw();

	}
	
    
}

void ofApp::drawPointCloud() {
    /*if (currentBox ==1)*/
        box1.drawBoundingBox(kinect);
  /*  if (currentBox ==2)
        box2.drawBoundingBox(kinect);*/

}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {
		case ' ':{
			/*bThreshWithOpenCV = !bThreshWithOpenCV;*/
            BoxEvent BoxEvt;
            BoxEvt.Debug = true;
            ofNotifyEvent(BoxEvent::onBoxEvent, BoxEvt);
        }
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
            case 'm':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
            case 'n':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
            
        case 'b':
            
            bvideo = !bvideo;
            break;
        case 's':
            /*box1.pointCloudNearThreshold--;*/
            currentBox = 2;
            box1.gui->setVisible(false);
            break;
             /*
        case 'd':
            box1.pointCloudFarThreshold++;
            break;
        case 'f':
            box1.pointCloudFarThreshold--;
            break;
        case 'g':
            box1.CalibrationPoints = box1.CountPoints;
            break;
        case 'h':
            box1.pointCloudFloorThreshold++;
            break;
        case 'j':
            box1.pointCloudFloorThreshold--;
            break;
        case 'k':
            box1.pointCloudRoofThreshold++;
            break;
        case 'l':
            box1.pointCloudRoofThreshold--;
            break;
             */
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}
