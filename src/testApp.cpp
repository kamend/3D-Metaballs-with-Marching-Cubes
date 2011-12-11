#include "testApp.h"

const int kNumBalls = 10;
const float kThreshold = 0.7;
//--------------------------------------------------------------
void testApp::setup(){
	
	// setup
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    mcubes = new ofxKMarchingCubes();
    mcubes->initGrid();
    for(int i=0;i<kNumBalls;i++) {
        MetaBall b;
        b.init(ofVec3f(abs(ofRandomf()*1000),abs(ofRandomf()*1000) ,abs(ofRandomf()*1000)));
        b.size = 20;
        balls.push_back(b);
    }
    
    shader.load("shader");
    // marching cubes
    
    mcubes->zeroGrid();
    for(int i=0;i<kNumBalls;i++) {
        mcubes->addMetaBall(balls[i], balls[i].size);
    }    
    mcubes->update(kThreshold);
}

//--------------------------------------------------------------
void testApp::update(){
    // update metaballs
    
    for(int i=0;i<kNumBalls;i++) {
        ofVec3f force = ofVec3f(ofRandomf(),ofRandomf(),ofRandomf());
        balls[i].update(force*3);
    }
   
    mcubes->zeroGrid();
    for(int i=0;i<kNumBalls;i++) {
        mcubes->addMetaBall(balls[i], balls[i].size);
    }    
    

}

//--------------------------------------------------------------
void testApp::draw(){
    
    glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_BLEND);
    
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2);
    
    cam.begin();
   
    ofRotateX(-75);
    
 
    ofScale(-1,-1,1);
    
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    ofBackground(0);

    mcubes->update(kThreshold);
    ofNoFill();
    ofSetColor(100,100,100,100);
   // ofRect(0, 0, kGridSizeX*kGridResolution, kGridResolution*kGridSizeY);
    shader.begin();
    mcubes->drawGridTriangles();
    shader.end();
    cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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