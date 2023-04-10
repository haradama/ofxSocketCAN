#pragma once

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ofMain.h"
#include "ofxCanFdFrame.h"
#include "ofxCanFrame.h"

class ofxSocketCAN {
   public:
    ofxSocketCAN();
    ~ofxSocketCAN();

    bool setup(const std::string& interface);
    bool send(const ofxCanFrame& frame);
    bool receive(ofxCanFrame& frame);
    bool send(const ofxCanFdFrame& frame);
    bool receive(ofxCanFdFrame& frame);
    bool available();

   private:
    int _sockfd;
    bool _isAvailable;
};
