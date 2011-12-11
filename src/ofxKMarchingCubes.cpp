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

ofVec3f ofxKMarchingCubes::getCubeVertex(int num, int i, int j, int k) {

}

float ofxKMarchingCubes::getCubeIsoValue(int num, int i, int j, int k) {
    
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
    normals.clear();
    vertices.clear();
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
    
        m.addNormal(normals[i*3]);
        m.addNormal(normals[i*3+1]);
        m.addNormal(normals[i*3+2]);
    }
    
   m.drawFaces();
   /*
    ofSetColor(255, 255, 255);
    ofMesh nm;
    nm.setMode(OF_PRIMITIVE_LINES);
    for(int i=0;i < numTriangles; i++) {
        nm.addVertex(vertices[i*3]);
        nm.addVertex(vertices[i*3]+normals[i*3]*40);
        nm.addVertex(vertices[i*3+1]);
        nm.addVertex(vertices[i*3+1]+normals[i*3+1]*40);
        nm.addVertex(vertices[i*3+2]);
        nm.addVertex(vertices[i*3+2]+normals[i*3+2]*40);
        

    }
    nm.drawWireframe();
  */   
}
//----------------------------------
void ofxKMarchingCubes::addMetaBall(ofVec3f center, float charge) {
    ofVec3f diff;
    for(int x=0; x < gridSizeX_; x++) {
        for(int y=0; y < gridSizeY_; y++) {
            for(int z = 0; z < gridSizeZ_; z++) {
                diff = gridPoints_[x][y][z] - center;
                gridIsoValues_[x][y][z] += charge / sqrt(pow(diff.x,2) + pow(diff.y,2) + pow(diff.z,2));
                
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

void ofxKMarchingCubes::getVec4(int index, int i, int j, int k,vec4 &v) {
    switch(index) {
        case 0:
            v.vector = gridPoints_[i][j][k];
            v.value = gridIsoValues_[i][j][k];
        break;
        case 1:
            v.vector = gridPoints_[i+1][j][k];
            v.value = gridIsoValues_[i+1][j][k];
            break;
        case 2:
            v.vector = gridPoints_[i+1][j][k+1];
            v.value = gridIsoValues_[i+1][j][k+1];
            break;
        case 3:
            v.vector = gridPoints_[i][j][k+1];
            v.value = gridIsoValues_[i][j][k+1];
            break;
        case 4:
            v.vector = gridPoints_[i][j+1][k];
            v.value = gridIsoValues_[i][j+1][k];
            break;
        case 5:
            v.vector = gridPoints_[i+1][j+1][k];
            v.value = gridIsoValues_[i+1][j+1][k];
            break;
        case 6:
            v.vector = gridPoints_[i+1][j+1][k+1];
            v.value = gridIsoValues_[i+1][j+1][k+1];
            break;
        case 7:
            v.vector = gridPoints_[i][j+1][k+1];
            v.value = gridIsoValues_[i][j+1][k+1];
            break;
        
    }
}

void ofxKMarchingCubes::getGradient(int index, int i, int j, int k,vec4 &v) {
   
    switch(index) {
        case 0:
            if(i != 0 && j != 0 && k != 0) { 
                v.vector = ofVec3f(
                    gridIsoValues_[i-1][j][k] - gridIsoValues_[i+1][j][k],
                    gridIsoValues_[i][j-1][k] - gridIsoValues_[i][j+1][k],
                    gridIsoValues_[i][j][k-1] - gridIsoValues_[i][j][k+1]);
                   v.value = gridIsoValues_[i][j][k];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
            break;
            
        case 1:
            if(i < gridSizeX_-2 && j != 0 && k != 0) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i+1-1][j][k] - gridIsoValues_[i+1+1][j][k],
                                   gridIsoValues_[i+1][j-1][k] - gridIsoValues_[i+1][j+1][k],
                                   gridIsoValues_[i+1][j][k-1] - gridIsoValues_[i+1][j][k+1]);
                v.value = gridIsoValues_[i+1][j][k];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
     
            break;
        case 2:
            if(i < gridSizeX_-2 && j != 0 && k < gridSizeZ_-2) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i+1-1][j][k+1] - gridIsoValues_[i+1+1][j][k+1],
                                   gridIsoValues_[i+1][j-1][k+1] - gridIsoValues_[i+1][j+1][k+1],
                                   gridIsoValues_[i+1][j][k+1-1] - gridIsoValues_[i+1][j][k+1+1]);
                 v.value = gridIsoValues_[i+1][j][k+1];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
           
            break;
        case 3:
            if(i != 0 && j != 0 && k < gridSizeZ_-2) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i-1][j][k+1] - gridIsoValues_[i+1][j][k+1],
                                   gridIsoValues_[i][j-1][k+1] - gridIsoValues_[i][j+1][k+1],
                                   gridIsoValues_[i][j][k+1-1] - gridIsoValues_[i][j][k+1+1]);
                  v.value = gridIsoValues_[i][j][k+1];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
          
            break;
        case 4:
            if(i != 0 && j < gridSizeY_-2 && k != 0) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i-1][j+1][k] - gridIsoValues_[i+1][j+1][k],
                                   gridIsoValues_[i][j+1-1][k] - gridIsoValues_[i][j+1+1][k],
                                   gridIsoValues_[i][j+1][k-1] - gridIsoValues_[i][j+1][k+1]);
                  v.value = gridIsoValues_[i][j+1][k];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
          
            break;
        case 5:
            if(i < gridSizeX_-2 && j <gridSizeY_-2 && k != 0) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i+1-1][j+1][k] - gridIsoValues_[i+1+1][j+1][k],
                                   gridIsoValues_[i+1][j+1-1][k] - gridIsoValues_[i+1][j+1+1][k],
                                   gridIsoValues_[i+1][j+1][k-1] - gridIsoValues_[i+1][j+1][k+1]);
               v.value = gridIsoValues_[i+1][j+1][k];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
           
            break;
        case 6:
            if(i <gridSizeX_-2 && j <gridSizeY_-2 && k <gridSizeZ_-2) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i+1-1][j+1][k+1] - gridIsoValues_[i+1+1][j+1][k+1],
                                   gridIsoValues_[i+1][j+1-1][k+1] - gridIsoValues_[i+1][j+1+1][k+1],
                                   gridIsoValues_[i+1][j+1][k+1-1] - gridIsoValues_[i+1][j+1][k+1+1]);
              v.value = gridIsoValues_[i+1][j+1][k+1];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
          
            break;
        case 7:
            if(i != 0 && j <gridSizeY_-2 && k < gridSizeZ_-2) { 
                v.vector = ofVec3f(
                                   gridIsoValues_[i-1][j+1][k+1] - gridIsoValues_[i+1][j+1][k+1],
                                   gridIsoValues_[i][j+1-1][k+1] - gridIsoValues_[i][j+1+1][k+1],
                                   gridIsoValues_[i][j+1][k+1-1] - gridIsoValues_[i][j+1][k+1+1]);
              v.value = gridIsoValues_[i][j+1][k+1];
            } else {
                v.vector = ofVec3f(0.0,0.0,0.0);
                v.value = 1.0;
            }
          
            break;
            
    }
}

