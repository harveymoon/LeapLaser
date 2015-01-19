#include "testApp.h"
#include "ofxIldaFrame.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project.
 where the first path ../../../addons/ofxLeapMotion/ is the path to the ofxLeapMotion addon.
 
 cp -f ../../../addons/ofxLeapMotion/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
 
 If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
 */

//--------------------------------------------------------------
void testApp::setup(){
    
    // ofSetFrameRate(60);
    ofSetVerticalSync(false);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    leap.open();
    
    //l1.setPosition(200, 300, 50);
    //l2.setPosition(-200, -200, 50);
    
    //	cam.setOrientation(ofPoint(-20, 0, 0));
    
    //	glEnable(GL_DEPTH_TEST);
    //   glEnable(GL_NORMALIZE);
    
    etherdream.setup();
    etherdream.setPPS(30000);
    
    MODENOW = 0;
    rot = 0;
    
    
    
    //Physics
    
//    // Box2d
//    box2d.init();
//    box2d.setGravity(0, 30);
//    box2d.createGround();
//    box2d.setFPS(30.0);
//    
//    
//    // load the lines we saved...
//    ifstream f;
//    f.open(ofToDataPath("lines.txt").c_str());
//    vector <string> strLines;
//    while (!f.eof()) {
//        string ptStr;
//        getline(f, ptStr);
//        strLines.push_back(ptStr);
//    }
//    f.close();
//    
//    for (int i=0; i<strLines.size(); i++) {
//        vector <string> pts = ofSplitString(strLines[i], ",");
//        if(pts.size() > 0) {
//            ofPtr <ofxBox2dEdge> edge = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
//            for (int j=0; j<pts.size(); j+=2) {
//                if(pts[j].size() > 0) {
//                    float x = ofToFloat(pts[j]);
//                    float y = ofToFloat(pts[j+1]);
//                    edge.get()->addVertex(x, y);
//                }				
//            }
//            edge.get()->create(box2d.getWorld());
//            edges.push_back(edge);
//        }
//    }
//    
    
    
}


