//
// Created by Henry Abrahamsen on 11/20/24.
//

#ifndef UWBPOSITIONING_UWB_TAG_H
#define UWBPOSITIONING_UWB_TAG_H


#include <Arduino.h>
#include <SoftwareSerial.h>

class UWBTag {
public:
    UWBTag(int rxPin, int txPin);
    void initialize();
    void update();

private:
    SoftwareSerial *serial;
    void sendATCommand(const char *command);
};

#endif //UWBPOSITIONING_UWB_TAG_H
