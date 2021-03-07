
const int CYTRON_V5 = 16;
const int CYTRON_M2A_CLUTCH_ON = 17;
const int CYTRON_M1A_SPEED_UP = 18;
const int CYTRON_M1B_SPEED_DOWN = 19;

bool toggle = false;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setup");
  pinMode(CYTRON_V5, INPUT);
  pinMode(CYTRON_M2A_CLUTCH_ON, OUTPUT);
  pinMode(CYTRON_M1A_SPEED_UP, OUTPUT);
  pinMode(CYTRON_M1B_SPEED_DOWN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Loop" + String(toggle));
  if (toggle){
    toggle = false;
    digitalWrite(CYTRON_M2A_CLUTCH_ON, HIGH);
  }else{
    toggle = true;
    digitalWrite(CYTRON_M2A_CLUTCH_ON, LOW);
  }
  delay(500);
}
