#include "MyWifiManager.h"
#include "MyOTA.h"
#include "MyElmduino.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    setupWifiManager();
    setupOTA();
    setupElmduino();
}

void loop(){
    loopWifiManager();
    loopOTA();
    loopElmduino();
}