//--------------------------------------------------------------
void testApp::update(){
    
    fingersFound.clear();
    
    //here is a simple example of getting the hands and using them to draw trails from the fingertips.
    //the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
    
    //if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method.
    //there you can work with the frame data directly.
    //
    
    
    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms.
    
    simpleHands = leap.getSimpleHands();
    //
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, 0,1);
        leap.setMappingY(90, 490, 0,1);
        leap.setMappingZ(-150, 150, 1,0);
        
        for(int i = 0; i < simpleHands.size(); i++){
            
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                
                ofPolyline & polyline = fingerTrails[id];
                ofPoint pt = simpleHands[i].fingers[j].pos;
                
                //if the distance between the last point and the current point is too big - lets clear the line
                //this stops us connecting to an old drawing
                if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
                    
                    polyline.clear();
                    
                }
                if( pt.z<.5){
                    polyline.clear();
                }
                
                
                //add our point to our trail
                polyline.addVertex(pt);
                
                
                vector<ofPoint> boo = polyline.getVertices();
                
                while(boo.size()>30){
                    
                    boo.erase(boo.begin());
                }
                //
                
                polyline.clear();
                
                
                polyline.addVertices(boo);
                
                
                //store fingers seen this frame for drawing
                fingersFound.push_back(id);
            }
        }
    }
    
    
    ofPushMatrix();
    ofRotate(180, 0, 0, 0);
    ofTranslate(ofGetWidth()/2, 0);
    
    
    
    //    for(int i = 0; i < simpleHands.size(); i++){
    //        // simpleHands[i].debugDraw();
    //        for(int j = 0; j < simpleHands[i].fingers.size(); j++){
    //            ofDrawBitmapString("Finger at " + ofToString(simpleHands[i].fingers[j].pos), 200, 20*j);
    //
    //
    //
    //            //        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
    //            //		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
    //            //        leap.setMappingZ(-150, 150, -200, 200);
    //
    //
    //
    //            float zSiz = ofMap(simpleHands[i].fingers[j].pos.z, -300, 300, .2, 0, true);
    //            float xLoc = ofMap(simpleHands[i].fingers[j].pos.x, -800, 800, 1, 0, true);
    //            float yLoc = ofMap(simpleHands[i].fingers[j].pos.y, -800, 800, 1, 0, true);
    //
    //
    //            if(simpleHands[i].fingers[j].pos.z>0){
    //
    //                ofCircle(xLoc*ofGetWidth(),yLoc*ofGetHeight(),zSiz*10);
    //
    //
    //
    //                 ofPolyline p = ofPolyline::fromRectangle(ofRectangle(xLoc ,yLoc ,zSiz,zSiz));
    //
    //                     ildaFrame.addPoly(p);
    //            }
    //        }
    //    }
    
    for(int i = 0; i < fingersFound.size(); i++){
        
        
        
        int id = fingersFound[i];
        
        
        ofPolyline & polyline = fingerTrails[id];
        
        
        
        //    ildaFrame.addPoly(polyline);
        
        
        // if(i == 1){
        
        
        //   ildaFrame.addPoly(polyline);
        
        
        //  }
        
        
        //ofCircle(polyline.getVertices().x,polyline.getVertices().y,200,200);
        
        //  strip.generate(polyline.getVertices(), 15, ofPoint(0, 0.5, 0.5) );
        
        //	ofSetColor(255 - id * 15, 0, id * 25);
        //strip.getMesh().draw();
    }
    
    ofPopMatrix();
    
    
    
    
    // Option 2: Work with the leap data / sdk directly - gives you access to more properties than the simple approach
    // uncomment code below and comment the code above to use this approach. You can also inhereit ofxLeapMotion and get the data directly via the onFrame callback.
    
    //	vector <Hand> hands = leap.getLeapHands();
    //	if( leap.isFrameNew() && hands.size() ){
    //
    //		//leap returns data in mm - lets set a mapping to our world space.
    //		//you can get back a mapped point by using ofxLeapMotion::getMappedofPoint with the Leap::Vector that tipPosition returns
    //		leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
    //		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
    //		leap.setMappingZ(-150, 150, -200, 200);
    //
    //		for(int i = 0; i < hands.size(); i++){
    //            for(int j = 0; j < hands[i].fingers().count(); j++){
    //				ofPoint pt;
    //
    //				const Finger & finger = hands[i].fingers()[j];
    //
    //				//here we convert the Leap point to an ofPoint - with mapping of coordinates
    //				//if you just want the raw point - use ofxLeapMotion::getofPoint
    //				pt = leap.getMappedofPoint( finger.tipPosition() );
    //
    //				//lets get the correct trail (ofPolyline) out of our map - using the finger id as the key
    //				ofPolyline & polyline = fingerTrails[finger.id()];
    //
    //				//if the distance between the last point and the current point is too big - lets clear the line
    //				//this stops us connecting to an old drawing
    //				if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
    //					polyline.clear();
    //				}
    //
    //				//add our point to our trail
    //				polyline.addVertex(pt);
    //
    //				//store fingers seen this frame for drawing
    //				fingersFound.push_back(finger.id());
    //			}
    //		}
    //	}
    //
    
    //IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
    leap.markFrameAsOld();
    
    
