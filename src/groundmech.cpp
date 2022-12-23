#include "groundmech.h"

StaticJsonDocument<3072> doc;
/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial);
/* SBUS object, writing SBUS */
// bfs::SbusTx sbus_tx(&Serial);
/* SBUS data */
bfs::SbusData sbusdata;

Groundmech::Groundmech(char* config)
{
    configstring = config;
    init();
}

Groundmech::Groundmech(void)
{
    init();
}

void Groundmech::init(void)
{
    // Mech State: 0x00 Boot, 0x01 Sleep, 0x10 Default, 0x11 Happy, 0x1F Sad
    mechState = 0x00;
}

void Groundmech::begin(void)
{
    deserializeJson(doc,configstring);
    pinMode(PPM_PIN,INPUT_PULLUP);

    const char* name = doc["name"]; // "B2EMO"

    for (int i=0; i<24; i++) { servoPos[i] = 0; }

    for (JsonPair Servo : doc["Servos"].as<JsonObject>())
    {
        //const char* Servo_key = Servo.key().c_str();
        int Servo_value_pin = Servo.value()["pin"];
        if (Servo_value_pin > 0)
        {
            servoMin[Servo_value_pin] = Servo.value()["min"];
            servoMax[Servo_value_pin] = Servo.value()["max"];
            servoPos[Servo_value_pin] = Servo.value()["default"];
            servoPosNext[Servo_value_pin] = Servo.value()["default"];
            servoPosPrev[Servo_value_pin] = Servo.value()["default"];
            servoPosSpeed[Servo_value_pin] = Servo.value()["speed"];

            int easingID = 0x00;
            const char* easingType = Servo.value()["easing"];
            if(easingType=="easeInSine") { easingID = 0x01; }
            else if(easingType=="easeOutSine") { easingID = 0x02; }
            else if(easingType=="easeInOutSine") { easingID = 0x03; }
            else if(easingType=="easeInQuad") { easingID = 0x04; }
            else if(easingType=="easeOutQuad") { easingID = 0x05; }
            else if(easingType=="easeInOutQuad") { easingID = 0x06; }
            else if(easingType=="easeInCubic") { easingID = 0x07; }
            else if(easingType=="easeOutCubic") { easingID = 0x08; }
            else if(easingType=="easeInOutCubic") { easingID = 0x09; }
            else if(easingType=="easeInQuart") { easingID = 0x0A; }
            else if(easingType=="easeOutQuart") { easingID = 0x0B; }
            else if(easingType=="easeInOutQuart") { easingID = 0x0C; }
            else if(easingType=="easeInQuint") { easingID = 0x0D; }
            else if(easingType=="easeOutQuint") { easingID = 0x0E; }
            else if(easingType=="easeInOutQuint") { easingID = 0x0F; }
            else if(easingType=="easeInExpo") { easingID = 0x10; }
            else if(easingType=="easeOutExpo") { easingID = 0x11; }
            else if(easingType=="easeInOutExpo") { easingID = 0x12; }
            else if(easingType=="easeInCirc") { easingID = 0x13; }
            else if(easingType=="easeOutCirc") { easingID = 0x14; }
            else if(easingType=="easeInOutCirc") { easingID = 0x15; }
            else if(easingType=="easeInBounce") { easingID = 0x16; }
            else if(easingType=="easeOutBounce") { easingID = 0x17; }
            else if(easingType=="easeInOutBounce") { easingID = 0x18; }
        }
    }

    /* for (JsonPair Servo : doc["InputChannels"].as<JsonObject>())
    {
        const char* Servo_key = Servo.key().c_str();
        int Servo_value_pin = Servo.value();
    } */
}

unsigned long lastIdleMovement = millis();
void Groundmech::loop(void)
{
    mechStateN = constrain(map(ppmch[5],600,1600,0,3),0,2);

    if (Serial.available())
    {
        char ch = Serial.read();
        receiveData(ch);
    }

    if (mechState != mechStateN)
    {
        Serial.print("State Update: "); Serial.println(mechStateN);
        mechState = mechStateN;
        updateState(mechStateN);
    }

    if (millis() > lastIdleMovement && mechState == 1)
    {
        lastIdleMovement = millis() + 8000;
        setServoPos(1,random(1000,2000));
        setServoPos(2,random(1200,1800));
        setServoPos(3,random(1200,1800));
        setServoPos(4,random(1500,2500));
        setServoPos(5,random(1500,2500));
        setServoPos(6,random(1700,2500));
    }

    updateDriveMotors(ppmch[0],ppmch[1],ppmch[3]);
}

int Groundmech::switchChanged(int switchID)
{
    return 0;
}

