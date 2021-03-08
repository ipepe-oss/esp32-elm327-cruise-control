const int CYTRON_V5 = 16;
const int CYTRON_M2A_CLUTCH_ON = 17;
const int CYTRON_M1A_SPEED_UP = 18;
const int CYTRON_M1B_SPEED_DOWN = 19;

int target_speed = -1;

void emergencyStopCC(){
    digitalWrite(CYTRON_M2A_CLUTCH_ON, LOW);
    target_speed = -1;
    Serial.println("[CC] EMERGENCY STOP!");
}

void setupCC() {
  Serial.println("[CC] Setup start");
  pinMode(CYTRON_V5, INPUT);
  pinMode(CYTRON_M2A_CLUTCH_ON, OUTPUT);
  pinMode(CYTRON_M1A_SPEED_UP, OUTPUT);
  pinMode(CYTRON_M1B_SPEED_DOWN, OUTPUT);
  emergencyStopCC();
  Serial.println("[CC] Setup end");
}

void loopCC() {

  delay(500);
}

bool isEnabledCC(){
    return digitalRead(CYTRON_V5);
}
