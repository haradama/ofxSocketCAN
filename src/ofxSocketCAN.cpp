#include "ofxSocketCAN.h"

// Default constructor for ofxSocketCAN
ofxSocketCAN::ofxSocketCAN() : _sockfd(-1), _isAvailable(false) {}

// Destructor for ofxSocketCAN
ofxSocketCAN::~ofxSocketCAN() {
    // Close the socket if it's open
    if (_sockfd >= 0) {
        close(_sockfd);

        // Set availability to false
        _isAvailable = false;
    }
}

// Setup function for ofxSocketCAN
bool ofxSocketCAN::setup(const std::string& interface) {
    // Create a socket and store the socket file descriptor
    _sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Check if the socket was created successfully
    if (_sockfd < 0) {
        // Log an error message if the socket creation failed
        ofLogError("ofxSocketCAN") << "Error opening socket.";
        return false;
    }

    // Initialize the interface request structure
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    // Get the interface index
    if (ioctl(_sockfd, SIOCGIFINDEX, &ifr) < 0) {
        // Log an error message if the ioctl call failed
        ofLogError("ofxSocketCAN") << "Error getting interface index.";
        close(_sockfd);
        _sockfd = -1;
        return false;
    }

    // Initialize the socket address structure
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Bind the socket to the interface
    if (bind(_sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        // Log an error message if the bind call failed
        ofLogError("ofxSocketCAN") << "Error binding socket.";
        close(_sockfd);
        _sockfd = -1;
        return false;
    }

    // Set availability to true
    _isAvailable = true;

    // Return true for successful setup
    return true;
}

// Send function for CAN
bool ofxSocketCAN::send(const ofxCanFrame& ofxcanframe) {
    // Return false if the socket is not open
    if (sockfd < 0) {
        return false;
    }

    // Write the frame to the socket
    const can_frame& frame = ofxcanframe.getRawFrame();
    int nbytes = write(sockfd, &frame, sizeof(frame));

    // Check if the write was successful
    return nbytes == sizeof(frame);
}

// Send function for CAN FD
bool ofxSocketCAN::send(const ofxCanFdFrame& ofxcanfdframe) {
    // Return false if the socket is not open
    if (sockfd < 0) {
        return false;
    }

    // Write the frame to the socket
    const canfd_frame& frame = ofxcanfdframe.getRawFrame();
    int nbytes = write(sockfd, &frame, sizeof(frame));

    // Check if the write was successful
    return nbytes == sizeof(frame);
}

// Receive function for CAN
bool ofxSocketCAN::receive(ofxCanFrame& ofxcanframe) {
    // Return false if the socket is not open
    if (sockfd < 0) {
        return false;
    }

    // Read a frame from the socket
    canfd_frame frame;
    int nbytes = read(sockfd, &frame, sizeof(frame));

    // Check if the read was successful
    if (nbytes == sizeof(frame)) {
        ofxcanframe = ofxCanFrame(frame);
        return true;
    }

    return false;
}

// Receive function for CAN FD
bool ofxSocketCAN::receive(ofxCanFdFrame& ofxcanfdframe) {
    // Return false if the socket is not open
    if (sockfd < 0) {
        return false;
    }

    // Read a frame from the socket
    canfd_frame frame;
    int nbytes = read(sockfd, &frame, sizeof(frame));

    // Check if the read was successful
    if (nbytes == sizeof(frame)) {
        ofxcanfdframe = ofxCanFdFrame(frame);
        return true;
    }

    return false;
}

// Check availability of ofxSocketCAN
bool ofxSocketCAN::available() { return _isAvailable; }
