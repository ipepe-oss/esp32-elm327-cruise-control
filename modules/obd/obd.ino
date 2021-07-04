#include "ELMduino.h"
#include "TimedAction.h"
#include "painlessMesh.h"
#include "meshsecrets.h"
#include "ArduinoJSON.h"

// BT HC-05
// pin 1 = TX
// pin 2 = RX
// ESP32
// pin 17 = uart2 TX
// pin 16 = uart2 RX

ELM327 myELM327;
int obd_errors_count = 0;
const int INVALID = -1;
float current_throttle = INVALID;
int32_t current_speed = INVALID;
TaskHandle_t Task1;
TaskHandle_t Task2;
painlessMesh  mesh;

String preprareJsonMessage () {
  DynamicJsonDocument json(1024);
  json["from"] = "OBD";
  json["connected"] = obdIsConnected();
  json["speed"] = current_speed;
  json["throttle"] = current_throttle;
  String output;
  serializeJson(json, output);
  return output;
}

void broadcastMessage(){
  Serial.println("Broadcasting...");
  String msg = preprareJsonMessage();
  Serial.println(msg);
  mesh.sendBroadcast(msg);
}

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

bool obdIsConnected(){
  return obd_errors_count < 10 && myELM327.connected;
}

void loopOBD(){
  if(obdIsConnected()){
    checkObdThrottle();
    broadcastMessage();
    checkObdSpeed();
    broadcastMessage();
    checkObdThrottle();
    broadcastMessage();
  }else{
    obd_errors_count = 0;
    current_throttle = INVALID;
    current_speed = INVALID;
    broadcastMessage();
    myELM327.begin(Serial2, false, 10000);
  }
}

TimedAction timedLoopOBD = TimedAction(700, loopOBD);

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
     mesh.update();
     delay(1);
  }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
     timedLoopOBD.check();
     delay(1);
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(38400);
  Serial.println("[OBD] Start setupOBD");
  //    mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);
  mesh.init(MESH_PREFIX, MESH_PASSWORD);
  while (!myELM327.begin(Serial2, false, 10000)){
      Serial.println("[OBD] Couldn't connect to OBD scanner - Phase 2");
  }
  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);

  Serial.println("[OBD] End setupOBD - Connected to ELM327 command line");
}

void loop() {
  delay(10);
}
