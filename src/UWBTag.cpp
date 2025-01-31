//
// Created by Henry Abrahamsen on 11/20/24.
// UWBTag.cpp
//

#include "UWBTag.h"
#include <math.h>

UWBTag::UWBTag(int rxPin, int txPin, const String &address) : UWBDevice(rxPin, txPin), address(address) {}

void UWBTag::initialize() {
    UWBDevice::initialize();
    sendATCommand("AT+MODE=0\r\n"); // Set to TAG
    sendATCommand(("AT+ADDRESS=" + address + "\r\n").c_str());
}

void UWBTag::addAnchor(UWBAnchor *anchor) {
    anchors.push_back(anchor);
}

// This helper extracts "DIST:xx.x" from a response
float UWBTag::parseDistance(const String &response) {
    int idx = response.indexOf("DIST:");
    if (idx == -1) return NAN;
    String distStr = response.substring(idx + 5);
    distStr.trim();
    return distStr.toFloat();
}

void UWBTag::updateDistances() {
    for (auto *anchor : anchors) {
        // Example AT command to trigger a distance reading from the anchor
        sendATCommand(("AT+ANCHOR_SEND=" + anchor->getAddress() + ",4,TEST\r\n").c_str());
        String response = readResponse();

        float distance = parseDistance(response);
        if (!isnan(distance)) {
            // Store in ring buffer for this anchor address
            DistanceHistory &hist = distanceHistories[anchor->getAddress()];
            hist.distances[hist.index] = distance;
            hist.index = (hist.index + 1) % 50;
        }
    }
}

float UWBTag::getDistanceToAnchor(const String &anchorAddress) {
    // Return the most recent distance
    auto it = distanceHistories.find(anchorAddress);
    if (it == distanceHistories.end()) {
        return NAN;
    }
    const DistanceHistory &hist = it->second;
    // The most recent entry is at hist.index - 1
    int idx = (hist.index - 1 + 50) % 50;
    return hist.distances[idx];
}

float UWBTag::getAverageDistanceToAnchor(const String &anchorAddress, int numSamples) {
    auto it = distanceHistories.find(anchorAddress);
    if (it == distanceHistories.end()) {
        return NAN; // anchor not found
    }

    const DistanceHistory &hist = it->second;
    float sum = 0;
    int count = 0;

    for (int i = 0; i < numSamples && i < 50; i++) {
        int idx = (hist.index - 1 - i + 50) % 50;
        float val = hist.distances[idx];
        if (!isnan(val)) {
            sum += val;
            count++;
        }
    }
    if (count == 0) return NAN;
    return sum / count;
}

void UWBTag::calculate2DPosition(float &x, float &y, bool useSecondPoint, float errorMargin) {
    if (anchors.size() < 2) {
        x = NAN;
        y = NAN;
        return;
    }

    // For 2D calc, we assume anchor z=0
    float x1 = anchors[0]->getX();
    float y1 = anchors[0]->getY();
    float r1 = getDistanceToAnchor(anchors[0]->getAddress());

    float x2 = anchors[1]->getX();
    float y2 = anchors[1]->getY();
    float r2 = getDistanceToAnchor(anchors[1]->getAddress());

    if (isnan(r1) || isnan(r2)) {
        x = NAN;
        y = NAN;
        return;
    }

    float dx = x2 - x1;
    float dy = y2 - y1;
    float d = sqrt(dx * dx + dy * dy);

    // Basic circle intersection check
    if (d > (r1 + r2) || d < fabs(r1 - r2)) {
        x = NAN;
        y = NAN;
        return;
    }

    float a = (r1*r1 - r2*r2 + d*d) / (2 * d);
    float h = sqrt(r1*r1 - a*a);

    float cx = x1 + a * (dx / d);
    float cy = y1 + a * (dy / d);

    float rx = -dy * (h / d);
    float ry =  dx * (h / d);

    float intersectionX1 = cx + rx;
    float intersectionY1 = cy + ry;
    float intersectionX2 = cx - rx;
    float intersectionY2 = cy - ry;

    // Decide which intersection
    if (useSecondPoint) {
        x = intersectionX2;
        y = intersectionY2;
    } else {
        x = intersectionX1;
        y = intersectionY1;
    }
}

void UWBTag::calculate3DPosition(float &x, float &y, float &z, float errorMargin) {
    // Placeholder, needs full sphere intersection approach
    if (anchors.size() < 4) {
        x = NAN;
        y = NAN;
        z = NAN;
        return;
    }
    // Example dummy
    x = 0;
    y = 0;
    z = 0;
}
