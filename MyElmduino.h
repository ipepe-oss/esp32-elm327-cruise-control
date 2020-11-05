#include "TimedAction.h"
#include "BluetoothSerial.h"
#include "ELMduino.h"

// OBDII bluetooth mac 24:42:16:08:00:00
BluetoothSerial SerialBT;
ELM327 myELM327;

// OBD Parameters
uint32_t obdRPM = 0;
uint32_t obdSPEED = 0;
uint32_t obdTHROTTLE = 0;

//Thread
TimedAction elmduinoAction = TimedAction(300, getOBDValues);

void setupElmduino(){
  //SerialBT.setPin("1234");
  SerialBT.begin("ArduHUD", true);

  if (!SerialBT.connect("OBDII")){
    Serial.println("Couldn't connect to OBD scanner - Phase 1");
    while(1);
  }

  if (!myELM327.begin(SerialBT)){
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");
}
void loopElmduino() {
    elmduinoAction.check();
}

void getOBDValues() {
  float tempRPM = myELM327.rpm();

  if (myELM327.status == ELM_SUCCESS){
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
  }else{
    printError();
  }
}


void printError()
{
  Serial.print("Received: ");
  for (byte i = 0; i < myELM327.recBytes; i++)
    Serial.write(myELM327.payload[i]);
  Serial.println();

  if (myELM327.status == ELM_SUCCESS)
    Serial.println(F("\tELM_SUCCESS"));
  else if (myELM327.status == ELM_NO_RESPONSE)
    Serial.println(F("\tERROR: ELM_NO_RESPONSE"));
  else if (myELM327.status == ELM_BUFFER_OVERFLOW)
    Serial.println(F("\tERROR: ELM_BUFFER_OVERFLOW"));
  else if (myELM327.status == ELM_UNABLE_TO_CONNECT)
    Serial.println(F("\tERROR: ELM_UNABLE_TO_CONNECT"));
  else if (myELM327.status == ELM_NO_DATA)
    Serial.println(F("\tERROR: ELM_NO_DATA"));
  else if (myELM327.status == ELM_STOPPED)
    Serial.println(F("\tERROR: ELM_STOPPED"));
  else if (myELM327.status == ELM_TIMEOUT)
    Serial.println(F("\tERROR: ELM_TIMEOUT"));
  else if (myELM327.status == ELM_TIMEOUT)
    Serial.println(F("\tERROR: ELM_GENERAL_ERROR"));
}
