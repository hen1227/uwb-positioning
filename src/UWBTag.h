#ifndef UWB_TAG_H
#define UWB_TAG_H

#include "UWBDevice.h"
#include "UWBAnchor.h"
#include <vector>

class UWBTag : public UWBDevice {
public:
    UWBTag(int rxPin, int txPin, const String &address);
    void initialize();
    void addAnchor(UWBAnchor *anchor);
    void updateDistances();
    float getDistanceToAnchor(const String &anchorAddress);
    float getAverageDistanceToAnchor(const String &anchorAddress, int numSamples);
    void calculate2DPosition(float &x, float &y, float errorMargin = 0.1);
    void calculate3DPosition(float &x, float &y, float &z, float errorMargin = 0.1);

private:
    String address;
    std::vector<UWBAnchor *> anchors;
    std::map<String, float> anchorDistances;
};

#endif // UWB_TAG_H
