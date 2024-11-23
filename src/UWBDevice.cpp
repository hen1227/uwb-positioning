//
// Created by Henry Abrahamsen on 11/22/24.
// UWBDevice.cpp
//

#include "UWBDevice.h"

UWBDevice::UWBDevice(int rxPin, int txPin, float x, float y, float z)
        : x(x), y(y), z(z) {
    serial = new SoftwareSerial(rxPin, txPin);
}

UWBDevice::~UWBDevice() {
    delete serial;
}

float UWBDevice::getX() const { return x; }
float UWBDevice::getY() const { return y; }
float UWBDevice::getZ() const { return z; }
bool UWBDevice::is3D() const { return z != 0; } // If z = 0, it's in 2D mode

void UWBDevice::initialize() {
    serial->begin(9600);
    delay(500);
    while (true) {
        sendATCommand("AT\r\n");
        String response = readResponse();
        if (response.indexOf("OK") != -1) break;
        delay(500);
    }
}

void UWBDevice::sendATCommand(const char *command) {
    serial->println(command);
}

String UWBDevice::readResponse() {
    String response = "";
    long timeout = millis() + 1000;
    while (millis() < timeout) {
        if (serial->available()) {
            char c = serial->read();
            response += c;
        }
    }
    return response;
}
