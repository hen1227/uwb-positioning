//
// Created by Henry Abrahamsen on 11/22/24.
// UWBDevice.h
//


#ifndef UWB_DEVICE_H
#define UWB_DEVICE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class UWBDevice {
public:
    UWBDevice(int rxPin, int txPin, float x, float y, float z = 0); // Default z = 0 for 2D mode
    virtual ~UWBDevice();

    void initialize();
    float getX() const;
    float getY() const;
    float getZ() const;
    bool is3D() const;

protected:
    SoftwareSerial *serial;
    float x, y, z;
    void sendATCommand(const char *command);
    String readResponse();
};


#endif // UWB_DEVICE_H
