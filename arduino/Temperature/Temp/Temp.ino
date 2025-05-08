#include <Wire.h>
#include <Adafruit_MLX90614.h>

// MLX90614 객체 생성
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int currentState = 0;  // 초기 상태는 정상(0)

void setup() {
  Serial.begin(9600);
  if (!mlx.begin()) {
    Serial.println("MLX90614 초기화 실패. 연결을 확인하세요.");
    while (1);
  }
  Serial.println("MLX90614 체온 센서 시작됨.");
}

int updateTemperatureState(float bodyTemp) {
  switch (currentState) {
    case 0:
      if (bodyTemp >= 37.5) {
        currentState = 1;
      }
      break;
    case 1:
      if (bodyTemp >= 37.0) {
        currentState = 2;
      } else if (bodyTemp < 37.0) {
        currentState = 0;
      }
      break;
    case 2:
      if (bodyTemp < 37.0) {
        currentState = 0;
      }
      break;
  }
  return currentState;
}

void loop() {
  float bodyTemp = mlx.readObjectTempC();  // GY-906 센서에서 체온 측정
  int status = updateTemperatureState(bodyTemp);

  Serial.print("체온: ");
  Serial.print(bodyTemp);
  Serial.print(" °C | 상태: ");
  Serial.println(status);

  delay(1000);
}

