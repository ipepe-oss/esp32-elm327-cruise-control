#include "TimedAction.h"
#include "MyHelpers.h"

const int INVALID = -1;
float current_throttle = INVALID;
int32_t current_speed = INVALID;

#include "MyCCController.h"
#include "MyOBDController.h"

void logCurrentStatus(){
    Serial.println("[LOG] Speed: " + String(current_speed) + " Throttle: " + String(current_throttle) + " Target speed: " + String(target_speed));
    Serial.println("[LOG] isEnabledCC() " + String(isEnabledCC()) + " isEnabledOBD(): " + String(isEnabledOBD()));
}

TimedAction logAction = TimedAction(1000, logCurrentStatus);

void setup(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("");
    Serial.println("[MAIN] Initializing Arduino OBD Cruise Control V0.6");
    setupCC();
    setupOBD();
    Serial.println("[MAIN] End of main setup");
}

void loop(){
    bool isEnabledNow = isEnabledCC() && isEnabledOBD();
    loopOBD(isEnabledNow);
    loopCC(isEnabledNow);
    logAction.check();
}
