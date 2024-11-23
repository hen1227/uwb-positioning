//
// Created by Henry Abrahamsen on 11/20/24.
// UWBAnchor.cpp
//

#include "UWBAnchor.h"

UWBAnchor::UWBAnchor(int rxPin, int txPin, const String &address, float x, float y, float z)
        : UWBDevice(rxPin, txPin), address(address), x(x), y(y), z(z), distanceIndex(0) {
    memset(distances, 0, sizeof(distances));
}

float UWBAnchor::getX() const { return x; }
float UWBAnchor::getY() const { return y; }
float UWBAnchor::getZ() const { return z; }


void UWBAnchor::initialize() {
    UWBDevice::initialize();
    sendATCommand(("AT+MODE=1\r\n").c_str()); // Set to ANCHOR
    sendATCommand(("AT+ADDRESS=" + address + "\r\n").c_str());
}

const String &UWBAnchor::getAddress() const {
    return address;
}

float UWBAnchor::readDistance() {
    // Example of getting distance (can be triggered by TAG)
    String response = readResponse();
    return parseDistance(response);
}

float UWBAnchor::parseDistance(const String &response) {
    int idx = response.indexOf("DIST:");
    if (idx != -1) {
        String distStr = response.substring(idx + 5);
        distStr.trim();
        return distStr.toFloat();
    }
    return NAN;
}


