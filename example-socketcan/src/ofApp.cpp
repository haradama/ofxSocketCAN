#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowTitle("ofxSocketCAN Example");

	if (!can.setup("vcan0")) { // classical CAN on virtual interface
		ofLogError() << "Failed to open CAN";
		return;
	}

	can_frame frame {};
	frame.can_id = 0x321;
	frame.can_dlc = 2;
	frame.data[0] = 0xAB;
	frame.data[1] = 0xCD;

	can.send(frame);
}

void ofApp::update() {
	can_frame rx;
	if (can.receive(rx)) {
		ofLogNotice() << "ID: 0x" << std::hex << rx.can_id << "  len: " << std::dec << rx.can_dlc;
	}
}
