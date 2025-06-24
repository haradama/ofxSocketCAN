#pragma once
#include "ofMain.h"

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>

class ofxSocketCAN {
public:
	ofxSocketCAN() = default;
	~ofxSocketCAN();

	bool setup(const std::string & interface);
	bool setup(const std::string & interface, bool enable_can_fd);

	bool send(const can_frame & frame) const;
	bool send(const canfd_frame & frame) const;

	bool receive(can_frame & frame) const;
	bool receive(canfd_frame & frame) const;

	[[nodiscard]] bool isOpen() const noexcept { return sockfd_ >= 0; }

private:
	ofxSocketCAN(const ofxSocketCAN &) = delete;
	ofxSocketCAN & operator=(const ofxSocketCAN &) = delete;

	bool enableCANFD();
	void closeSocket() noexcept;

	int sockfd_ { -1 };
	bool can_fd_enabled_ { false };
};
