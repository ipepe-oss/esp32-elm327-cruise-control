#include "painlessMesh.h"
#include "meshsecrets.h"
#include "TimedAction.h"
#include "ArduinoJSON.h"

painlessMesh  mesh;

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
}

String preprareJsonMessage () {
  DynamicJsonDocument json(1024);
  json["from"] = "OBD";
  json["connected"] = true;
  json["speed"] = millis()/333;
  json["throttle"] = 6.77;
  String output;
  serializeJson(json, output);
  return output;
}

void broadcastDemoMessage(){
  Serial.println("Broadcasting...");
  String msg = preprareJsonMessage();
  Serial.println(msg);
  mesh.sendBroadcast(msg);
}

TimedAction logAction = TimedAction(333, broadcastDemoMessage);

void setup() {
  Serial.begin(115200);
//  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );
  mesh.init(MESH_PREFIX, MESH_PASSWORD);
  mesh.onReceive(&receivedCallback);
}

void loop() {
  mesh.update();
  logAction.check();
}
