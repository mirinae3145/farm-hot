#include <Arduino.h>

#include <MAX30105.h>
#include <heartRate.h>

#include "HeartSensor.h"

bool HeartSensor::begin() {
    if (!sensor.begin(Wire, I2C_SPEED_FAST)) return false;
    sensor.setup();
    sensor.setPulseAmplitudeRed(0x0A);
    sensor.setPulseAmplitudeGreen(0);
    return true;
}

void HeartSensor::update() {
    long irValue = sensor.getIR();
    if (checkForBeat(irValue)) {
        unsigned long now = millis();
        unsigned long delta = now - lastBeat;
        lastBeat = now;
        bpm = 60.0 / (delta / 1000.0);
    }
}

float HeartSensor::getBPM() {
    return bpm;
}

bool HeartSensor::isDanger(unsigned long alertDuration) {
    if (bpm >= 0 && (bpm < 40 || bpm > 170)) {
        if (dangerStart == 0) dangerStart = millis();
        else if (!dangerState && millis() - dangerStart >= alertDuration) {
            dangerState = true;
        }
    } else {
        dangerStart = 0;
        dangerState = false;
    }
    return dangerState;
}

void HeartSensor::print() {
    Serial.print("BPM: ");
    Serial.println(bpm);
}
