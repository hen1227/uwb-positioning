//
// Created by Henry Abrahamsen on 11/20/24.
//

#include "UWBAnchor.h"

UWBAnchor::UWBAnchor(int rxPin, int txPin) {
    serial = new SoftwareSerial(rxPin, txPin);
    distanceIndex = 0;
    memset(distances, 0, sizeof(distances));
}

void UWBAnchor::initialize() {
    serial->begin(9600);
    delay(500);

    bool ok = false;
    while (!ok) {
        sendATCommand("AT\r\n");
        while (serial->available()) {
            char c = serial->read();
            Serial.print(c);
            if (c == 'O') { // Assuming 'OK' is returned
                ok = true;
            }
        }
        delay(500);
    }

    sendATCommand("AT+CPIN=FABC0002EEDCAA90FABC0002EEDCAA90\r\n");
    delay(500);
    Serial.println("Reading...");
    while (serial->available()) {
        Serial.print(char(serial->read()));
    }
}

void UWBAnchor::update() {
    delay(500);
    Serial.println("Reading...");
    while (serial->available()) {
        String data = serial->readStringUntil('\n');
        Serial.println(data);
        float distance = parseDistance(data);
        if (!isnan(distance)) {
            distances[distanceIndex] = distance;
            distanceIndex = (distanceIndex + 1) % 50; // Circular buffer
        }
    }
}

float UWBAnchor::readDistance() {
    int idx = (distanceIndex - 1 + 50) % 50;
    return distances[idx];
}

float UWBAnchor::getAverageDistance(int numSamples) {
    if (numSamples > 50) numSamples = 50;
    float sum = 0;
    int count = 0;
    for (int i = 0; i < numSamples; i++) {
        int idx = (distanceIndex - 1 - i + 50) % 50;
        if (!isnan(distances[idx])) {
            sum += distances[idx];
            count++;
        }
    }
    if (count == 0) return NAN;
    return sum / count;
}

void UWBAnchor::sendATCommand(const char *command) {
    serial->write(command);
}

float UWBAnchor::parseDistance(String data) {
    int idx = data.indexOf("DIST:");
    if (idx != -1) {
        String distanceStr = data.substring(idx + 5);
        distanceStr.trim();
        return distanceStr.toFloat();
    }
    return NAN;
}
