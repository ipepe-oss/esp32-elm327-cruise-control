#include "BluetoothSerial.h"
#include "ELMduino.h"

//const char * const ADAPTER_ADDRESS = "24:42:16:08:00:00"; // "OBDII"
const char * const ADAPTER_ADDRESS = "00:1D:A5:15:E9:68"; // "V-LINK"

BluetoothSerial SerialBT;
ELM327 myELM327;
bool debug = false;

int obd_errors_count = 0;

void checkObdThrottle(){
  float temp = myELM327.throttle();
  if (myELM327.status == ELM_SUCCESS){
    current_throttle = (float)temp;
    obd_errors_count = 0;
  }else{
    obd_errors_count++;
    current_throttle = INVALID;
    current_speed = INVALID;
    myELM327.printError();
  }
}

void checkObdSpeed(){
  int32_t temp = myELM327.kph();
  if (myELM327.status == ELM_SUCCESS){
    current_speed = (int32_t)temp;
    obd_errors_count = 0;
  }else{
    obd_errors_count++;
    current_speed = INVALID;
    current_throttle = INVALID;
    myELM327.printError();
  }
}

TimedAction fastObdSpeedAction = TimedAction(503, checkObdSpeed);
TimedAction fastObdThrottleAction = TimedAction(251, checkObdThrottle);

TimedAction slowObdSpeedAction = TimedAction(2000, checkObdSpeed);
TimedAction slowObdThrottleAction = TimedAction(1000, checkObdThrottle);

void setupOBD(){
  Serial.println("[OBD] Start setupOBD");
  SerialBT.begin("ArduHUD", true); // bool BluetoothSerial::begin(String localName, bool isMaster)
  while (!SerialBT.connect(ADAPTER_ADDRESS)){
    Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 1");
  }
  Serial.println("[OBD] Connected to " + String(ADAPTER_ADDRESS));
  while (!myELM327.begin(SerialBT, debug, 10000)){
    Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 2");
  }
  Serial.println("[OBD] End setupOBD - Connected to ELM327 command line");
}

bool isEnabledOBD(){
  return obd_errors_count < 5 && current_speed >= 40 && current_throttle > 0;
}

void loopOBD(bool isEnabledNow){
  if(isEnabledNow){
    fastObdSpeedAction.check();
    fastObdThrottleAction.check();
  }else{
    slowObdSpeedAction.check();
    slowObdThrottleAction.check();
  }
    
}
