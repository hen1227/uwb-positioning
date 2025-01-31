//
// Created by Henry Abrahamsen on 11/22/24.
// UWBAnchor.h
//


#ifndef UWB_ANCHOR_H
#define UWB_ANCHOR_H

#include "UWBDevice.h"
#include <vector>

class UWBAnchor : public UWBDevice {
public:
    UWBAnchor(int rxPin, int txPin, const String &address, float x, float y, float z = 0);

    void initialize();
    const String& getAddress() const;

    // Using base class x,y,z for anchor’s location
    // getX(), getY(), getZ() already inherited from UWBDevice

private:
    String address;
};

#endif // UWB_ANCHOR_H
