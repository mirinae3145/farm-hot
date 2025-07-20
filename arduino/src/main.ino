// #define DEBUG_SERIAL
#include <Arduino.h>
#if defined(ESP32)
    #include <esp_log.h>
#endif

#include "IMU.h"
#define M_FRAMES            10
#define A_THRESHOLD         0.1f
#define OMEGA_THRESHOLD     5.0f
float aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int bufIndex = 0;
IMU_UpdateResult r;

#include "tempSensor.h"
TemperatureSensor tempSensor;

#include "HeartSensor.h"
HeartSensor heart;

#include "BT.h"

uint8_t danger[3] = {0, 0, 0};
long last = 0;
int iter = 0;

void setup() {
    #if defined(ESP32)
        esp_log_level_set("i2c", ESP_LOG_NONE);
    #endif

    Serial.begin(38400);
    while(!Serial);
    Serial.println("Serial success");

    if (!BT_begin()) {
        Serial.println("BT failed");
        while (1);
    }
    Serial.println("BT success");

    #if defined(ESP32)
        Wire.begin(6,7);
    #else
        Wire.begin();
    #endif

    if (!tempSensor.begin()) {
        Serial.println("GY-906 failed");
        while (1);
    }
    Serial.println("GY-906 success");

    IMU_Config cfg = {M_FRAMES, A_THRESHOLD, OMEGA_THRESHOLD};
    if(!IMU_begin(cfg)) {
        Serial.println("MPU-6050 failed");
        while(1);
    }
    Serial.println("MPU-6050 success");

    if (!heart.begin()) {
        Serial.println("MAX-30105 failed");
        while(1);
    }
    Serial.println("MAX-30105 success");

    Wire.setClock(100000);
}

void loop() {
    tempSensor.update();
    float temp = tempSensor.getTemperature();
    #ifdef DEBUG_SERIAL
        Serial.print("temp \t");
        Serial.println(temp);
    #endif

    heart.update();

    r = IMU_update();
    if (r.hasData) {
        aBuf[bufIndex] = r.aMag;
        omegaBuf[bufIndex] = r.omegaRate;
        bufIndex = (bufIndex + 1) % M_FRAMES;

        #ifdef DEBUG_SERIAL
          Serial.print("aMag \t");
          Serial.println(r.aMag);
          Serial.print("omega \t");
          Serial.println(r.omegaRate);
        #endif
    }
    else {
      #ifdef DEBUG_SERIAL
        Serial.println("IMU no data");
      #endif
    }
    if (aBuf[M_FRAMES - 1]) {
        if (IMU_isNoMotion(aBuf, omegaBuf))
            iter++;
        else
            iter = 0;
    }

    danger[0] = tempSensor.isTempOkay() ? 1 : 0;
    danger[1] = heart.isDanger() ? 0 : 1;
    danger[2] = (iter > 100) ? 0 : 1;

    if (millis() - last > 1000) {
        BT_sendDanger(danger);
        #ifdef DEBUG_SERIAL
            Serial.print("Danger: ");
            Serial.print(danger[0]);
            Serial.print(", ");
            Serial.print(danger[1]);
            Serial.print(", ");
            Serial.println(danger[2]);
        #endif
        last = millis();
    }
}