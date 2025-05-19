#pragma once
#include <MAX30105.h>

class HeartSensor {
public:
    bool begin();
    void update();
    float getBPM();
    bool isDanger(unsigned long alertDuration = 5000); // 5초 이상 위험 BPM?
    void print();

private:
    MAX30105 sensor;
    float bpm = 0;
    unsigned long lastBeat = 0;
    unsigned long dangerStart = 0;
    bool dangerState = false;
};
