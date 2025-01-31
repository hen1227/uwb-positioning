#ifndef UWB_TAG_H
#define UWB_TAG_H

#include "UWBDevice.h"
#include "UWBAnchor.h"
#include <vector>
#include <map>

// Helper struct to hold the rolling history for each anchor
struct DistanceHistory {
    float distances[50];
    int index;
    DistanceHistory() : index(0) {
        for (int i = 0; i < 50; i++) {
            distances[i] = NAN;
        }
    }
};

class UWBTag : public UWBDevice {
public:
    UWBTag(int rxPin, int txPin, const String &address);

    void initialize();
    void addAnchor(UWBAnchor *anchor);

    void updateDistances();   // Sends request to each anchor, reads distance
    float getDistanceToAnchor(const String &anchorAddress);
    float getAverageDistanceToAnchor(const String &anchorAddress, int numSamples);

    void calculate2DPosition(float &x, float &y, bool useSecondPoint = false, float errorMargin = 0.1);
    void calculate3DPosition(float &x, float &y, float &z, float errorMargin = 0.1);

private:
    String address;
    std::vector<UWBAnchor *> anchors;

    // Each anchor address => ring buffer of distances
    std::map<String, DistanceHistory> distanceHistories;

    float parseDistance(const String &response);
};

#endif // UWB_TAG_H
