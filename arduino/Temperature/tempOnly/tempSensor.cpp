#include "tempSensor.h"
#include <Wire.h>

TemperatureSensor::TemperatureSensor()
  : mlx(), currentState(0), tempIndex(0), tempCount(0), bodyTemp(0.0f) {
  for (int i = 0; i < 5; ++i) tempHistory[i] = 0.0f;
}

bool TemperatureSensor::begin() {
  return mlx.begin();
}

void TemperatureSensor::update() {
  float newTemp = mlx.readObjectTempC();

  // 온도 기록 업데이트. 최근 5개의 기록을 저장하도록 설정
  tempHistory[tempIndex] = newTemp;
  tempIndex = (tempIndex + 1) % 5;
  
  if (tempCount < 5) tempCount++; //평균을 낼 수 있는지 확인

  if (tempCount < 5) {
    bodyTemp = newTemp;
    currentState = 0;  // 5개의 값이 모이기 전까지는 항상 정상 상태로 간주
  } else {
    bodyTemp = getAverageTemp();
    currentState = updateTemperatureState(bodyTemp);
  }
}

float TemperatureSensor::getTemperature() const {
  return bodyTemp;
}

int TemperatureSensor::getStatus() const {
  return currentState;
}

bool TemperatureSensor::isTempOkay() const {
  return currentState == 0;
}

float TemperatureSensor::getAverageTemp() const {
  float sum = 0.0f;
  for (int i = 0; i < tempCount; i++) {
    sum += tempHistory[i];
  }
  return sum / tempCount;
}

int TemperatureSensor::updateTemperatureState(float temp) {
  switch (currentState) {
    case 0:
      if (temp >= 37.5) currentState = 1;
      break;
    case 1:
      if (temp >= 37.0) currentState = 2;
      else if (temp < 37.0) currentState = 0;
      break;
    case 2:
      if (temp < 37.0) currentState = 0;
      break;
  }
  return currentState;
}
