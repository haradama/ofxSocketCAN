#pragma once

#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/fd.h>

#include "ofMain.h"

class ofxCanFdFrame {
   public:
    ofxCanFdFrame();
    explicit ofxCanFdFrame(const canfd_frame& frame);
    ~ofxCanFdFrame();

    uint32_t getCanId() const;
    void setCanId(uint32_t id);

    uint8_t getDlc() const;
    void setDlc(uint8_t dlc);

    std::vector<uint8_t> getData() const;
    void setData(const std::vector<uint8_t>& data);

    const canfd_frame& getRawFrame() const;

   private:
    canfd_frame frame;
};