//----------------------------------
void ofxKMarchingCubes::marchingCubes(int cubeX,int cubeY,int cubeZ,float threshold) {
    int cubeindex = 0;
    
    int i = cubeX;
    int j = cubeY;
    int k = cubeZ;
    

    vec4 vv[8];
    
    for(int ind=0;ind<8;ind++) {
        getVec4(ind, i, j, k,vv[ind]);
     
    }
    
    
    // check which vertices are inside the surface
    if(vv[0].value < threshold) cubeindex |= 1;
    if(vv[1].value < threshold) cubeindex |= 2;
    if(vv[2].value < threshold) cubeindex |= 4;
    if(vv[3].value < threshold) cubeindex |= 8;
    if(vv[4].value < threshold) cubeindex |= 16;
    if(vv[5].value < threshold) cubeindex |= 32;
    if(vv[6].value < threshold) cubeindex |= 64;
    if(vv[7].value < threshold) cubeindex |= 128;
    
   // ofLog() << cubeindex << endl;
    
    // cube is entirely out of the surface 
    if(edgeTable[cubeindex] == 0)
        return;
    
    /*
    // draw grid
    ofSetColor(100,100,100);
    ofLine(vv[0].vector,vv[1].vector);
    ofLine(vv[1].vector,vv[2].vector);
    ofLine(vv[2].vector,vv[3].vector);
    ofLine(vv[3].vector,vv[0].vector);
    ofLine(vv[4].vector,vv[5].vector);
    ofLine(vv[5].vector,vv[6].vector);
    ofLine(vv[6].vector,vv[7].vector);
    ofLine(vv[7].vector,vv[4].vector);
    ofLine(vv[5].vector,vv[1].vector);
    ofLine(vv[6].vector,vv[2].vector);
    ofLine(vv[7].vector,vv[3].vector);
    ofLine(vv[4].vector,vv[0].vector);
   
    */
    
    
    /* Find the vertices where the surface intersects the cube */
    
    // intersects with edge 0
    if(edgeTable[cubeindex] & 1) {
        vertexInterpolation(threshold, vv[0].vector, vv[1].vector, vv[0].value, vv[1].value, vertList[0]);
        
        getGradient(0, i, j, k, gradients[0]);
        getGradient(1, i, j, k, gradients[1]);
               
        vertexInterpolation(threshold, gradients[0].vector, gradients[1].vector
                            , gradients[0].value, gradients[1].value, gradList[0]);
    }
    
    // intersects with edge 1
    if(edgeTable[cubeindex] & 2) {
        vertexInterpolation(threshold, vv[1].vector, vv[2].vector, vv[1].value, vv[2].value, vertList[1]);
        
        getGradient(1, i, j, k, gradients[1]);
        getGradient(2, i, j, k, gradients[2]);
        
        vertexInterpolation(threshold, gradients[1].vector, gradients[2].vector
                            , gradients[1].value, gradients[2].value, gradList[1]);
    
     }

    // intersects with edge 2
    if(edgeTable[cubeindex] & 4) {
        vertexInterpolation(threshold, vv[2].vector, vv[3].vector, vv[2].value, vv[3].value, vertList[2]);
        
        getGradient(2, i, j, k, gradients[2]);
        getGradient(3, i, j, k, gradients[3]);
        
        vertexInterpolation(threshold, gradients[2].vector, gradients[3].vector
                            , gradients[2].value, gradients[3].value, gradList[2]);    
    }

    // intersects with edge 3
    if(edgeTable[cubeindex] & 8) {
        
        vertexInterpolation(threshold, vv[3].vector, vv[0].vector, vv[3].value, vv[0].value, vertList[3]);
        
        getGradient(3, i, j, k, gradients[3]);
        getGradient(0, i, j, k, gradients[0]);
        
        vertexInterpolation(threshold, gradients[3].vector, gradients[0].vector
                            , gradients[3].value, gradients[0].value, gradList[3]);
    }

    // intersects with edge 4
    if(edgeTable[cubeindex] & 16) {
        vertexInterpolation(threshold, vv[4].vector, vv[5].vector, vv[4].value, vv[5].value, vertList[4]);
        
        getGradient(4, i, j, k, gradients[4]);
        getGradient(5, i, j, k, gradients[5]);
        
        vertexInterpolation(threshold, gradients[4].vector, gradients[5].vector
                            , gradients[4].value, gradients[5].value, gradList[4]);
    }

    // intersects with edge 5
    if(edgeTable[cubeindex] & 32) {
        vertexInterpolation(threshold, vv[5].vector, vv[6].vector, vv[5].value, vv[6].value, vertList[5]);
        
        getGradient(5, i, j, k, gradients[5]);
        getGradient(6, i, j, k, gradients[6]);
        
        vertexInterpolation(threshold, gradients[5].vector, gradients[6].vector
                            , gradients[5].value, gradients[6].value, gradList[5]);
    }

    // intersects with edge 6
    if(edgeTable[cubeindex] & 64) {
        vertexInterpolation(threshold, vv[6].vector, vv[7].vector, vv[6].value, vv[7].value, vertList[6]);
        
        getGradient(6, i, j, k, gradients[6]);
        getGradient(7, i, j, k, gradients[7]);
        
        vertexInterpolation(threshold, gradients[6].vector, gradients[7].vector
                            , gradients[6].value, gradients[7].value, gradList[6]);
    }

    // intersects with edge 7
    if(edgeTable[cubeindex] & 128) {
        vertexInterpolation(threshold, vv[7].vector, vv[4].vector, vv[7].value, vv[4].value, vertList[7]);
        
        getGradient(7, i, j, k, gradients[7]);
        getGradient(4, i, j, k, gradients[4]);
        
        vertexInterpolation(threshold, gradients[7].vector, gradients[4].vector
                            , gradients[7].value, gradients[4].value, gradList[7]);
    }

    // intersects with edge 8
    if(edgeTable[cubeindex] & 256) {
        vertexInterpolation(threshold, vv[0].vector, vv[4].vector, vv[0].value, vv[4].value, vertList[8]);
        
        getGradient(0, i, j, k, gradients[0]);
        getGradient(4, i, j, k, gradients[4]);
        
        vertexInterpolation(threshold, gradients[0].vector, gradients[4].vector
                            , gradients[0].value, gradients[4].value, gradList[8]);
    }

    // intersects with edge 9
    if(edgeTable[cubeindex] & 512) {
        vertexInterpolation(threshold, vv[1].vector, vv[5].vector, vv[1].value, vv[5].value, vertList[9]);
        
        getGradient(1, i, j, k, gradients[1]);
        getGradient(5, i, j, k, gradients[5]);
        
        vertexInterpolation(threshold, gradients[1].vector, gradients[5].vector
                            , gradients[1].value, gradients[5].value, gradList[9]);
    }

    // intersects with edge 10
    if(edgeTable[cubeindex] & 1024) {
        vertexInterpolation(threshold, vv[2].vector, vv[6].vector, vv[2].value, vv[6].value, vertList[10]);
        
        getGradient(2, i, j, k, gradients[2]);
        getGradient(6, i, j, k, gradients[6]);
        
        vertexInterpolation(threshold, gradients[2].vector, gradients[6].vector
                            , gradients[2].value, gradients[6].value, gradList[10]);
    }

    // intersects with edge 11
    if(edgeTable[cubeindex] & 2048) {
        vertexInterpolation(threshold, vv[3].vector, vv[7].vector, vv[3].value, vv[7].value, vertList[11]);
        
        getGradient(3, i, j, k, gradients[3]);
        getGradient(7, i, j, k, gradients[7]);
        
        vertexInterpolation(threshold, gradients[3].vector, gradients[7].vector
                            , gradients[3].value, gradients[7].value, gradList[11]);
    }
    
    /* create triangles */
    
  
    for(int ind=0;triTable[cubeindex][ind] != -1;ind+=3) {
        
        vertices.push_back(vertList[triTable[cubeindex][ind]]);
        vertices.push_back(vertList[triTable[cubeindex][ind+1]]);
        vertices.push_back(vertList[triTable[cubeindex][ind+2]]);
        
        normals.push_back(gradList[triTable[cubeindex][ind]]);
        normals.push_back(gradList[triTable[cubeindex][ind+1]]);
        normals.push_back(gradList[triTable[cubeindex][ind+2]]);
        
        numTriangles++;
    }
  
     
}

void ofxKMarchingCubes::vertexInterpolation(float threshold, 
                                            ofVec3f& p1, ofVec3f& p2, 
                                            float valp1, float valp2, 
                                            ofVec3f& vertex) 
{
    float mu;
    
  //  this is case where the resolution is too low anyway
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

void ofxKMarchingCubes::v4Interpolation(float threshold, 
                                            vec4 v1, vec4 v2, 
                                            vec4& vertex) 
{
    float mu;
    /*
     this is case where the resolution is too low anyway
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
     */
    
	mu = (threshold - v1.value) / (v2.value - v1.value);
    
	vertex.vector.x = v1.vector.x + mu * (v2.vector.x - v1.vector.x);
	vertex.vector.y = v1.vector.y + mu * (v2.vector.y - v1.vector.y);
	vertex.vector.z = v1.vector.z + mu * (v2.vector.z - v1.vector.z);    
}







