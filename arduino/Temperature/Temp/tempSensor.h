#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Adafruit_MLX90614.h>

// 혹시나 내부 값 건드릴까봐 private, public으로 나눔.
class TemperatureSensor {
public:
  TemperatureSensor();
  bool begin();
  void update();
  float getTemperature() const;
  int getStatus() const;
  bool isTempOkay() const;

private:
  Adafruit_MLX90614 mlx;
  int currentState;
  float tempHistory[5];
  int tempIndex;
  int tempCount;
  float bodyTemp;
  float getAverageTemp() const;
  int updateTemperatureState(float avgTemp);
};

#endif