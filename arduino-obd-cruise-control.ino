#include "MyWifiManager.h"
#include "MyOTA.h"
#include "MyELM327Controller.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    setupWifiManager();
    setupOTA();
    setupELM327Manager();
}

void loop(){
    loopWifiManager();
    loopOTA();
    loopELM327Manager();
}
