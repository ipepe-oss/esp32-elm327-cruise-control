//#include "MyWifi.h"
//#include "MyOTA.h"
#include "MyELM327.h"

void ensureSafety(){
    cc_ensure_cytron_stop();
}

void setup(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("");
    Serial.println("[MAIN] Initializing Arduino OBD Cruise Control V0.1");
    //setupWifi();
    //setupOTA();
    setupELM327();
    //setupCruiseController();
    Serial.println("[MAIN] End of main setup");
}

void loop(){
    //loopWifi();
    //loopOTA();
    loopELM327();
    //loopCruiseController();
}
