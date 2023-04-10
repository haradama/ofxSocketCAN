#include "ofxCanFdFrame.h"

ofxCanFdFrame::ofxCanFdFrame() { memset(&frame, 0, sizeof(frame)); }

ofxCanFdFrame::ofxCanFdFrame(const canfd_frame& frame) : frame(frame) {}

ofxCanFdFrame::~ofxCanFdFrame() {}

uint32_t ofxCanFdFrame::getCanId() const { return frame.can_id; }

void ofxCanFdFrame::setCanId(uint32_t id) { frame.can_id = id; }

uint8_t ofxCanFdFrame::getDlc() const { return frame.len; }

void ofxCanFdFrame::setDlc(uint8_t dlc) { frame.len = dlc; }

std::vector<uint8_t> ofxCanFdFrame::getData() const {
    return std::vector<uint8_t>(frame.data, frame.data + frame.len);
}

void ofxCanFdFrame::setData(const std::vector<uint8_t>& data) {
    frame.len = std::min(static_cast<uint8_t>(data.size()),
                         static_cast<uint8_t>(CANFD_MAX_DLEN));
    memcpy(frame.data, data.data(), frame.len);
}

const canfd_frame& ofxCanFdFrame::getRawFrame() const { return frame; }
