#include "ELMduino.h"
#include "TimedAction.h"

// BT HC-05
// pin 1 = TX
// pin 2 = RX
// ESP32
// pin 17 = uart2 TX
// pin 16 = uart2 RX


ELM327 myELM327;
bool debug = false;

int obd_errors_count = 0;


const int INVALID = -1;
float current_throttle = INVALID;
int32_t current_speed = INVALID;

void checkObdThrottle(){
  Serial.println("Query: throttle");
  float temp = myELM327.throttle();
  if (myELM327.status == ELM_SUCCESS){
    
    current_throttle = (float)temp;
    obd_errors_count = 0;
    Serial.println("Throttle: " + String(current_throttle));
  }else{
    obd_errors_count++;
    current_throttle = INVALID;
    current_speed = INVALID;
    myELM327.printError();
  }
}

void checkObdSpeed(){
  Serial.println("Query: speed");
  int32_t temp = myELM327.kph();
  if (myELM327.status == ELM_SUCCESS){
    current_speed = (int32_t)temp;
    obd_errors_count = 0;
    Serial.println("Speed: " + String(current_speed));
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
  while (!myELM327.begin(Serial2, debug, 10000)){
    Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 2");
  }
  Serial.println("[OBD] End setupOBD - Connected to ELM327 command line");
}

bool obdIsConnected(){
  return obd_errors_count < 10 && myELM327.connected;
}

void loopOBD(bool isEnabledNow){
  if(obdIsConnected()){
    if(isEnabledNow){
      fastObdSpeedAction.check();
      fastObdThrottleAction.check();
    }else{
      slowObdSpeedAction.check();
      slowObdThrottleAction.check();
    }
  }else{
    obd_errors_count = 0;
    myELM327.begin(Serial2, debug, 10000);
  }
}

void setup() {
  Serial.begin(38400);
  Serial2.begin(38400);
  setupOBD();
}

void loop() {
    loopOBD(true);
  
}
