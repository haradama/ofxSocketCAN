#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("ofxSocketCAN Example");

    interfaceName = "vcan0";
    device.setup(interfaceName);

    if (device.available()) {
        ofLogNotice("ofApp::setup") << "Connected to " << interfaceName;

        can_frame sendFrame;
        sendFrame.can_id = 0x123;
        sendFrame.can_dlc = 3;
        sendFrame.data[0] = 0x11;
        sendFrame.data[1] = 0x22;
        sendFrame.data[2] = 0x33;

        if (device.send(sendFrame)) {
            ofLogNotice("ofApp::setup") << "Sent CAN frame: ID 0x" << std::hex << sendFrame.can_id;
        } else {
            ofLogError("ofApp::setup") << "Failed to send CAN frame.";
        }
    } else {
        ofLogError("ofApp::setup") << "Failed to connect to " << interfaceName;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (device.available()) {
        if (device.receive(receiveFrame)) {
            ofLogNotice("ofApp::update") << "Received CAN frame: ID 0x" << std::hex << receiveFrame.can_id;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
