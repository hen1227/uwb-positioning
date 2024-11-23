//
// Created by Henry Abrahamsen on 11/20/24.
// UWBTag.cpp
//

#include "UWBTag.h"
#include <math.h>

UWBTag::UWBTag(int rxPin, int txPin, const String &address)
        : UWBDevice(rxPin, txPin), address(address) {}

void UWBTag::initialize() {
    UWBDevice::initialize();
    sendATCommand(("AT+MODE=0\r\n").c_str()); // Set to TAG
    sendATCommand(("AT+ADDRESS=" + address + "\r\n").c_str());
}

void UWBTag::addAnchor(UWBAnchor *anchor) {
    anchors.push_back(anchor);
}

void UWBTag::updateDistances() {
    for (UWBAnchor *anchor : anchors) {
        sendATCommand(("AT+ANCHOR_SEND=" + anchor->getAddress() + ",4,TEST\r\n").c_str());
        String response = readResponse();
        float distance = anchor->parseDistance(response);
        if (!isnan(distance)) {
            anchorDistances[anchor->getAddress()] = distance;
        }
    }
}

float UWBTag::getDistanceToAnchor(const String &anchorAddress) {
    return anchorDistances.count(anchorAddress) ? anchorDistances[anchorAddress] : NAN;
}

float UWBTag::getAverageDistanceToAnchor(const String &anchorAddress, int numSamples) {
    auto it = std::find_if(anchors.begin(), anchors.end(),
                           [&anchorAddress](UWBAnchor *anchor) { return anchor->getAddress() == anchorAddress; });

    if (it == anchors.end()) return NAN; // Anchor not found

    UWBAnchor *anchor = *it;
    float sum = 0;
    int count = 0;

    for (int i = 0; i < numSamples && i < 50; i++) {
        int idx = (anchor->distanceIndex - 1 - i + 50) % 50;
        float distance = anchor->distances[idx];
        if (!isnan(distance)) {
            sum += distance;
            count++;
        }
    }

    if (count == 0) return NAN; // No valid samples
    return sum / count;
}

void UWBTag::calculate2DPosition(float &x, float &y, bool useSecondPoint = false, float errorMargin = 0.1) {
    if (anchors.size() < 2) {
        x = NAN;
        y = NAN;
        return; // Need at least two anchors for 2D triangulation
    }

    // Ensure all anchors are 2D
    for (UWBAnchor *anchor : anchors) {
        if (anchor->is3D()) {
            x = NAN;
            y = NAN;
            return; // Cannot calculate 2D position if any anchor is in 3D
        }
    }

    float x1 = anchors[0]->getX(), y1 = anchors[0]->getY(), r1 = getDistanceToAnchor(anchors[0]->getAddress());
    float x2 = anchors[1]->getX(), y2 = anchors[1]->getY(), r2 = getDistanceToAnchor(anchors[1]->getAddress());

    // Calculate distance between the two anchor points
    float dx = x2 - x1;
    float dy = y2 - y1;
    float d = sqrt(dx * dx + dy * dy);

    if (d > (r1 + r2) || d < fabs(r1 - r2)) {
        x = NAN;
        y = NAN;
        return; // No intersection
    }

    // Solve for intersection points
    float a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    float h = sqrt(r1 * r1 - a * a);

    float cx = x1 + a * (x2 - x1) / d;
    float cy = y1 + a * (y2 - y1) / d;

    float intersectionX1 = cx + h * (y2 - y1) / d;
    float intersectionY1 = cy - h * (x2 - x1) / d;

    float intersectionX2 = cx - h * (y2 - y1) / d;
    float intersectionY2 = cy + h * (x2 - x1) / d;

    // Let the user decide which point to use
    if (useSecondPoint) {
        x = intersectionX2;
        y = intersectionY2;
    } else {
        x = intersectionX1;
        y = intersectionY1;
    }
}


// TODO: Implement this.
// It would be cool to allow the library to also allow for 3d positioning, but
// it isn't necessary for the lunabotics, so I'll implement it later.
void UWBTag::calculate3DPosition(float &x, float &y, float &z, float errorMargin) {
    // 3D implementation using spheres (requires at least 4 anchors for accurate positioning)
    if (anchors.size() < 4) {
        x = NAN;
        y = NAN;
        z = NAN;
        return; // Need at least four anchors for 3D triangulation
    }

    // Example calculation (placeholder for a real implementation using sphere intersection)
    x = 0;
    y = 0;
    z = 0;
}
