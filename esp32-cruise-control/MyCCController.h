const int CYTRON_V5 = 16;
const int CYTRON_M2A_CLUTCH_ON = 17;
const int CYTRON_M1A_SPEED_UP = 18;
const int CYTRON_M1B_SPEED_DOWN = 19;

const int CYTRON_STEP_MILIS = 25;
const float THROTTLE_BACKLASH_PERCENT = 0.15;

int target_speed = -1;

float speedToThrottle(int p_speed){
    return (((float)p_speed) + 3.0) / 6.0;
}

float throttleCompensation(){
    return (target_speed - current_speed)*0.05;
}

void speedUpCC(int multiplier){
  //Serial.println("[CC] Speed UP");
  digitalWrite(CYTRON_M1A_SPEED_UP, HIGH);
  delay(CYTRON_STEP_MILIS * multiplier);
  digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
}

void speedDownCC(int multiplier){
  //Serial.println("[CC] Speed DOWN");
  digitalWrite(CYTRON_M1B_SPEED_DOWN, HIGH);
  delay(CYTRON_STEP_MILIS * multiplier);
  digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
}

void setThrottleTo(float target_throttle, float throttle_compensation){
  //Serial.println("[CC] setThrottleTo " + String(target_throttle) + " " + String(current_throttle));
  if( (current_throttle - THROTTLE_BACKLASH_PERCENT) > (target_throttle + throttle_compensation)){
    speedDownCC((int)throttle_compensation);
  }else if ( (current_throttle + THROTTLE_BACKLASH_PERCENT) < (target_throttle + throttle_compensation)){
    speedUpCC((int)throttle_compensation);
  }
}

void handleCruising(){
  digitalWrite(CYTRON_M2A_CLUTCH_ON, HIGH);
  setThrottleTo(speedToThrottle(target_speed), throttleCompensation());
}

TimedAction cruisingAction = TimedAction(500, handleCruising);

void emergencyStopCC(){
    if(target_speed != -1 || digitalRead(CYTRON_M2A_CLUTCH_ON)){
      Serial.println("[CC] EMERGENCY STOP!");  
    }
    digitalWrite(CYTRON_M2A_CLUTCH_ON, LOW);
    digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
    digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
    target_speed = -1;
    while(digitalRead(CYTRON_V5)){
        Serial.println("[CC] EMERGENCY STOP! - Waiting for button depress");
        delay(500);
    }
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

void loopCC(bool isEnabledNow) {
  if(isEnabledNow){
    if(target_speed == -1){
        target_speed = current_speed;
    }
    cruisingAction.check();
  }else{
    emergencyStopCC();
  }
}

bool isEnabledCC(){
    return digitalRead(CYTRON_V5);
}
