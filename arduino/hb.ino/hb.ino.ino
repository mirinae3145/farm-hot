#include <Wire.h>
#include "HeartSensor.h"


HeartSensor heart;


void setup() {
    Serial.begin(115200);
    Wire.begin();

    if (!heart.begin()) {
        Serial.println("MAX30105 연결 실패"); while(1);
    }

    Serial.println("모든 센서 초기화 완료");
}

void loop() {
    // 각 센서 업데이트
    heart.update();


    // 출력
    heart.print();


    // 심박 이상 경고
    if (heart.isDanger()) {
        Serial.println();
        Serial.println("⚠️ 심박이 위험 범위입니다. 잠시 쉬었다가,");
        Serial.println("   몸에 이상이 있으면 즉시 병원에 방문하세요.");
        Serial.println();
    }

    delay(200); // 적절한 주기
}
