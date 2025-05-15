#include <Wire.h>
#include "IMU.h"

#define M_FRAMES            10
#define A_THRESHOLD         0.1f
#define OMEGA_THRESHOLD     5.0f

float aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int bufIndex = 0;
IMU_UpdateResult r;

void setup() {
    Wire.begin();
    Wire.setClock(400000); 
  
    Serial.begin(115200);
    while(!Serial);

    IMU_Config cfg = {M_FRAMES, A_THRESHOLD, OMEGA_THRESHOLD};
    IMU_begin(cfg);
}

void loop() {
    r = IMU_update();
    if(r.hasData) {
        aBuf[bufIndex] = r.aMag;
        omegaBuf[bufIndex] = r.omegaRate;
        bufIndex = (bufIndex + 1) % M_FRAMES;

        Serial.println(r.aMag);
        Serial.println(r.omegaRate);
    }
    
    if(aBuf[M_FRAMES - 1]) {
        if (IMU_isNoMotion(aBuf, omegaBuf)) {
            Serial.println("no motion detected");
        } else {
            Serial.println("motion detected");
        }
    }
}
