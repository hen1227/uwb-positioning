/*
   UWBAnchor2DExample.ino
   Henry Abrahamsen
   11/22/24
   Example implementation of UWBPositioning Anchor in 2D
   No documentation available at this time
*/

#include "UWBPositioning.h"

UWBAnchor anchor1(2, 3, "ANCHOR001", 0, 0);   // 2D: (0, 0)
UWBAnchor anchor2(4, 5, "ANCHOR002", 5, 0);   // 2D: (5, 0)

void setup() {
    Serial.begin(9600);
    anchor1.initialize();
    anchor2.initialize();
}

void loop() {
    anchor1.update();
    anchor2.update();
}
