#include <Arduino.h>
#include "UWBTag.h"

UWBTag tag(13, 15);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing Tag...");
    tag.initialize();
}

void loop() {
    tag.update();
    delay(1000);
}
