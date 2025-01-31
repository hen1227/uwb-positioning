//
// Created by Henry Abrahamsen on 11/20/24.
// UWBAnchor.cpp
//

#include "UWBAnchor.h"

UWBAnchor::UWBAnchor(int rxPin, int txPin, const std::string &address, float x, float y, float z)
    : UWBDevice(rxPin, txPin, x, y, z), address(address) {
}

void UWBAnchor::initialize() {
    UWBDevice::initialize();
    sendATCommand("AT+MODE=1\r\n");               // Set to ANCHOR
    sendATCommand(("AT+ADDRESS=" + address + "\r\n").c_str());
}

const std::string& UWBAnchor::getAddress() const {
    return address;
}
