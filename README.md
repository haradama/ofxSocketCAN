# ofxSocketCAN

## Description

A simple wrapper of SocketCAN for [openFrameworks](https://openframeworks.cc).

<!-- - **Example Projects**: Includes a sample project to demonstrate the usage of the add-on, helping users get started quickly. -->

## Usage

```cpp
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    interface = "vcan0";
    device.setup(interface);
    if (!device.available()) {
        ofLogError("ofApp")
            << "Failed to set up SocketCAN on interface " << interface;
    } else {
        ofLogNotice("ofApp") << "SocketCAN set up on interface " << interface;
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    if (device.available()) {
        can_frame receiveFrame;
        if (device.receive(receiveFrame)) {
            ofLogNotice("ofApp")
                << "Received frame: ID 0x" << std::hex << receiveFrame.can_id
                << " DLC " << std::dec << (int)receiveFrame.can_dlc
                << " DATA 0x" << std::hex << (int)receiveFrame.data[0] << " 0x"
                << (int)receiveFrame.data[1];
        }
    }
}

```