// This code is programmed only for the test session. For the final version, integration with IMU sensor data will be required.

#define USE_ARDUINO_INTERRUPTS true

#include <PulseSensorPlayground.h>

const int PULSE_INPUT = A0;       // PulseSensor 연결 핀
const int PULSE_BLINK = 9;        // 심박 감지 시 LED 핀
const int THRESHOLD = 550;        // 신호 감지 기준값
const int BPM_SAFE_MIN = 50;      // 최소 정상 BPM (이 값보다 낮으면 위기 상황)

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(9600);  // 시리얼 통신 시작

  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.setThreshold(THRESHOLD);

  if (!pulseSensor.begin()) {
    Serial.println("PulseSensor 초기화 실패");
    while (true);  // 무한 대기
  }
}

void loop() {
  int bpm = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    if (bpm < BPM_SAFE_MIN) {
      Serial.println(1);  // 위기 상황
    } else {
      Serial.println(0);  // 정상 상태
    }
  }

  delay(20);
}
