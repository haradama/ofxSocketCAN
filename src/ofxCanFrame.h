#pragma once

#include <linux/can.h>

#include "ofMain.h"

class ofxCanFrame {
   public:
    ofxCanFrame();
    explicit ofxCanFrame(const can_frame& frame);
    ~ofxCanFrame();

    uint32_t getCanId() const;
    void setCanId(uint32_t id);

    uint8_t getDlc() const;
    void setDlc(uint8_t dlc);

    std::vector<uint8_t> getData() const;
    void setData(const std::vector<uint8_t>& data);

    const can_frame& getRawFrame() const;

   private:
    can_frame frame;
};
