#include <groundmech.h>

char config[] = "{"
  "\"name\":\"B2EMO\","
  "\"controller\":\"PPM\","
  "\"Servos\":{"
    "\"HeadY\":{\"pin\":1,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"HeadX1\":{\"pin\":2,\"min\":1800,\"max\":1200,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"HeadX2\":{\"pin\":3,\"min\":1200,\"max\":1800,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"HeadZ\":{\"pin\":4,\"min\":2500,\"max\":800,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"BodyLower\":{\"pin\":5,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"BodyUpper\":{\"pin\":6,\"min\":2500,\"max\":500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"BodyRotateLower\":{\"pin\":7,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
    "\"BodyRotateUpper\":{\"pin\":8,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"DriveExtension\":{\"pin\":9,\"min\":1000,\"max\":2000,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"DriveA\":{\"pin\":10,\"min\":1200,\"max\":1700,\"centre\":1500,\"default\":1500,\"speed\":30,\"easing\":\"none\"},"
    "\"DriveALift\":{\"pin\":11,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"DriveB\":{\"pin\":12,\"min\":1200,\"max\":1700,\"centre\":1500,\"default\":1500,\"speed\":30,\"easing\":\"none\"},"
    "\"DriveBLift\":{\"pin\":13,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"DriveC\":{\"pin\":14,\"min\":1200,\"max\":1700,\"centre\":1500,\"default\":1500,\"speed\":30,\"easing\":\"none\"},"
    "\"DriveCLift\":{\"pin\":15,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"

    "\"DriveD\":{\"pin\":16,\"min\":1200,\"max\":1700,\"centre\":1500,\"default\":1500,\"speed\":30,\"easing\":\"none\"},"
    "\"DriveDLift\":{\"pin\":17,\"min\":500,\"max\":2500,\"centre\":1500,\"default\":1500,\"speed\":10,\"easing\":\"none\"},"
  "}"
"}";

Groundmech B2EMO(config);
PioServo pioServo(pio1);

void UpdateServoBlock0(void *_this)
{
  int blockID = 0;
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    for (int i = 0; i < 8; i++)
    {
      pioServo.setServoMicros(blockID, i, B2EMO.getServoPos(blockID * 8 + i));
    }
  }
}

void UpdateServoBlock1(void *_this)
{
  int blockID = 1;
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    for (int i = 0; i < 8; i++)
    {
      int servoPos = B2EMO.getServoPos(blockID * 8 + i);
      // Serial.print(blockID * 8 + i);
      // Serial.print("=");
      // Serial.print(servoPos);
      // Serial.print("\t");
      pioServo.setServoMicros(blockID, i, servoPos);
    }
    //Serial.println(";");
  }
}

void UpdateServoBlock2(void *_this)
{
  int blockID = 2;
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    for (int i = 0; i < 8; i++)
    {
      pioServo.setServoMicros(blockID, i, B2EMO.getServoPos(blockID * 8 + i));
    }
  }
}

void setup() {
  Serial.begin(115200);
  B2EMO.begin();
  delay(5000);

  TaskHandle_t Handle1;
  xTaskCreate(UpdateServoBlock0, "ServoUpdate1", 1024, NULL, 1, &Handle1);
  auto blck = pioServo.addServoBlock(0, 8, Handle1);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }

  TaskHandle_t Handle2;
  xTaskCreate(UpdateServoBlock1, "ServoUpdate2", 1024, NULL, 1, &Handle2);
  blck = pioServo.addServoBlock(8, 8, Handle2);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }

  TaskHandle_t Handle3;
  xTaskCreate(UpdateServoBlock2, "ServoUpdate3", 1024, NULL, 1, &Handle3);
  blck = pioServo.addServoBlock(16, 8, Handle3);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }
}

void loop() {
  B2EMO.loop();
  delay(10);
}

void setup1()
{
  //InitSbus();
}

void loop1() {
  B2EMO.PPMupdate();
}