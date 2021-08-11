#include "TimedAction.h"
#include "MyHelpers.h"
#include "painlessMesh.h"
#include "meshsecrets.h"
#include "ArduinoJSON.h"

const int INVALID = -1;
unsigned long lastOBDUpdateTime;
bool isConnectedOBD = false;
float current_throttle = INVALID;
int32_t current_speed = INVALID;
const int CYTRON_V5 = 16;
const int CYTRON_M2A_CLUTCH_ON = 17;
const int CYTRON_M1A_SPEED_UP = 18;
const int CYTRON_M1B_SPEED_DOWN = 19;
const int CYTRON_STEP_MILIS = 25;
const float THROTTLE_BACKLASH_PERCENT = 0.15;
int target_speed = INVALID;
TaskHandle_t Task1;
TaskHandle_t Task2;
painlessMesh  mesh;

float Kp=1, Ki=0.2, Kd=0.2;
float Pout, Iout, Dout;
float actPWM, error = 0, lastError = 0, errorSum = 0, Derror = 0;


bool isEnabledOBD(){
  return isConnectedOBD && ((millis() - lastOBDUpdateTime) < 5000);
}

void logCurrentStatus(){
    Serial.println("[LOG] Speed: " + String(current_speed) + " Throttle: " + String(current_throttle) + " Target speed: " + String(target_speed));
    Serial.println("[LOG] isEnabledCC() " + String(isEnabledCC()) + " isEnabledOBD(): " + String(isEnabledOBD()));
}

TimedAction logAction = TimedAction(1000, logCurrentStatus);

float throttleCompensation(){
    return 0.0;
    return (target_speed - current_speed)*0.15;
}

float speedChangeMultiplier(float target_throttle){
  return 1;
  if(absDiff(current_speed, target_speed) < 10){
    return 1;
  }else{
    return 5;
  }
}

float pidCalcThrottle(){
    if (target_speed > current_speed){
      error = target_speed - current_speed;
    }else{
      error = (target_speed - current_speed) * 2;
    }
    error = target_speed - current_speed;
    errorSum = errorSum + ((error+lastError)*0.5);
    Derror = (error - lastError);
    Pout = Kp * error;
    Iout = Ki * errorSum;
    Dout = Kd * Derror;
    if(Iout>255){ Iout = 255;}
    if(Iout<0){Iout = 0;}

    actPWM = Pout + Iout + Dout;

    if(actPWM > 255){ actPWM = 255;}
    if(actPWM < 0){ actPWM = 0;}

    lastError = error;
    return 6.0+(actPWM / (5.0));
}

void speedUpCC(float target_throttle){
  //Serial.println("[CC] Speed UP");
  digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
  digitalWrite(CYTRON_M1A_SPEED_UP, HIGH);
  delay(CYTRON_STEP_MILIS * speedChangeMultiplier(target_throttle));
  digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
}

void speedDownCC(float target_throttle){
  //Serial.println("[CC] Speed DOWN");
  digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
  digitalWrite(CYTRON_M1B_SPEED_DOWN, HIGH);
  delay(CYTRON_STEP_MILIS * speedChangeMultiplier(target_throttle));
  digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
}

void setThrottleTo(float target_throttle, float throttle_compensation){
  Serial.println("[CC] setThrottleTo tt:" + String(target_throttle) + ", tc:" + String(throttle_compensation) + ", ct: " + String(current_throttle));
  if( (current_throttle - THROTTLE_BACKLASH_PERCENT) > (target_throttle + throttle_compensation)){
    speedDownCC(target_throttle + throttle_compensation);
  }else if ( (current_throttle + THROTTLE_BACKLASH_PERCENT) < (target_throttle + throttle_compensation)){
    speedUpCC(target_throttle + throttle_compensation);
  }
}

void handleCruising(){
  digitalWrite(CYTRON_M2A_CLUTCH_ON, HIGH);
  setThrottleTo(pidCalcThrottle(), throttleCompensation());
}

TimedAction cruisingAction = TimedAction(400, handleCruising);

void emergencyStopCC(){
    if(target_speed != INVALID || digitalRead(CYTRON_M2A_CLUTCH_ON)){
      Serial.println("[CC] EMERGENCY STOP!");
    }
    digitalWrite(CYTRON_M2A_CLUTCH_ON, LOW);
    digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
    digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
    target_speed = INVALID;
    if(digitalRead(CYTRON_V5)){
        Serial.println("[CC] EMERGENCY STOP! - Waiting for button depress");
    }
}

bool isEnabledCC(){
    return digitalRead(CYTRON_V5);
}

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
     mesh.update();
     delay(1);
  }
}

void receivedCallback(uint32_t from, String &msg) {
  //Serial.printf("[MESH] Received from %u msg=%s\n", from, msg.c_str());
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, msg.c_str());
  String msg_from = doc["from"];
  //Serial.println("[MESH] msg_from: " + msg_from);
  if(msg_from == String("OBD")){
    lastOBDUpdateTime = millis();
    //Serial.println("[MESH] From is equal to OBD");
    isConnectedOBD = doc["connected"];
    current_speed = doc["speed"];
    current_throttle = doc["throttle"];
  }
}


void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
       if(isEnabledCC() && isEnabledOBD() && current_speed > 30){
         if(target_speed == INVALID){
             target_speed = current_speed;
             error = 0;
             lastError = 0;
             errorSum = 0;
             Derror = 0;
             speedUpCC(20);
         }
         cruisingAction.check();
       }else{
         emergencyStopCC();
       }
       delay(1);
  }
}

void setup(){
    Serial.begin(115200);
    delay(500);
    Serial.println("");
    Serial.println("[MAIN] Initializing Arduino OBD Cruise Control V3.0");
    //mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);
    mesh.init(MESH_PREFIX, MESH_PASSWORD);
    mesh.onReceive(&receivedCallback);
    Serial.println("[CC] Setup start");
    pinMode(CYTRON_V5, INPUT);
    pinMode(CYTRON_M2A_CLUTCH_ON, OUTPUT);
    pinMode(CYTRON_M1A_SPEED_UP, OUTPUT);
    pinMode(CYTRON_M1B_SPEED_DOWN, OUTPUT);
    emergencyStopCC();
    Serial.println("[CC] Setup end");
    xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 1);
    xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 0);
    Serial.println("[MAIN] End of main setup");
}

void loop(){
    logAction.check();
    delay(10);
}