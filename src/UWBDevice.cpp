//
// Created by Henry Abrahamsen on 11/22/24.
// UWBDevice.cpp
//

#include "UWBDevice.h"

UWBDevice::UWBDevice(int rxPin, int txPin, float x, float y, float z)
    : x(x), y(y), z(z)
{
    // Create a SoftwareSerial on the given RX/TX pins
    serial = new SoftwareSerial(rxPin, txPin);
}

UWBDevice::~UWBDevice()
{
    delete serial;
}

float UWBDevice::getX() const { return x; }
float UWBDevice::getY() const { return y; }
float UWBDevice::getZ() const { return z; }
bool UWBDevice::is3D() const { return (z != 0.0); }

void UWBDevice::initialize()
{
    // Start SoftwareSerial at 9600 baud
    serial->begin(9600);
    delay(500);

    // Attempt to get an "OK" from the device up to 5 times
    const int maxAttempts = 5;
    for (int i = 0; i < maxAttempts; i++) {
        sendATCommand("AT\r\n");
        String response = readResponse();
        if (response.indexOf("OK") != -1) {
            // Device responded properly
            return;
        }
        delay(500);
    }

    // If we get here, we never received an OK.
    // You could add further error handling or flags here.
}

void UWBDevice::sendATCommand(const char *command)
{
    serial->println(command);
}

String UWBDevice::readResponse()
{
    String response;
    unsigned long timeout = millis() + 1000; // 1s per read cycle
    while (millis() < timeout) {
        while (serial->available()) {
            char c = (char)serial->read();
            response += c;
        }
    }
    return response;
}
