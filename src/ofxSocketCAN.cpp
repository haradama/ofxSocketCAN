#include "ofxSocketCAN.h"

ofxSocketCAN::ofxSocketCAN() : sockfd(-1), can_fd_enabled(false), is_available(false) {}

ofxSocketCAN::~ofxSocketCAN() {
    if (sockfd >= 0) {
        close(sockfd);
    }

    is_available = false;
}

bool ofxSocketCAN::setup(const std::string& interface, bool use_can_fd) {
    can_fd_enabled = use_can_fd;

    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd < 0) {
        ofLogError("ofxSocketCAN") << "Error opening socket.";
        return false;
    }

    if (can_fd_enabled) {
        int enable = 1;
        if (setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable, sizeof(enable)) < 0) {
            ofLogError("ofxSocketCAN") << "Error enabling CAN FD support.";
            close(sockfd);
            sockfd = -1;
            return false;
        }
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        ofLogError("ofxSocketCAN") << "Error getting interface index.";
        close(sockfd);
        sockfd = -1;
        return false;
    }

    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ofLogError("ofxSocketCAN") << "Error binding socket.";
        close(sockfd);
        sockfd = -1;
        return false;
    }

    is_available = true;

    return true;
}

bool ofxSocketCAN::send(const can_frame& frame) {
    if (sockfd < 0) {
        return false;
    }

    if (can_fd_enabled) {
        ofLogError("ofxSocketCAN") << "CAN FD is enabled. Use send(const canfd_frame&) instead.";
        return false;
    }

    int nbytes = write(sockfd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

bool ofxSocketCAN::send(const canfd_frame& frame) {
    if (sockfd < 0) {
        return false;
    }

    if (!can_fd_enabled) {
        ofLogError("ofxSocketCAN") << "CAN FD is not enabled. Use send(const can_frame&) instead.";
        return false;
    }

    int nbytes = write(sockfd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

bool ofxSocketCAN::receive(can_frame& frame) {
    if (sockfd < 0) {
        return false;
    }

    if (can_fd_enabled) {
        ofLogError("ofxSocketCAN") << "CAN FD is enabled. Use receive(canfd_frame&) instead.";
        return false;
    }

    int nbytes = read(sockfd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

bool ofxSocketCAN::receive(canfd_frame& frame) {
    if (sockfd < 0) {
        return false;
    }

    if (!can_fd_enabled) {
        ofLogError("ofxSocketCAN") << "CAN FD is not enabled. Use receive(can_frame&) instead.";
        return false;
    }

    int nbytes = read(sockfd, &frame, sizeof(frame));
    return nbytes == sizeof(frame);
}

bool ofxSocketCAN::available() {return is_available;}
