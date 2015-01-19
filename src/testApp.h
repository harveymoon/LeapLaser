#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxEtherdream.h"
//#include "ofxBox2d.h"

class testApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
		
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    
	vector <int> fingersFound; 
	ofEasyCam cam;
	ofLight l1;
	ofLight l2;
	ofMaterial m1;
	
	map <int, ofPolyline> fingerTrails;
    
    ofxIlda::Frame ildaFrame;   // stores and manages ILDA frame drawings
    
    ofxEtherdream etherdream;   // interface to the etherdream device
    float rot;
    int MODENOW;
    
    
//    //physics
//    vector <ofPolyline>                 lines;
//    ofxBox2d                            box2d;
//    vector <ofPtr<ofxBox2dCircle> >		circles;
//   vector <ofPtr<ofxBox2dEdge> >       edges;
//    
//    int ballDropTimer;

    
};
