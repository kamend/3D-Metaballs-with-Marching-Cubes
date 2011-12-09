#pragma once

#include "ofMain.h"


class MetaBall: public ofVec3f{ 
public:
	ofVec3f accel, vel;
    float maxspeed;
	float size;
        
	MetaBall(){
        maxspeed = 10.0;
		size = ofRandom(2, 5);
	}
	
	void init(const ofVec3f& _pos){
		x = _pos.x;
		y = _pos.y;
        z = _pos.z;
	}
	
	void goTo(const ofVec3f& target, float k = 0.1f, float damp = 0.9f){
		accel = (target - *this)*k;
		vel += accel;
		vel *= damp;
		*this += vel;
	}
	
	void update(const ofVec3f& _force, float damp = 0.9f){
		vel += _force;
		vel *= damp;
        
        vel.limit(maxspeed);
        
		*this += vel;
	}
};

