#include "TimedAction.h"
#include "MyCCController.h"
#include "MyOBDController.h"

void logCurrentStatus(){
    Serial.println("[LOG] Speed: " + String(speed) + " Throttle: " + String(throttle));
    Serial.println("[LOG] Target speed: " + String(target_speed));
}

TimedAction logAction = TimedAction(1000, logCurrentStatus);

void setup(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("");
    Serial.println("[MAIN] Initializing Arduino OBD Cruise Control V0.1");
    setupCC();
    setupOBD();
    Serial.println("[MAIN] End of main setup");
}

void loop(){
    if(isEnabledCC()){
        loopOBD();
        loopCC();
    }else{
        emergencyStopCC();
    }
    logAction.check();
}
