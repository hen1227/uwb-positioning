//
// Created by Henry Abrahamsen on 11/20/24.
//

#ifndef UWBPOSITIONING_UWB_ANCHOR_H
#define UWBPOSITIONING_UWB_ANCHOR_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class UWBAnchor {
public:
    UWBAnchor(int rxPin, int txPin);
    void initialize();
    void update();
    float readDistance();
    float getAverageDistance(int numSamples);

private:
    SoftwareSerial *serial;
    void sendATCommand(const char *command);
    float distances[50]; // Stores up to 50 distance samples
    int distanceIndex;
    float parseDistance(String data);
};

#endif //UWBPOSITIONING_UWB_ANCHOR_H
