#pragma once

#include "ofMain.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

class ofxSocketCAN {
public:
    ofxSocketCAN();
    ~ofxSocketCAN();

    bool setup(const std::string& interface, bool use_can_fd = false);
    bool send(const can_frame& frame);
    bool send(const canfd_frame& frame);
    bool receive(can_frame& frame);
    bool receive(canfd_frame& frame);
    bool available();

private:
    int sockfd;
    bool can_fd_enabled;
    bool is_available;
};
