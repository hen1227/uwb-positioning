//
// Created by Henry Abrahamsen on 11/22/24.
// UWBAnchor.h
//


#ifndef UWB_ANCHOR_H
#define UWB_ANCHOR_H

#include "UWBDevice.h"

class UWBAnchor : public UWBDevice {
public:
    UWBAnchor(int rxPin, int txPin, const String &address, float x, float y, float z = 0);
    void initialize() override;
    const String &getAddress() const;
    float getX() const;
    float getY() const;
    float getZ() const;
    float readDistance();
    float getAverageDistance(int numSamples);

private:
    String address;
    float x, y, z; // Position
    float distances[50];
    int distanceIndex;
    float parseDistance(const String &response);
};


#endif // UWB_ANCHOR_H
