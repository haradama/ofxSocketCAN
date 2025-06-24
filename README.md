# ofxSocketCAN

A minimal, **header‑only** wrapper that makes Linux [SocketCAN](https://www.kernel.org/doc/Documentation/networking/can.txt) available inside [openFrameworks](https://openframeworks.cc).
It lets you send and receive both classical CAN and CAN‑FD frames with a simple, modern C++17 interface that follows openFrameworks logging conventions.

## Features

* Classical CAN & CAN‑FD
* RAII — socket is closed automatically
* Detailed error logs via `ofLog*`
* `[[nodiscard]]` returns to prevent silent failures

## Prerequisites

| Requirement               | Notes                                                                           |
| ------------------------- | ------------------------------------------------------------------------------- |
| **Linux kernel ≥ 3.6**    | SocketCAN is part of the mainline kernel from 2.6.25, but CAN‑FD requires ≥ 3.6 |
| **openFrameworks ≥ 0.12** | Tested with 0.12.2 (gcc/clang, Make & QtCreator)                                |
| **C++17 toolchain**       | E.g. GCC 10+, Clang 12+                                                         |

## Installation

```bash
cd /path/to/openFrameworks/addons
# HTTPS
git clone https://github.com/haradama/ofxSocketCAN.git
# or SSH
git clone git@github.com:haradama/ofxSocketCAN.git
```

Add the addon to your project with the **projectGenerator** or by editing `addons.make`.

## Quick Start

```cpp
#include "ofxSocketCAN.h"

ofxSocketCAN can;

void ofApp::setup() {
    if (!can.setup("vcan0")) { // classical CAN on virtual interface
        ofLogError() << "Failed to open CAN";
        return;
    }

    can_frame frame{};
    frame.can_id  = 0x321;
    frame.can_dlc = 2;
    frame.data[0] = 0xAB;
    frame.data[1] = 0xCD;

    can.send(frame);
}

void ofApp::update() {
    can_frame rx;
    if (can.receive(rx)) {
        ofLogNotice() << "ID: 0x" << std::hex << rx.can_id << "  len: " << std::dec << rx.can_dlc;
    }
}
```

## API Reference

| Method                                                      | Description                                                        |
| ----------------------------------------------------------- | ------------------------------------------------------------------ |
| `bool setup(const std::string& iface, bool enableFD=false)` | Open the interface and optionally enable CAN‑FD.                   |
| `bool send(const can_frame& frame)`                         | Send a classical 8‑byte frame (fails if FD mode is enabled).       |
| `bool send(const canfd_frame& frame)`                       | Send a CAN‑FD frame up to 64 bytes (fails if FD mode is disabled). |
| `bool receive(can_frame& frame)`                            | Receive a classical frame (blocking).                              |
| `bool receive(canfd_frame& frame)`                          | Receive a CAN‑FD frame (blocking).                                 |
| `bool isOpen() const`                                       | `true` if the socket is valid.                                     |

All functions return `false` on error; check the log for details.

## Build & Run

1. **Generate** a project with the addon.
2. Ensure **`PKG_CONFIG_PATH`** includes *libsocketcan* if you use it for extra helpers (not required for this addon).
3. **Compile & run** with your usual oF workflow.

> For CAN‑FD you also need to bring the interface up with an appropriate data‑phase bitrate, e.g.:
>
> ```bash
> sudo ip link set can0 type can bitrate 500000 dbitrate 2000000 fd on
> sudo ip link set up can0
> ```

## Error Handling

* On failure, the class logs with `ofLogError("ofxSocketCAN")` and returns `false`.
* Always check return values (the methods are marked `[[nodiscard]]`).
* Use `dmesg` or `candump` for deeper diagnosis.

## License

`ofxSocketCAN` is released under the **MIT License** — see [LICENSE](LICENSE) for details.