/*
    States
*/
void Groundmech::updateState(int state)
{
    // Mech State: 0x00 Boot, 0x01 Sleep, 0x02 Drive, 0x03 Stationary, 0x04 Idle
    if (state == 0x02)
    {
        int Drivemode = true;
        Serial.println("Drive Mode Engaged");
        setServoPos(1,1500);
        setServoPos(2,1500);
        setServoPos(3,1500);
        setServoPos(4,1500);
        setServoPos(5,1000);
        setServoPos(6,1000);
    } else {
        Serial.println("Drive Mode Disabled");
        setServoPos(1,1500);
        setServoPos(2,1500);
        setServoPos(3,1500);
        setServoPos(4,2000);
        setServoPos(5,2000);
        setServoPos(6,2000);
    }
}

void Groundmech::updateDriveMotors(int x, int y, int z)
{
    if (mechState != 0x02)
    {
        setServoPos(10,1500);
        setServoPos(12,1500);
        setServoPos(14,1500);
        setServoPos(16,1500);
        return;
    }

    int inputX = constrain(map(x,600,1600,-900,900),-900,900);
    int inputY = constrain(map(y,600,1600,-900,900),-900,900);
    int inputZ = constrain(map(z,600,1600,-900,900),-900,900);

    int motorFL = map(servoPos[10], getServoMin(10), getServoMax(10), -900, 900);
    int motorFR = map(servoPos[12], getServoMin(12), getServoMax(12), -900, 900);
    int motorRR = map(servoPos[14], getServoMin(14), getServoMax(14), -900, 900);
    int motorRL = map(servoPos[16], getServoMin(16), getServoMax(16), -900, 900);

    motorFL = constrain(motorFL + (inputX - motorFL),-900,900);
    motorFR = constrain(motorFR + (inputX - motorFR),-900,900);
    motorRL = constrain(motorRL + (inputX - motorRL),-900,900);
    motorRR = constrain(motorRR + (inputX - motorRR),-900,900);

    motorFL = constrain(motorFL + inputY,-900,900);
    motorFR = constrain(motorFR - inputY,-900,900);
    motorRL = constrain(motorRL - inputY,-900,900);
    motorRR = constrain(motorRR + inputY,-900,900);

    motorFL = constrain(motorFL + inputZ,-900,900);
    motorFR = constrain(motorFR - inputZ,-900,900);
    motorRL = constrain(motorRL + inputZ,-900,900);
    motorRR = constrain(motorRR - inputZ,-900,900);

    servoPos[10] = constrain(map(motorFL,-900,900,getServoMin(10),getServoMax(10)),getServoMin(10),getServoMax(10));
    servoPosNext[10] = servoPos[10];
    servoPosPrev[10] = servoPos[10];
    servoPos[12] = constrain(map(motorFR,-900,900,getServoMin(12),getServoMax(12)),getServoMin(12),getServoMax(12));
    servoPosNext[12] = servoPos[12];
    servoPosPrev[12] = servoPos[12];
    servoPos[14] = constrain(map(motorRR,-900,900,getServoMin(14),getServoMax(14)),getServoMin(14),getServoMax(14));
    servoPosNext[14] = servoPos[14];
    servoPosPrev[14] = servoPos[14];
    servoPos[16] = constrain(map(motorRL,-900,900,getServoMin(16),getServoMax(16)),getServoMin(16),getServoMax(16));
    servoPosNext[16] = servoPos[16];
    servoPosPrev[16] = servoPos[16];

    // setServoPos(10,constrain(map(motorFL,-900,900,500,2500),500,2500));
    // setServoPos(12,constrain(map(motorFR,-900,900,500,2500),500,2500));
    // setServoPos(14,constrain(map(motorRR,-900,900,500,2500),500,2500));
    // setServoPos(16,constrain(map(motorRL,-900,900,500,2500),500,2500));

    // Serial.print("motorFL:");
    // Serial.print(servoPos[10]);
    // Serial.print("\t motorFR:");
    // Serial.print(servoPos[12]);
    // Serial.print("\t  motorRR:");
    // Serial.print(servoPos[14]);
    // Serial.print("\t  motorRL:");
    // Serial.print(servoPos[16]);
    // Serial.println(";");
}

/*
    Servos
*/

int Groundmech::getServoMin(int servoID) {
    return min(servoMin[servoID],servoMax[servoID]);
}

int Groundmech::getServoMax(int servoID) {
    return max(servoMin[servoID],servoMax[servoID]);
}

int Groundmech::isServoReversed(int servoID) {
    return (servoMin[servoID] > servoMax[servoID]);
}

void Groundmech::setServoPos(int servoID,int nextPos)
{
    int sMin = getServoMin(servoID);
    int sMax = getServoMax(servoID);
    servoPosNext[servoID] = constrain(nextPos,sMin,sMax);
    servoPosPrev[servoID] = constrain(servoPos[servoID],sMin,sMax);
}

