#include "ofxSocketCAN.h"

ofxSocketCAN::~ofxSocketCAN() { closeSocket(); }

bool ofxSocketCAN::setup(const std::string & interface) {
	return setup(interface, /*enable_can_fd =*/false);
}

bool ofxSocketCAN::setup(const std::string & interface, bool enable_can_fd) {
	closeSocket();
	if (interface.empty()) {
		ofLogError("ofxSocketCAN") << "Interface name is empty.";
		return false;
	}

	sockfd_ = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (sockfd_ < 0) {
		ofLogError("ofxSocketCAN") << "socket() failed: " << std::strerror(errno);
		return false;
	}

	can_fd_enabled_ = enable_can_fd;
	if (can_fd_enabled_ && !enableCANFD()) {
		closeSocket();
		return false;
	}

	struct ifreq ifr {};
	std::strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';

	if (ioctl(sockfd_, SIOCGIFINDEX, &ifr) < 0) {
		ofLogError("ofxSocketCAN") << "ioctl(SIOCGIFINDEX) failed: " << std::strerror(errno);
		closeSocket();
		return false;
	}

	struct sockaddr_can addr {};
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(sockfd_, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
		ofLogError("ofxSocketCAN") << "bind() failed: " << std::strerror(errno);
		closeSocket();
		return false;
	}

	return true;
}

bool ofxSocketCAN::enableCANFD() {
	int enable = 1;
	if (setsockopt(sockfd_, SOL_CAN_RAW, CAN_RAW_FD_FRAMES,
			&enable, sizeof(enable))
		< 0) {
		ofLogError("ofxSocketCAN")
			<< "setsockopt(CAN_RAW_FD_FRAMES) failed: " << std::strerror(errno);
		return false;
	}
	return true;
}

bool ofxSocketCAN::send(const can_frame & frame) const {
	if (!isOpen() || can_fd_enabled_) {
		ofLogError("ofxSocketCAN")
			<< "Socket not open, or CAN-FD mode active (wrong overload).";
		return false;
	}
	ssize_t nbytes = ::write(sockfd_, &frame, sizeof(frame));
	return nbytes == static_cast<ssize_t>(sizeof(frame));
}

bool ofxSocketCAN::send(const canfd_frame & frame) const {
	if (!isOpen() || !can_fd_enabled_) {
		ofLogError("ofxSocketCAN")
			<< "Socket not open, or CAN-FD not enabled (wrong overload).";
		return false;
	}
	ssize_t nbytes = ::write(sockfd_, &frame, sizeof(frame));
	return nbytes == static_cast<ssize_t>(sizeof(frame));
}

bool ofxSocketCAN::receive(can_frame & frame) const {
	if (!isOpen() || can_fd_enabled_) {
		ofLogError("ofxSocketCAN")
			<< "Socket not open, or CAN-FD mode active (wrong overload).";
		return false;
	}
	ssize_t nbytes = ::read(sockfd_, &frame, sizeof(frame));
	return nbytes == static_cast<ssize_t>(sizeof(frame));
}

bool ofxSocketCAN::receive(canfd_frame & frame) const {
	if (!isOpen() || !can_fd_enabled_) {
		ofLogError("ofxSocketCAN")
			<< "Socket not open, or CAN-FD not enabled (wrong overload).";
		return false;
	}
	ssize_t nbytes = ::read(sockfd_, &frame, sizeof(frame));
	return nbytes == static_cast<ssize_t>(sizeof(frame));
}

void ofxSocketCAN::closeSocket() noexcept {
	if (sockfd_ >= 0) {
		::close(sockfd_);
		sockfd_ = -1;
	}
}
