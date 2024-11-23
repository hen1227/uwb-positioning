/*
   UWBTag2DExample.ino
   Henry Abrahamsen
   11/22/24
   Example implementation of UWBPositioning Tag in 2D
   No documentation available at this time
*/

#include "UWBPositioning.h"

// Define the anchors' positions and addresses
UWBAnchor anchor1(2, 3, "ANCHOR001", 0, 0);    // 2D Example
UWBAnchor anchor2(4, 5, "ANCHOR002", 5, 0);

// Define the tag's (starting) position and address
UWBTag tag(6, 7, "TAG001");

void setup() {
    Serial.begin(9600);
    anchor1.initialize();
    anchor2.initialize();

    tag.initialize();
    tag.addAnchor(&anchor1);
    tag.addAnchor(&anchor2);
}

void loop() {
    tag.updateDistances();

    float x, y;
    tag.calculate2DPosition(x, y);

    Serial.print("2D Tag Position: (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(")");
}
