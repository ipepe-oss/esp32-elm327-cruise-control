#include "DFRobot_RGBLCD.h"
#include "painlessMesh.h"
#include "meshsecrets.h"
#include "ArduinoJSON.h"

painlessMesh  mesh;
int obd_speed = -1;
float obd_throttle = -1;
bool updateLcd = true;

DFRobot_RGBLCD lcd(16,2);  //16 characters and 2 lines of show

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, msg.c_str());
  String msg_from = doc["from"];
  Serial.println("msg_from: " + msg_from);
  if(msg_from == String("OBD")){
    Serial.println("From is equal to OBD");
    obd_speed = doc["speed"];
    obd_throttle = doc["throttle"];
    updateLcd = true;
  }
}

void setup() {
    Serial.begin(115200);
//    mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE);
    mesh.init(MESH_PREFIX, MESH_PASSWORD);
    mesh.onReceive(&receivedCallback);
    lcd.init();
    lcd.setRGB(255, 0, 0);
}

void loop() {
  if(updateLcd){
    updateLcd = false;
//    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("S:" + String(obd_speed));
    lcd.setCursor(9, 0);
    lcd.print("T:" + String(obd_throttle));
  }
  mesh.update();
}
