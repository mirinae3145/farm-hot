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

#if defined(ESP32)

  #include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE UUID 정의 (원하는 대로 바꾸어도 됨)
#define SERVICE_UUID        "7987D1FC-3DB6-4CFE-980F-AA458860918E"
#define CHARACTERISTIC_UUID "7987D1FC-3DB6-4CFE-980F-AA458860918E"
BLECharacteristic *pCharacteristic;
#else
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(RX_PIN,TX_PIN);
#endif


float aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int bufIndex = 0;
IMU_UpdateResult r;
#include "HeartSensor.h"
uint8_t danger[3]={0,0,0};
HeartSensor heart;


long last=0;
int iter=0;

void setup() {



Serial.begin(38400);
while(!Serial);
Serial.println("serial success");


#if defined(ESP32)

 BLEDevice::init("FarmDevice");

  // 서버 생성
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // notify 가능 characteristic 생성
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );

  // descriptor 추가 (notify 수신 허용 설정)
  pCharacteristic->addDescriptor(new BLE2902());

  // 서비스 시작
  pService->start();

  // 광고 시작
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE notify server ready");

#else
BTSerial.begin(RATE);
#endif

Wire.begin(6,7);
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

    Serial.println("HB success");
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
    danger[2]=(iter>100)?0:1;
    danger[0]=tempSensor.isTempOkay()?1:0;
  danger[1]=heart.isDanger()?0:1;

  if(millis()-last>1000){
    #ifndef ESP32
    BTSerial.write(0xAA);
    #endif
  for (uint8_t i = 0; i < 3; i++) {

    #if defined(ESP32)
     // 클라이언트가 구독 중인 경우에만 전송됨
     //if(pCharacteristic->getSubscribedCount()){
  pCharacteristic->setValue(danger, sizeof(danger));
  pCharacteristic->notify();
    #else
    BTSerial.write(danger[i]);
    #endif
    
    Serial.print(danger[i]);
  }
  //BTSerial.write(0x55);
  Serial.println();

  last=millis();
  }



}