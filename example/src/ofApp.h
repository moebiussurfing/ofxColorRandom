#pragma once

#include "ofMain.h"

#include "ofxColorRandom.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		ofxColorRandom colorRandom;
};
