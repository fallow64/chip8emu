#pragma once

#include <cstdint>
#include <string>

class IPlatform {
  public:
    IPlatform() {}

    // Update the display with the given pixel buffer and pitch
    virtual void Update(void const *buffer, int pitch) = 0;
    // Process input and update the keys array. Return true to quit.
    virtual bool ProcessInput(uint8_t *keys) = 0;
};
