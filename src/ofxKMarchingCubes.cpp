//
//  ofxKMarchingCubes.cpp
//  emptyExample
//
//  Created by Kamen Dimitrov on 12/8/11.
//  Based on: http://paulbourke.net/geometry/polygonise/
//



#include "ofxKMarchingCubes.h"
#include "ofxKMarchingCubesTables.h"

ofxKMarchingCubes::ofxKMarchingCubes() {
    gridSizeX_ = kGridSizeX;
    gridSizeY_ = kGridSizeY;
    gridSizeZ_ = kGridSizeZ;
    gridResolution_ = kGridResolution;
}
//----------------------------------
void ofxKMarchingCubes::initGrid() {
    for(int x = 0; x < gridSizeX_; x++) {
        vector<vector<float> > yv;
        vector<vector<ofVec3f> > yp;
        for(int y = 0; y < gridSizeY_; y++) {
            vector<float> zv;
            vector<ofVec3f> zp;
            for(int z = 0; z < gridSizeZ_; z++) {
                zv.push_back(0); 
                zp.push_back(ofVec3f(x*gridResolution_,y*gridResolution_,z*gridResolution_));
            }
            yv.push_back(zv);
            yp.push_back(zp);
        }
        gridIsoValues_.push_back(yv);
        gridPoints_.push_back(yp);
    }
}
//----------------------------------
void ofxKMarchingCubes::zeroGrid() {
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                gridIsoValues_[x][y][z] = 0;
                gridPoints_[x][y][z] = ofVec3f(x*gridResolution_,y*gridResolution_,z*gridResolution_);
            }
        }
    }    
}
//----------------------------------
void ofxKMarchingCubes::drawGridInPoints() {
    ofMesh m;
    m.setMode(OF_PRIMITIVE_POINTS);
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                if(gridIsoValues_[x][y][z] > 3) {
                m.addVertex(gridPoints_[x][y][z]);
                float c= 255 - gridIsoValues_[x][y][z]*255;
                m.addColor(ofColor(c,c,c,c));
                }
            }
        }
    }
    m.drawWireframe();
}

