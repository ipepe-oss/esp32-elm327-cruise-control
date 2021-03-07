#include "MyWifiManager.h"
#include "MyOTA.h"
#include "MyElmduino.h"

void setup(){
    Serial.begin(115200);
    delay(3000);
    Serial.println("Setting up Wifi Manager");
    setupWifiManager();
    Serial.println("Setting up OTA");
    setupOTA();
    Serial.println("Setting up Elduino");
    setupElmduino();
    Serial.println("Setup finished");
}

void loop(){
    loopWifiManager();
    loopOTA();
    loopElmduino();
}
