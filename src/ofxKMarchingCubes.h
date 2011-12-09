//
//  ofxKMarchingCubes.h
//  emptyExample
//
//  Created by Kamen Dimitrov on 12/8/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef emptyExample_ofxKMarchingCubes_h
#define emptyExample_ofxKMarchingCubes_h

#include "ofMain.h"

#define kGridResolution 20
#define kGridSizeX 50
#define kGridSizeY 50
#define kGridSizeZ 50

class ofxKMarchingCubes {
public:
    vector<vector<vector<float > > > gridIsoValues_; 
    vector<vector<vector<ofVec3f> > > gridPoints_;
     
    float gridSizeX_;
    float gridSizeY_;
    float gridSizeZ_;
    float gridResolution_;
    
    
    ofxKMarchingCubes();
    void initGrid();
    void drawGridInPoints();
    void drawGridTriangles();
    void zeroGrid();
    
    // misc functions
    int getNumTriangles();
    
    // the heart of Marching Cubes
    void update(float threshold);
    void marchingCubes(int cubeX,int cubeY,int cubeZ, float threshold);
    void vertexInterpolation(float threshold, ofVec3f& p1, ofVec3f& p2, float valp1, float valp2, ofVec3f& vertex);
    
    //Iso Values
    void addMetaBall(ofVec3f center, float charge);

private:
    ofVec3f vertList[12];
    vector<ofVec3f> vertices;
    vector<ofColor> colors;
    vector<ofVec3f> normals;
    int numTriangles;
    
    
};


#endif
