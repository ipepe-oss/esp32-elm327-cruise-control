#include "MyWifiManager.h"
#include "MyOTA.h"
#include "MyELM327Controller.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    Serial.println("Initializing Arduino OBD Cruise Control V0.1");
    setupWifiManager();
    setupOTA();
    setupELM327();
}

void loop(){
    loopWifiManager();
    loopOTA();
    loopELM327();
}