//    
//    //Physics
//    ballDropTimer++;
//    
//    if(ballDropTimer == 150){
//        
//        ballDropTimer = 0;
//        ofPtr<ofxBox2dCircle> c = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
//        c.get()->setPhysics(0.2, 0.2, 0.002);
//        c.get()->setup(box2d.getWorld(), 40, -20, 5);
//        // c.get()->setVelocity(0, 15); // shoot them down!
//        circles.push_back(c);
//    }
//    
//    box2d.update();	

    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDisableLighting();
    //  ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    ofBackground(0);
    
    ofSetColor(200);
    ofDrawBitmapString("Leap Connected? " + ofToString(leap.isConnected()), 200, 20);
    
    ofPushMatrix();
    ofRotate(90, mouseX, 0, 1);
    //ofSetColor(20);
    //ofDrawGridPlane(800, 20, false);
    ofPopMatrix();
    
    //ofEnableLighting();
    //l1.enable();
    //	l2.enable();
    
    //m1.begin();
    //m1.setShininess(0.6);
    
    ofDrawBitmapString("ModeNow  " + ofToString(MODENOW), 400, 20);
    
    if(MODENOW>=3){
        MODENOW = 0;
    }
    
    if(MODENOW == 0){
        
        
        ildaFrame.clear();
        for(int i = 0; i < fingersFound.size(); i++){
            
            int id = fingersFound[i];
            
            ofPolyline & polyline = fingerTrails[id];
            
            ildaFrame.addPoly(polyline);
            
            ofSetColor(255 - id * 15, 0, id * 25);
            
        }
    }
    
    if(MODENOW == 1 ){
        
        rot = rot + 0.1;
        ildaFrame.clear();
        
        
        for(int i = 0; i < simpleHands.size(); i++){
            
            for(int j = 0; j < simpleHands[i].fingers.size(); j++){
                int id = simpleHands[i].fingers[j].id;
                
                ofPoint pt = simpleHands[i].fingers[j].pos;
                float xLoc = pt.x;
                float yLoc = pt.y;
                float zSiz = pt.z*.08;
                
                ofPolyline p;
                
                for (float rr = 0; rr <= 6.28; rr+=6.28/3) {
                    float x = cos(rr+rot)*zSiz;
                    float y = sin(rr+rot)*zSiz;
                    p.addVertex(x+xLoc,y+yLoc);
                    //p = ofPolyline::fromRectangle(ofRectangle(xLoc ,yLoc ,.1,.1));
                    
                }
                
                if(zSiz>.01)  ildaFrame.addPoly(p);
                
            }
        }
        
        if(MODENOW == 2 ){
            
            
        }
        
        
        
        
        //        for(int i = 0; i < fingersFound.size(); i++){
        //
        //            int id = fingersFound[i];
        //
        //            ofPolyline & polyline = fingerTrails[id];
        //            ofVec3f nowFinger = polyline.getVertices()[0];
        //
        //            ofDrawBitmapString("fingerNow  " + ofToString(nowFinger), 400, 400+(20*i));
        //
        //            ofDrawBitmapString("rot  " + ofToString(rot), 400, 390+(20*i));
        //
        //
        //            float xLoc = nowFinger.x;
        //            float yLoc = nowFinger.y;
        //
        //            float zSiz = nowFinger.z*.08;
        //
        //            ofPolyline p;
        //
        //            for (float rr = 0; rr <= 6.28; rr+=6.28/3) {
        //                float x = cos(rr+rot+(id/10))*zSiz;
        //                float y = sin(rr+rot+(id/10))*zSiz;
        //
        //                p.addVertex(x+xLoc,y+yLoc);
        //
        //                //p = ofPolyline::fromRectangle(ofRectangle(xLoc ,yLoc ,.1,.1));
        //
        //                        }
        //
        //          if(zSiz>.01)  ildaFrame.addPoly(p);
        //
        //        }
    }
    
    
    // do your thang
    ildaFrame.update();
    
    // draw to the screen
    ildaFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // send points to the etherdream
    etherdream.setPoints(ildaFrame);
    
    
    ofSetColor(255);
    ofDrawBitmapString(ildaFrame.getString(), 10, 30);
    
    
    
    //physics
    
