#include "TimedAction.h"
#include "BluetoothSerial.h"
#include "ELMduino.h"

// NONAME MAC 24:42:16:08:00:00
// V-LINK MAC 00:1D:A5:15:E9:68
BluetoothSerial SerialBT;
ELM327 myELM327;

// OBD Parameters
uint32_t obdRPM = 0;
uint32_t obdSPEED = 0;
uint32_t obdTHROTTLE = 0;

void setupElmduino(){
  //SerialBT.setPin("1234");
  SerialBT.begin("ESP32-CC", true);

  if (!SerialBT.connect("V-LINK")){
    Serial.println("Couldn't connect to OBD scanner - Phase 1");
    while(1);
  }

  if (!myELM327.begin(SerialBT)){
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");
  delay(10000);
  Serial.println(myELM327.sendCommand(READ_VOLTAGE));
  Serial.println(myELM327.payload);
}


void printError(){
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
  else if (myELM327.status == ELM_GENERAL_ERROR)
    Serial.println(F("\tERROR: ELM_GENERAL_ERROR"));
}

void getOBDValues() {
  float tempRPM = myELM327.rpm();

  if (myELM327.status == ELM_SUCCESS){
    obdRPM = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(obdRPM);
  }else{
    printError();
  }


//  Serial.println("OBD!");
//  Serial.println(myELM327.rpm());
//  Serial.println(myELM327.speed());
//  Serial.println(myELM327.throttle());
//  Serial.println(myELM327.queryPID(SERVICE_01, THROTTLE_POSITION));
//  Serial.println(myELM327.payload);
//  Serial.println(myELM327.sendCommand(READ_VOLTAGE));
//  Serial.println(myELM327.payload);
}


TimedAction elmduinoAction = TimedAction(1000, getOBDValues);
void loopElmduino() {
    elmduinoAction.check();
}
