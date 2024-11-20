#include <Arduino.h>
#include "UWBAnchor.h"

UWBAnchor anchor(13, 15);

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing Anchor...");
    anchor.initialize();
}

void loop() {
    anchor.update();

    float distance = anchor.readDistance();
    Serial.print("Current Distance: ");
    Serial.println(distance);

    float avgDistance = anchor.getAverageDistance(10);
    Serial.print("Average Distance (last 10 samples): ");
    Serial.println(avgDistance);

    delay(1000);
}