//    // some circles :)
//    for (int i=0; i<circles.size(); i++) {
//        ofFill();
//        ofSetHexColor(0xc0dd3b);
//        circles[i].get()->draw();
//    }
//    
//    ofSetHexColor(0x444342);
//    ofNoFill();
//    for (int i=0; i<lines.size(); i++) {
//        lines[i].draw();
//    }
//    for (int i=0; i<edges.size(); i++) {
//        edges[i].get()->draw();
//    }
//    
//    string info = "Draw a shape with the mouse\n";
//    info += "Press 1 to add some circles\n";
//    info += "Press c to clear everything\n";
//    
//    ofSetHexColor(0x444342);
//    ofDrawBitmapString(info, 10, 15);
//
//    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch(key) {
        case 'f': ofToggleFullscreen(); break;
            
            
            // clear the frame
        case 'c':
            ildaFrame.clear();
//            lines.clear();
//            edges.clear();

            
            break;
            
            // draw rectangle
        case 'r': {
            ofPolyline p = ofPolyline::fromRectangle(ofRectangle(ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2));
            ildaFrame.addPoly(p);
        }
            break;
            
            // change color
        case 'R': ildaFrame.params.output.color.set(1,0,0); break;
        case 'G': ildaFrame.params.output.color.set(0,1,0); break;
        case 'B': ildaFrame.params.output.color.set(0,0,1); break;
            
            // toggle draw lines (on screen only)
        case 'l': ildaFrame.params.draw.lines ^= true; break;
            
            // toggle loop for last poly
        case 'o': ildaFrame.getLastPoly().setClosed(ildaFrame.getLastPoly().isClosed()); break;
            
            // toggle draw points (on screen only)
        case 'p': ildaFrame.params.draw.points ^= true; break;
            
            // adjust point count
        case '.': ildaFrame.polyProcessor.params.targetPointCount++; break;
        case ',': if(ildaFrame.polyProcessor.params.targetPointCount > 10) ildaFrame.polyProcessor.params.targetPointCount--; break;
            
            // adjust point count quicker
        case '>': ildaFrame.polyProcessor.params.targetPointCount += 10; break;
        case '<': if(ildaFrame.polyProcessor.params.targetPointCount > 20) ildaFrame.polyProcessor.params.targetPointCount -= 10; break;
            
            // flip image
        case 'x': ildaFrame.params.output.transform.doFlipX ^= true; break;
        case 'y': ildaFrame.params.output.transform.doFlipY ^= true; break;
            
            // cap image
        case 'X': ildaFrame.params.output.doCapX ^= true; break;
        case 'Y': ildaFrame.params.output.doCapY ^= true; break;
            
            // move output around
        case OF_KEY_UP: ildaFrame.params.output.transform.offset.y -= 0.05; break;
        case OF_KEY_DOWN: ildaFrame.params.output.transform.offset.y += 0.05; break;
        case OF_KEY_LEFT: ildaFrame.params.output.transform.offset.x -= 0.05; break;
        case OF_KEY_RIGHT: ildaFrame.params.output.transform.offset.x += 0.05; break;
            
            // scale output
        case 'w': ildaFrame.params.output.transform.scale.y += 0.05; break;
        case 's': ildaFrame.params.output.transform.scale.y -= 0.05; break;
        case 'a': ildaFrame.params.output.transform.scale.x -= 0.05; break;
        case 'd': ildaFrame.params.output.transform.scale.x += 0.05; break;
            
        case 'C': ildaFrame.drawCalibration(); break;
            
        case 'M': MODENOW++; break;
            
            
//            
//             // want to save out some line...
//             if(key == ' ') {
//             ofstream f;
//             f.clear();
//             f.open(ofToDataPath("lines.txt").c_str());
//             for (int i=0; i<lines.size(); i++) {
//             for (int j=0; j<lines[i].size(); j++) {
//             float x = lines[i][j].x;
//             float y = lines[i][j].y;
//             f << x << "," << y << ",";
//             }
//             f << "\n";
//             }
//             f.close();lines.clear();
//             }
//            
            
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ildaFrame.getLastPoly().lineTo(x / (float)ofGetWidth(), y / (float)ofGetHeight());
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // create a new poly in the ILDA frame
    ildaFrame.addPoly();
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void testApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
