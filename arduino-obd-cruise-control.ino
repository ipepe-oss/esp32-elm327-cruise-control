#include "TimedAction.h"
#include "MyWifiManager.h"
#include "MyOTA.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    setupWifiManager();
    setupOTA();
}

void loop(){
  loopWifiManager();
  loopOTA();
}
