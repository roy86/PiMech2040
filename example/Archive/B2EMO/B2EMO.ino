/*
{
  "Name":"B2EMO",

}
*/

#define SMOOTHING 0.08

#include <groundmech.h>

char ConfigFile[] = "{\
  \"Name\":\"B2EMO\",\
  \"DriveType\":\"Mecanum-4\",\
  \"InputProtocol\":\"SBUS,I2C\",\
  \"PinOut1\":[500,500,2500],\
  \"PinOut2\":[500,500,2500],\
  \"PinOut3\":[500,500,2500],\
  \"PinOut4\":[500,500,2500],\
  \"PinOut5\":[500,500,2500],\
  \"PinOut6\":[500,500,2500],\
  \"PinOut7\":[500,500,2500],\
  \"PinOut8\":[500,500,2500],\
  \"PinOut9\":[500,500,2500],\
  \"PinOut10\":[500,500,2500],\
  \"PinOut11\":[500,500,2500],\
  \"PinOut12\":[500,500,2500],\
  \"PinOut13\":[500,500,2500],\
  \"PinOut14\":[500,500,2500],\
  \"PinOut15\":[500,500,2500],\
  \"PinOut16\":[500,500,2500],\
  \"PinOut17\":[500,500,2500],\
  \"PinOut18\":[500,500,2500],\
}";

StaticJsonDocument<200> DroidConfig;

Groundmech B2EMO;
PioServo pioServo(pio1);

void UpdateServoBlock0(void *_this)
{
  int posMin = 500;
  int posMax = 2500;
  int mid = posMin + (posMax-posMin)/2;

  int NeckLPos = mid;
  int NeckRPos = mid;
  int TurnPos  = mid;

  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    unsigned long pos = 3000 - constrain(map(B2EMO.ppmch[0],600,1600,500,2500),500,2100);

    int inputX = constrain(map(B2EMO.ppmch[2],600,1600,posMin,posMax),posMin,posMax);
    int inputY = constrain(map(B2EMO.ppmch[3],600,1600,posMin,posMax),posMin,posMax);
    int inputZ = constrain(map(B2EMO.ppmch[1],600,1600,posMin,posMax),posMin,posMax);

    NeckLPos = constrain(NeckLPos + (posMax - inputX - NeckLPos) * SMOOTHING, posMin, posMax);
    NeckRPos = constrain(NeckRPos + (inputX - NeckRPos) * SMOOTHING, posMin, posMax);

    NeckLPos = constrain(NeckLPos + (posMax - inputY - NeckLPos) * SMOOTHING, posMin, posMax);
    NeckRPos = constrain(NeckRPos + (posMax - inputY - NeckRPos) * SMOOTHING, posMin, posMax);

    TurnPos  = constrain(TurnPos + (inputZ - TurnPos) * SMOOTHING,posMin,posMax);

    Serial.print("CH pos"); Serial.print(map(B2EMO.ppmch[0],600,1600,500,2500)); Serial.print("\t");
    Serial.print("CH inputX"); Serial.print(inputX); Serial.print("\t");
    Serial.print("CH NeckLPos"); Serial.print(NeckLPos); Serial.print("\t");
    Serial.print("CH inputY"); Serial.print(inputY); Serial.print("\t");
    Serial.print("CH NeckRPos"); Serial.print(NeckRPos); Serial.print("\t");
    Serial.print("CH inputZ"); Serial.print(inputZ); Serial.print("\t");
    Serial.print("CH TurnPos"); Serial.print(TurnPos); Serial.print("\t");
    Serial.println(";");

    pioServo.setServoMicros(0,1,constrain(pos,500,2500));
    pioServo.setServoMicros(0,2,constrain(NeckLPos,posMin,posMax));
    pioServo.setServoMicros(0,3,constrain(NeckRPos,posMin,posMax));
    pioServo.setServoMicros(0,4,constrain(TurnPos,posMin,posMax));
  }
}

void UpdateServoBlock1(void *_this)
{
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    unsigned long pos = map(millis()%10000,0,10000,500,2500);
    //for (int i=0;i<8;i++){ pioServo.setServoMicros(1,i,pos); }
    //Serial.println(pos);
  }
}

void UpdateServoBlock2(void *_this)
{
  int reverse = 0;
  int posMin = 500;
  int posMax = 2500;
  while (true)
  {
    xTaskNotifyWait(0x00, ULONG_MAX, nullptr, portMAX_DELAY);
    int pos = map(B2EMO.ppmch[0],600,1600,posMin,posMax);
    if (reverse) {
      pos = pos - 5;
    } else {
      pos = pos + 5;
    }
    if (pos > posMax) {reverse = 1;}
    if (pos < posMin) {reverse = 0;}

    int pos1 = 3000 - map(B2EMO.ppmch[0],600,1600,500,2500);

    //for (int i=0;i<8;i++){ pioServo.setServoMicros(2,i,constrain(pos,posMin,posMax)); }
    pioServo.setServoMicros(2,0,constrain(pos1,500,2500));
    pioServo.setServoMicros(2,1,constrain(pos,posMin,posMax));
    //Serial.println(pos);
    #ifdef DEBUG_PPM
    for(int i = 0; i < PPM_CHs; i++) //Prints all the values readed
    {
        //Serial.print("CH"); //Channel
        //Serial.print(i); //Channel number
        //Serial.print(":"); Serial.print(B2EMO.ppmch[i]); //Print the value
        //Serial.print("\t"); 
    }
    //Serial.println(";");
    //delay(10); //Give time to print values.
    #endif
  }
}

void setup()
{
  Serial.begin(115200);

  B2EMO.begin();

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

  blck = pioServo.addServoBlock(9,7, Handle2);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }

  TaskHandle_t Handle3;
  xTaskCreate(UpdateServoBlock2, "ServoUpdate3", 1024, NULL, 1, &Handle3);

  blck = pioServo.addServoBlock(16,2, Handle3);
  if (blck < 0) {
    Serial.printf("addServoblock error :%d", blck);
  } else {
    pioServo.runServoBlock(blck);
  }
}

void loop()
{
  B2EMO.updateControlInputs();
}