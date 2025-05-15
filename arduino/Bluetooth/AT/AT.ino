#define RX_PIN 2
#define TX_PIN 3

// #define RATE_05 38400
// #define RATE_06 9600
#define RATE_CUSTOM

#include <SoftwareSerial.h>

String inputString = "";
bool inputReady = false;

void sHandle(){
  while(Serial.available()) {
      char c=Serial.read();
      if((c=='\n' || c=='\r')&&inputString.length()>0){
        inputReady=true;
        return;
      }
      else{
        inputString+=c;
      }
    }
}


SoftwareSerial BTSerial(RX_PIN, TX_PIN);
void setup() {
  int baudRate;
  Serial.begin(9600);

  #ifdef RATE_05
    baudRate = RATE_05;
  #elif defined(RATE_06)
    baudRate = RATE_06;
  #else
    Serial.println("input BT baudrate");
    while(!inputReady) sHandle();
    baudRate = inputString.toInt();
  #endif

  BTSerial.begin(baudRate);
  Serial.print("AT Command & Serial Test Mode w/ ");
  Serial.println(baudRate);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }

  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.write(c);
  }
}
