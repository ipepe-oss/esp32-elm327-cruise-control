#include "BluetoothSerial.h"
#include "ELMduino.h"

//const char * const ADAPTER_ADDRESS = "OBDII";
const char * const ADAPTER_ADDRESS = "V-LINK";

BluetoothSerial SerialBT;
ELM327 myELM327;
bool debug = false;

float throttle = -1;
int32_t speed = -1;
int obd_errors_count = 0;


void checkObdThrottle(){
  float temp = myELM327.throttle();
  if (myELM327.status == ELM_SUCCESS){
    throttle = (float)temp;
  }else{
    obd_errors_count++;
    myELM327.printError();
  }
}

void checkObdSpeed(){
  int32_t temp = myELM327.kph();
  if (myELM327.status == ELM_SUCCESS){
    speed = (int32_t)temp;
  }else{
    obd_errors_count++;
    myELM327.printError();
  }
}

TimedAction obdSpeedAction = TimedAction(503, checkObdSpeed);
TimedAction obdThrottleAction = TimedAction(251, checkObdThrottle);


void setupOBD(){
  Serial.println("[OBD] Start setupOBD");
  SerialBT.begin("ArduHUD", true); // bool BluetoothSerial::begin(String localName, bool isMaster)
  while (!SerialBT.connect(ADAPTER_ADDRESS)){
    Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 1");
  }
  while (!myELM327.begin(SerialBT, debug, 5000)){
    Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 2");
  }
  Serial.println("[OBD] End setupOBD - Connected to ELM327");
}

void loopOBD(){
    obdSpeedAction.check();
    obdThrottleAction.check();
    if(obd_errors_count > 6){
        emergencyStopCC();
    }
}
