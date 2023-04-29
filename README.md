# ofxSocketCAN

A simple wrapper of SocketCAN for [openFrameworks](https://openframeworks.cc).


## Usage

| Method                                                              | Description                                                                                                                                  |
| ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- |
| `bool setup(const std::string& interface, bool use_can_fd = false)` | Initializes the SocketCAN connection with the specified interface and optional CAN FD support. Returns `true` on success, `false` otherwise. |
| `bool send(const can_frame& frame)`                                 | Sends a standard CAN frame. Returns `true` on success, `false` otherwise. Use this method when CAN FD is not enabled.                        |
| `bool send(const canfd_frame& frame)`                               | Sends a CAN FD frame. Returns `true` on success, `false` otherwise. Use this method when CAN FD is enabled.                                  |
| `bool receive(can_frame& frame)`                                    | Receives a standard CAN frame. Returns `true` on success, `false` otherwise. Use this method when CAN FD is not enabled.                     |
| `bool receive(canfd_frame& frame)`                                  | Receives a CAN FD frame. Returns `true` on success, `false` otherwise. Use this method when CAN FD is enabled.                               |
| `bool available()`                                                  | Rutuns `true` if the device is available, `false` otherwise.                                                                                 |

## Example

```cpp
#pragma once

#include "ofMain.h"
#include "ofxSocketCAN.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	ofxSocketCAN device;
	std::string interfaceName;
	can_frame receiveFrame;
		
};

```

```cpp
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

```