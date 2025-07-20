#pragma once
#if defined(ESP32)
    #include <BLEDevice.h>
    #include <BLEServer.h>
    #include <BLEUtils.h>
    #include <BLE2902.h>

    #define SERVICE_UUID        "7987D1FC-3DB6-4CFE-980F-AA458860918E"
    #define CHARACTERISTIC_UUID "7987D1FC-3DB6-4CFE-980F-AA458860918E"
#else
    #include <SoftwareSerial.h>
    #define RX_PIN 2
    #define TX_PIN 3
    #define RATE 9600
#endif
bool BT_begin();
void BT_sendDanger(uint8_t danger[3]);