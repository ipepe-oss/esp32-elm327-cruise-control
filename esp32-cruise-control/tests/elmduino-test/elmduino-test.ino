#include "BluetoothSerial.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;

#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial

ELM327 myELM327;
float rpm = 0;
bool debug = false;

void setup(){
  DEBUG_PORT.begin(115200);
  ELM_PORT.begin("ArduHUD", true);
  while (!ELM_PORT.connect("V-LINK")){
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 1");
    while(1);
  }
  while (!myELM327.begin(ELM_PORT, debug, 10000)){
    DEBUG_PORT.println("Couldn't connect to OBD scanner - Phase 2");
  }
  Serial.println("Connected to ELM327");
}

void loop(){
  float tempRPM = myELM327.throttle();
  if (myELM327.status == ELM_SUCCESS){
    rpm = (float)tempRPM;
    Serial.print("RPM: ");
    Serial.println(rpm);
  }else{
    myELM327.printError();
  }
  delay(100);
}
