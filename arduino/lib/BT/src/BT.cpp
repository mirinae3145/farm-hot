#include "BT.h"

#if defined(ESP32)
    static BLECharacteristic *pCharacteristic = nullptr;
#else
    static SoftwareSerial BTSerial(RX_PIN, TX_PIN);
#endif

bool BT_begin() {
    #if defined(ESP32)
        BLEDevice::init("FarmDevice");
        BLEServer *pServer = BLEDevice::createServer();
        if (!pServer)
            return false;
        BLEService *pService = pServer->createService(SERVICE_UUID);
        if (!pService)
            return false;

        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_NOTIFY
        );
        if (!pCharacteristic)
            return false;

        pCharacteristic->addDescriptor(new BLE2902());
        pService->start();
        
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        if (!pAdvertising)
            return false;
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->start();
    #else
        BTSerial.begin(RATE);
        while (!BTSerial);
    #endif
    return true;
}

void BT_sendDanger(uint8_t danger[3]) {
    #if defined(ESP32)
        pCharacteristic->setValue(danger, 3);
        pCharacteristic->notify();
    #else
        BTSerial.write(0xAA); // Start byte
        BTSerial.write(danger, 3);
        BTSerial.write(0x55); // End byte
    #endif
    return;
}