int Groundmech::getServoPos(int servoID)
{
    int sPos = servoPos[servoID];
    int sPosNext = servoPosNext[servoID];
    int sPosPrev = servoPosPrev[servoID];
    int sMin = getServoMin(servoID);
    int sMax = getServoMax(servoID);

    if (sPos == 0)
    {
        return 1500;
    }

    if (sPos != sPosNext)
    {
        float progress = (float)sPos/(float)sPosNext;
        if (progress >= 1) { progress = 1 - progress; };
        progress = (float)(sPosNext - sPosPrev) * easeInOutSine(progress/servoPosSpeed[servoID]);
        if (progress <1 && progress > 0) {progress = 1;}
        if (progress <0 && progress > -1) {progress = -1;}

        if (sPosNext > sPosPrev)
            sPos = constrain(sPos + progress,sPosPrev,sPosNext);
        else if (sPosPrev > sPosNext)
            sPos = constrain(sPos + progress,sPosNext,sPosPrev);

        //Serial.print(servoID); Serial.print("sPosNext:"); Serial.print(sPosNext); Serial.print("\t");
        //Serial.print(servoID); Serial.print("sPosPrev:"); Serial.print(sPosPrev); Serial.print("\t");
        //Serial.print(servoID); Serial.print("To:"); Serial.print(sPos); Serial.println("\t");
    }

    servoPos[servoID] = constrain(sPos,sMin,sMax);
    if (isServoReversed(servoID)) { sPos = (sMin + sMax) - sPos; }

    return constrain(sPos,sMin,sMax);
}

/*
    Communications
*/
void Groundmech::receiveData(char ch)
{
    if (ch == '<')
        return;

    if (ch == '>' || ch == '\r' || ch == '\n' || ch == 0) {
        cmdBuffer[cmdPos] = '\0';
        cmdPos = 0;
        if (*cmdBuffer != '\0') {
            processCommands(cmdBuffer);
        }
    }
    else if (cmdPos < (int)sizeof(cmdBuffer)) {
        cmdBuffer[cmdPos++] = ch;
    }

    if (cmdPos == (int)sizeof(cmdBuffer)) {
        cmdBuffer[cmdPos-1] = '\0';
        cmdPos = 0;
        if (*cmdBuffer != '\0') {
            processCommands(cmdBuffer);
        }
    }
}

void Groundmech::processCommands(char* input)
{
    Serial.println(input);
    String response = ToString(input);
    String qC = getValue(response,',',0);

    if (qC.equals((String)"S"))
    {
        mechStateN = getValue(response,',',1).toInt();
    }
}

String Groundmech::getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



/*
    Control Inputs Mapping
*/

int Groundmech::remoteConnected(void) {
  return remoteData[0];
}

int Groundmech::ButtonPressed(int buttonID) {
  if (!remoteConnected())
    return 0;
  return remoteData[buttonID];
}

int Groundmech::ButtonDoublePressed(int buttonID) {
  if (!remoteConnected())
    return 0;
  return remoteData[buttonID];
}

int Groundmech::ButtonHeld(int buttonID) {
  if (!remoteConnected())
    return 0;
  return remoteData[buttonID];
}

float Groundmech::JoystickX(int joystickID) {
  if (!remoteConnected())
    return (float)0.00;
  int jX = remoteData[JOYSTICK_L+1];
  if (joystickID == JOYSTICK_R) {
    jX = remoteData[JOYSTICK_R+1];
  };
  return (float)jX;
};

float Groundmech::JoystickY(int joystickID) {
  if (!remoteConnected())
    return (float)0.00;
  int jY = remoteData[JOYSTICK_L+2];
  if (joystickID == JOYSTICK_R) {
    jY = remoteData[JOYSTICK_R+2];
  };
  return (float)jY;
};

float Groundmech::JoystickDirection(int joystickID) {
  if (!remoteConnected())
    return (float)0.00;
  if (joystickID == JOYSTICK_L) {
    int jX = remoteData[JOYSTICK_L+1];
    int jY = remoteData[JOYSTICK_L+2];
    return (float)abs(180 - atan2(jX,jY)*(180/3.14159));
  };
  if (joystickID == JOYSTICK_R) {
    int jX = remoteData[JOYSTICK_R+1];
    int jY = remoteData[JOYSTICK_R+2];
    return (float)abs(180 - atan2(jX,jY)*(180/3.14159));
  };
  return (float)0.00;
}

float Groundmech::JoystickStrength(int joystickID) {
  if (!remoteConnected())
    return (float)0.00;
  if (joystickID == JOYSTICK_L) {
    int jX = remoteData[JOYSTICK_L+1];
    int jY = remoteData[JOYSTICK_L+2];
    return (float)hypot(jX,jY);
  };
  if (joystickID == JOYSTICK_R) {
    int jX = remoteData[JOYSTICK_R+1];
    int jY = remoteData[JOYSTICK_R+2];
    return (float)hypot(jX,jY);
  };
  return (float)0.00;
}


