#define RX_PIN 2
#define TX_PIN 3
#define RATE 9600

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(RX_PIN,TX_PIN);

void setup() {
  BTSerial.begin(RATE);
}

void loop() {
  uint8_t dataToSend[3] = {0, 0, 1};

  BTSerial.write(0xAA);
  for (uint8_t i = 0; i < 3; i++) {
    BTSerial.write(dataToSend[i]);
  }
  BTSerial.write(0x55);

  delay(1000);
}