void ofxKMarchingCubes::drawGridTriangles() {
    ofMesh m;
    m.setMode(OF_PRIMITIVE_TRIANGLES);
    for(int i=0; i < numTriangles; i++) {
        m.addVertex(vertices[i*3]);
    
        m.addVertex(vertices[i*3+1]);
        m.addVertex(vertices[i*3+2]);
    }
    
    m.drawFaces();
}
//----------------------------------
void ofxKMarchingCubes::addMetaBall(ofVec3f center, float charge) {
    ofVec3f diff;
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                diff = gridPoints_[x][y][z] - center;
                gridIsoValues_[x][y][z] += charge / sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
            }
        }
    }    
}
//----------------------------------
void ofxKMarchingCubes::update(float threshold) {
    numTriangles = 0;
    vertices.clear();
    for(int x=0; x < gridSizeX_-1; x++) {
        for(int y=0; y < gridSizeY_-1; y++) {
            for(int z = 0; z < gridSizeZ_-1; z++) {
                marchingCubes(x, y, z, threshold);
            }
        }
    }     
}
//----------------------------------
int ofxKMarchingCubes::getNumTriangles() {
    return numTriangles;
}
//----------------------------------
void ofxKMarchingCubes::marchingCubes(int cubeX,int cubeY,int cubeZ,float threshold) {
    int cubeindex = 0;
    
    
    // here we find all edges that intersect with our Iso surface 
    
    if (gridIsoValues_[cubeX][cubeY][cubeZ] < threshold) cubeindex |= 1;
	if (gridIsoValues_[cubeX+1][cubeY][cubeZ] < threshold) cubeindex |= 2;
	if (gridIsoValues_[cubeX+1][cubeY+1][cubeZ] < threshold) cubeindex |= 4;
	if (gridIsoValues_[cubeX][cubeY+1][cubeZ] < threshold) cubeindex |= 8;
	if (gridIsoValues_[cubeX][cubeY][cubeZ+1] < threshold) cubeindex |= 16;
	if (gridIsoValues_[cubeX+1][cubeY][cubeZ+1] < threshold) cubeindex |= 32;
	if (gridIsoValues_[cubeX+1][cubeY+1][cubeZ+1] < threshold) cubeindex |= 64;
	if (gridIsoValues_[cubeX][cubeY+1][cubeZ+1] < threshold) cubeindex |= 128;
	if (edgeTable[cubeindex] == 0){
        return;
	}
    
    
    // for shorter 
    int i = cubeX;
    int j = cubeY;
    int k = cubeZ;
       
    // here we calculate our intersecting verticies
    if(edgeTable[cubeindex] & 1) {
        vertexInterpolation(threshold, gridPoints_[i][j][k], gridPoints_[i+1][j][k], gridIsoValues_[i][j][k] ,gridIsoValues_[i+1][j][k], vertList[0]);
    }
    
	if (edgeTable[cubeindex] & 2){
		vertexInterpolation(threshold, gridPoints_[i+1][j][k], gridPoints_[i+1][j+1][k], gridIsoValues_[i+1][j][k], gridIsoValues_[i+1][j+1][k], vertList[1]);
	}
	if (edgeTable[cubeindex] & 4){
        vertexInterpolation(threshold, gridPoints_[i+1][j+1][k], gridPoints_[i][j+1][k], gridIsoValues_[i+1][j+1][k], gridIsoValues_[i][j+1][k], vertList[2]);
	}
	if (edgeTable[cubeindex] & 8){
        vertexInterpolation(threshold, gridPoints_[i][j+1][k], gridPoints_[i][j][k], gridIsoValues_[i][j+1][k], gridIsoValues_[i][j][k], vertList[3]);
	}
	if (edgeTable[cubeindex] & 16){
        vertexInterpolation(threshold, gridPoints_[i][j][k+1], gridPoints_[i+1][j][k+1], gridIsoValues_[i][j][k+1], gridIsoValues_[i+1][j][k+1], vertList[4]);
	}
	if (edgeTable[cubeindex] & 32){
        vertexInterpolation(threshold, gridPoints_[i+1][j][k+1], gridPoints_[i+1][j+1][k+1], gridIsoValues_[i+1][j][k+1], gridIsoValues_[i+1][j+1][k+1], vertList[5]);
	}
	if (edgeTable[cubeindex] & 64){
		vertexInterpolation(threshold, gridPoints_[i+1][j+1][k+1], gridPoints_[i][j+1][k+1], gridIsoValues_[i+1][j+1][k+1], gridIsoValues_[i][j+1][k+1], vertList[6]);
	}
	if (edgeTable[cubeindex] & 128){
		vertexInterpolation(threshold, gridPoints_[i][j+1][k+1], gridPoints_[i][j][k+1], gridIsoValues_[i][j+1][k+1], gridIsoValues_[i][j][k+1], vertList[7]); 
	}
	if (edgeTable[cubeindex] & 256){
		vertexInterpolation(threshold, gridPoints_[i][j][k], gridPoints_[i][j][k+1], gridIsoValues_[i][j][k], gridIsoValues_[i][j][k+1], vertList[8]);
	}
	if (edgeTable[cubeindex] & 512){
		vertexInterpolation(threshold, gridPoints_[i+1][j][k], gridPoints_[i+1][j][k+1], gridIsoValues_[i+1][j][k], gridIsoValues_[i+1][j][k+1], vertList[9]); 
	}
	if (edgeTable[cubeindex] & 1024){
		vertexInterpolation(threshold, gridPoints_[i+1][j+1][k], gridPoints_[i+1][j+1][k+1], gridIsoValues_[i+1][j+1][k], gridIsoValues_[i+1][j+1][k+1], vertList[10]); 
	}
	if (edgeTable[cubeindex] & 2048){
		vertexInterpolation(threshold,	gridPoints_[i][j+1][k], gridPoints_[i][j+1][k+1], gridIsoValues_[i][j+1][k], gridIsoValues_[i][j+1][k+1], vertList[11]); 
	}    
    
    // here we build our triangles
    for(int i=0;triTable[cubeindex][i] != -1;i+=3) {
        
        ofVec3f p1 = vertList[triTable[cubeindex][i]];
        ofVec3f p2 = vertList[triTable[cubeindex][i+1]];
        ofVec3f p3 = vertList[triTable[cubeindex][i+2]];
        
        vertices.push_back(p1);
        vertices.push_back(p2);
        vertices.push_back(p3);
        
        numTriangles++;
    }
    
}

void ofxKMarchingCubes::vertexInterpolation(float threshold, 
                                            ofVec3f& p1, ofVec3f& p2, 
                                            float valp1, float valp2, 
                                            ofVec3f& vertex) 
{
    float mu;
    if (ABS(threshold-valp1) < 0.00001){
		vertex.set(p1.x, p1.y, p1.z);
		return;
	}
	if (ABS(threshold-valp2) < 0.00001){
		vertex.set(p2.x, p2.y, p2.z);
		return;
	}
	if (ABS(valp1-valp2) < 0.00001){
		vertex.set(p1.x, p1.x, p1.z);
		return;
	}
	mu = (threshold - valp1) / (valp2 - valp1);
    
	vertex.x = p1.x + mu * (p2.x - p1.x);
	vertex.y = p1.y + mu * (p2.y - p1.y);
	vertex.z = p1.z + mu * (p2.z - p1.z);    
}






