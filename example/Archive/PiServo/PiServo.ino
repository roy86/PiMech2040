#include <Arduino.h>
#include "hardware/dma.h"
#include <FreeRTOS.h>
#include <task.h>
#include "pio_servo.hpp"

PioServo pioServo(pio1);

unsigned long getPos(int min,int max,int div)
{
  unsigned long pos = map(millis()%div,0,div,min,max*2);
  if (pos > max) { pos = max*2-pos; };
  return constrain(pos,min,max);
}

void UpdateServoBlock0(void *_this)
{
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    unsigned long pos = getPos(1500,1500,10000);
    for (int i=0;i<8;i++){ pioServo.setServoMicros(0,i,pos); }
    pioServo.setServoMicros(0,4,3000 - getPos(500,2200,10000));
    pioServo.setServoMicros(0,5,getPos(500,2500,8000));
    pioServo.setServoMicros(0,6,3000 - getPos(500,2500,12000));
    //Serial.println(pos);
  }
}

void UpdateServoBlock1(void *_this)
{
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    unsigned long pos = getPos(500,2500,10000);
    for (int i=0;i<8;i++){ pioServo.setServoMicros(1,i,pos); }
    //Serial.println(pos);
  }
}

void UpdateServoBlock2(void *_this)
{
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    unsigned long pos = getPos(500,2500,10000);
    for (int i=0;i<8;i++){ pioServo.setServoMicros(2,i,pos); }
    //Serial.println(pos);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10000);

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

  blck = pioServo.addServoBlock(9, 8, Handle2);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }

  TaskHandle_t Handle3;
  xTaskCreate(UpdateServoBlock2, "ServoUpdate3", 1024, NULL, 1, &Handle3);

  blck = pioServo.addServoBlock(17, 8, Handle3);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }
}

void loop()
{
  delay(1);
}

