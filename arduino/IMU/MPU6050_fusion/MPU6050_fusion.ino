#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu(0x68);

#define USE_PRESET_OFFSETS

#ifdef USE_PRESET_OFFSETS
  #define XG_OFFSET 204
  #define YG_OFFSET 1456
  #define ZG_OFFSET 1270
  #define XA_OFFSET 53
  #define YA_OFFSET -31
  #define ZA_OFFSET 50
#endif

// --- 위험 판단 파라미터 ---
#define M_FRAMES            10     // 평균을 낼 샘플 수
#define SUSTAIN_SECONDS     5      // 임계치 지속 시간 (초)

#define THETA_THRESHOLD     5.0f   // 회전 각도 임계치 (°)
#define A_THRESHOLD         0.15f   // 선형 가속도 임계치 (g)
#define OMEGA_THRESHOLD     5.0f   // 자이로 속도 임계치 (°/s)

#define LOOP_INTERVAL_MS    100     // 루프 주기 (ms)


int const INTERRUPT_PIN = 2;  // Define the interruption #0 pin

/*---MPU6050 Control/Status Variables---*/
bool DMPReady = false;  // Set true if DMP init was successful
uint8_t MPUIntStatus;   // Holds actual interrupt status byte from MPU
uint8_t devStatus;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // Expected DMP packet size (default is 42 bytes)
uint8_t FIFOBuffer[64]; // FIFO storage buffer

// --- 쿼터니언·센서 변수 ---
Quaternion qCur, qPrev, qRel;
VectorInt16 aaRaw, aaReal, gyroRaw;
VectorFloat gravity;

// --- 이동평균용 원형 버퍼 ---
float thetaBuf[M_FRAMES] = {0}, aBuf[M_FRAMES] = {0}, omegaBuf[M_FRAMES] = {0};
int   bufIndex = 0, bufCount = 0;
float sumTheta = 0, sumA = 0, sumOmega = 0;

// --- 지속시간 측정 ---
unsigned long motionStartTime = 0;

/*------Interrupt detection routine------*/
volatile bool MPUInterrupt = false;     // Indicates whether MPU6050 interrupt pin has gone high
void DMPDataReady() {
  MPUInterrupt = true;
}

void setup() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment on this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  Serial.begin(115200); //115200 is required for Teapot Demo output
  while (!Serial);

  /*Initialize device*/
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  /*Verify connection*/
  Serial.println(F("Testing MPU6050 connection..."));
  if(mpu.testConnection() == false){
    Serial.println("MPU6050 connection failed");
    while(true);
  }
  else {
    Serial.println("MPU6050 connection successful");
  }

  /*Wait for Serial input*/
  Serial.println(F("\nSend any character to begin: "));
  while (Serial.available() && Serial.read()); // Empty buffer
  while (!Serial.available());                 // Wait for data
  while (Serial.available() && Serial.read()); // Empty buffer again

  /* Initializate and configure the DMP*/
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  #ifdef USE_PRESET_OFFSETS
    mpu.setXGyroOffset(XG_OFFSET);
    mpu.setYGyroOffset(YG_OFFSET);
    mpu.setZGyroOffset(ZG_OFFSET);
    mpu.setXAccelOffset(XA_OFFSET);
    mpu.setYAccelOffset(YA_OFFSET);
    mpu.setZAccelOffset(ZA_OFFSET);
  #else
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
  #endif

  Serial.println("These are the Active offsets: ");
  mpu.PrintActiveOffsets();
  delay(1000);

  Serial.println(F("Enabling DMP..."));   //Turning ON DMP
  mpu.setDMPEnabled(true);

  /*Enable Arduino interrupt detection*/
  Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
  Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
  Serial.println(F(")..."));
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), DMPDataReady, RISING);
  MPUIntStatus = mpu.getIntStatus();

  /* Set the DMP Ready flag so the main loop() function knows it is okay to use it */
  Serial.println(F("DMP ready! Waiting for first interrupt..."));
  DMPReady = true;

  qPrev.w = 1; qPrev.x = qPrev.y = qPrev.z = 0;  // 초기 기준자세
}

void loop() {
  if (!DMPReady) return; // Stop the program if DMP programming fails.
    
  /* Read a packet from FIFO */
  if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) { // Get the Latest packet 
    // #ifdef OUTPUT_READABLE_QUATERNION
      /* Display Quaternion values in easy matrix form: [w, x, y, z] */
      mpu.dmpGetQuaternion(&qCur, FIFOBuffer);
      Serial.print("quat\t");
      Serial.print(qCur.w);
      Serial.print("\t");
      Serial.print(qCur.x);
      Serial.print("\t");
      Serial.print(qCur.y);
      Serial.print("\t");
      Serial.println(qCur.z);

      mpu.dmpGetAccel(&aaRaw,FIFOBuffer);
      mpu.dmpGetGravity(&gravity,&qCur);
      mpu.dmpGetLinearAccel(&aaReal,&aaRaw,&gravity);

      Serial.print("aareal\t");
      Serial.print(aaReal.x);
      Serial.print("\t");
      Serial.print(aaReal.y);
      Serial.print("\t");
      Serial.println(aaReal.z);

      mpu.dmpGetGyro(&gyroRaw,FIFOBuffer);
      qRel.w =  qPrev.w * qCur.w + qPrev.x * qCur.x + qPrev.y * qCur.y + qPrev.z * qCur.z;
      float thetaRel = 2 * acos(constrain(qRel.w, -1.0f, 1.0f)) * 180.0f / PI;
      Serial.print("qRelW\t");
      Serial.println(qRel.w);

      float ax = aaReal.x / 16384.0f;
      float ay = aaReal.y / 16384.0f;
      float az = aaReal.z / 16384.0f;
      float aMag = sqrt(ax*ax + ay*ay + az*az);
      Serial.print("aMag\t");
      Serial.println(aMag);

      float gx = gyroRaw.x / 131.0f;
      float gy = gyroRaw.y / 131.0f;
      float gz = gyroRaw.z / 131.0f;
      float omegaRate = sqrt(gx*gx + gy*gy + gz*gz);
      Serial.print("wRate\t");
      Serial.println(omegaRate);

      sumTheta  = sumTheta  - thetaBuf[bufIndex]  + thetaRel;
      sumA      = sumA      - aBuf[bufIndex]      + aMag;
      sumOmega  = sumOmega  - omegaBuf[bufIndex]  + omegaRate;
      thetaBuf[bufIndex] = thetaRel;
      aBuf[bufIndex]     = aMag;
      omegaBuf[bufIndex] = omegaRate;
      if (bufCount < M_FRAMES) bufCount++;
      bufIndex = (bufIndex + 1) % M_FRAMES;

      float avgTheta = sumTheta / bufCount;
      float avgA     = sumA     / bufCount;
      float avgOmega = sumOmega / bufCount;

      Serial.print("avgTheta\t");
      Serial.println(avgTheta);
      Serial.print("avgA\t");
      Serial.println(avgA);
      Serial.print("avgOmega\t");
      Serial.println(avgOmega);
      bool noMotion = (avgTheta < THETA_THRESHOLD) && (avgA < A_THRESHOLD) && (avgOmega < OMEGA_THRESHOLD);

      if (noMotion) {
        Serial.println("no motion detected");
        if (motionStartTime == 0) motionStartTime = millis();
        if (millis() - motionStartTime >= SUSTAIN_SECONDS * 1000UL) {
          Serial.println("Emergency");
        }
      } else {
        motionStartTime = 0;
      }
    // #endif

    qPrev=qCur;
    delay(LOOP_INTERVAL_MS);
  }

}
