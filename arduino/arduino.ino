#include "IMU.h"
#include "tempSensor.h"
// #include <Wire.h>
TemperatureSensor tempSensor;
#define M_FRAMES            10
#define A_THRESHOLD         0.1f
#define OMEGA_THRESHOLD     5.0f


#define RX_PIN 2
#define TX_PIN 3
#define RATE 9600

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(RX_PIN,TX_PIN);


float aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int bufIndex = 0;
IMU_UpdateResult r;
#include "HeartSensor.h"
bool danger[3]={0,0,0};
HeartSensor heart;


long last=0;
int iter=0;

void setup() {

BTSerial.begin(RATE);
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


if (!heart.begin()) {
        Serial.println("MAX30105 연결 실패"); while(1);
    }

    Serial.println("모든 센서 초기화 완료");
Wire.setClock(100000);
}

void loop() {
r = IMU_update();
    if(r.hasData) {
        aBuf[bufIndex] = r.aMag;
        omegaBuf[bufIndex] = r.omegaRate;
        bufIndex = (bufIndex + 1) % M_FRAMES;
        // Serial.print("aMag \t");
        // Serial.println(r.aMag);
        // Serial.print("omega \t");
        // Serial.println(r.omegaRate);
    }
    else {
      // Serial.println("imu nodata");
    }
  tempSensor.update();  // Update the temperature.
  float temp = tempSensor.getTemperature();
  // Serial.print("temp \t");
  // Serial.println(temp);

  heart.update();
if(aBuf[M_FRAMES - 1]) {
      if(IMU_isNoMotion(aBuf,omegaBuf)){
iter++;
      }else{
iter=0;
      }
    }
    danger[2]=(iter>100);
    danger[0]=!tempSensor.isTempOkay();
  danger[1]=heart.isDanger();

  if(millis()-last>1000){
    BTSerial.write(0xAA);
  for (uint8_t i = 0; i < 3; i++) {
    BTSerial.write(danger[i]);
    Serial.print(danger[i]);
  }
  BTSerial.write(0x55);
  Serial.println();

  last=millis();
  }
}