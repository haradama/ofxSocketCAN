#pragma once

#include "ofMain.h"
#include "ofxSocketCAN.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();

	ofxSocketCAN can;
};
