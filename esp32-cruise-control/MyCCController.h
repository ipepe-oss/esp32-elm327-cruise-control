const int CYTRON_V5 = 16;
const int CYTRON_M2A_CLUTCH_ON = 17;
const int CYTRON_M1A_SPEED_UP = 18;
const int CYTRON_M1B_SPEED_DOWN = 19;

const int CYTRON_STEP_MILIS = 30;

int target_speed = -1;

void speedUpCC(){
  //Serial.println("[CC] Speed UP");
  digitalWrite(CYTRON_M1A_SPEED_UP, HIGH);
  delay(CYTRON_STEP_MILIS);
  digitalWrite(CYTRON_M1A_SPEED_UP, LOW);
}

void speedDownCC(){
  //Serial.println("[CC] Speed DOWN");
  digitalWrite(CYTRON_M1B_SPEED_DOWN, HIGH);
  delay(CYTRON_STEP_MILIS);
  digitalWrite(CYTRON_M1B_SPEED_DOWN, LOW);
}


void setThrottleTo(float target_throttle){
  //Serial.println("[CC] setThrottleTo " + String(target_throttle) + " " + String(current_throttle));
  if( (current_throttle - 0.25) > target_throttle  ){
    speedDownCC();
  }else if ( (current_throttle + 0.25) < target_throttle ){
    speedUpCC();
  }
}

void handleCruising(){
  digitalWrite(CYTRON_M2A_CLUTCH_ON, HIGH);
  setThrottleTo(TARGET_THROTTLE);
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
    cruisingAction.check();
  }else{
    emergencyStopCC();
  }
}

bool isEnabledCC(){
    return digitalRead(CYTRON_V5);
}
