#include "ofxCanFrame.h"

ofxCanFrame::ofxCanFrame() { memset(&frame, 0, sizeof(frame)); }

ofxCanFrame::ofxCanFrame(const can_frame& frame) : frame(frame) {}

ofxCanFrame::~ofxCanFrame() {}

uint32_t ofxCanFrame::getCanId() const { return frame.can_id; }

void ofxCanFrame::setCanId(uint32_t id) { frame.can_id = id; }

uint8_t ofxCanFrame::getDlc() const { return frame.can_dlc; }

void ofxCanFrame::setDlc(uint8_t dlc) { frame.can_dlc = dlc; }

std::vector<uint8_t> ofxCanFrame::getData() const {
    return std::vector<uint8_t>(frame.data, frame.data + frame.can_dlc);
}

void ofxCanFrame::setData(const std::vector<uint8_t>& data) {
    frame.can_dlc =
        std::min(static_cast<uint8_t>(data.size()), static_cast<uint8_t>(8));
    memcpy(frame.data, data.data(), frame.can_dlc);
}

const can_frame& ofxCanFrame::getRawFrame() const { return frame; }
