#include "IMU.h"
#include "tempSensor.h"
// #include <Wire.h>
TemperatureSensor tempSensor;
#define M_FRAMES            10
#define A_THRESHOLD         0.1f
#define OMEGA_THRESHOLD     5.0f

float aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int bufIndex = 0;
IMU_UpdateResult r;

void setup() {
Serial.begin(38400);
while(!Serial);
Serial.println("serial success");

// Wire.begin();
if (!tempSensor.begin()) {
    Serial.println("Temprature sensor initialization failed. Check the connections.");
    while (1);
  }

  Serial.println("Temperature sensor initialized successfully.");

  IMU_Config cfg = {M_FRAMES, A_THRESHOLD, OMEGA_THRESHOLD};
    if(!IMU_begin(cfg)) {
        Serial.println("IMU failed");
        while(true);
    }
    Serial.println("IMU success");


Wire.setClock(100000);

}

void loop() {
r = IMU_update();
    if(r.hasData) {
        aBuf[bufIndex] = r.aMag;
        omegaBuf[bufIndex] = r.omegaRate;
        bufIndex = (bufIndex + 1) % M_FRAMES;
        Serial.print("aMag \t");
        Serial.println(r.aMag);
        Serial.print("omega \t");
        Serial.println(r.omegaRate);
    }
    else {
      Serial.println("imu nodata");
    }
delay(10);
  tempSensor.update();  // Update the temperature.
  float temp = tempSensor.getTemperature();
  Serial.print("temp \t");
  Serial.println(temp);
}