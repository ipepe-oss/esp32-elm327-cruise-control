#include "MyWifi.h"
#include "MyOTA.h"
#include "MyELM327.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    Serial.println("Initializing Arduino OBD Cruise Control V0.1");
    setupWifi();
    setupOTA();
    setupELM327();
}

void loop(){
    loopWifi();
    loopOTA();
    loopELM327();
}
