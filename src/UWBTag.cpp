//
// Created by Henry Abrahamsen on 11/20/24.
//

#include "UWBTag.h"

// Maybe should incldue in .h?
#include <iostream>
#include <string>

UWBTag::UWBTag(int rxPin, int txPin) {
    serial = new SoftwareSerial(rxPin, txPin);
}

void UWBTag::initialize(int address) {
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
    sendATCommand("AT+MODE=0"); //set tag
    delay(500);

    sendSerialData("AT+NETWORKID=NULUNABOTICS"); //set network id
    delay(500);

    sendSerialData("AT+ADDRESS=NUL" + std::to_string(address)); //set address
    delay(500);

    sendATCommand("AT+CPIN=FABC0002EEDCAA90FABC0002EEDCAA90\r\n");
    delay(500);
    Serial.println("Reading...");
    while (serial->available()) {
        Serial.print(char(serial->read()));
    }
}

void UWBTag::update() {
    sendATCommand("AT+ANCHOR_SEND=NULBTAGG,4,TEST\r\n");
    delay(300);
    Serial.println("Reading...");
    while (serial->available()) {
        Serial.print(char(serial->read()));
    }
}

void UWBTag::sendATCommand(const char *command) {
    serial->write(command);
}
