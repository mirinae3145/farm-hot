#include <Wire.h>
#include "tempSensor.h"

TemperatureSensor tempSensor;

void setup() {
  Serial.begin(9600);

  if (!tempSensor.begin()) {
    Serial.println("Temprature sensor initialization failed. Check the connections.");
    while (1);
  }

  Serial.println("Temperature sensor initialized successfully.");
}

void loop() {
  tempSensor.update();  // Update the temperature.

  float temp = tempSensor.getTemperature();
  int state = tempSensor.getStatus();
  bool isOkay = tempSensor.isTempOkay();

  Serial.print("Current temperature: ");
  Serial.print(temp);
  Serial.print(" °C | Internal state: ");
  Serial.print(state);
  Serial.print(" | Status: ");
  Serial.println(isOkay ? "Normal" : "Abnormal");

  delay(100);  // update the temperature every 100ms
}