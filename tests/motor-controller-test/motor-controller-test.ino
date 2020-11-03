#ifndef ARDUINO_AVR_LEONARDO
#error This file is only for Arduino Leonardo
#endif

// Cytron motor controller pins:
const int CC_TOGGLE     = A0;
const int M2A_CLUTCH_ON = A1;
const int M1A_SPEED_UP  = 11;
const int M1B_SLOW_DOWN = 10;

// Miscellaneous
const int SECOND = 1000;

void setup()
{
    delay(SECOND*5);
    Serial.begin(115200);
    delay(SECOND);
    Serial.println("Hello setup!");

    pinMode(CC_TOGGLE, INPUT);
    pinMode(M2A_CLUTCH_ON, OUTPUT);
    
    digitalWrite(M2A_CLUTCH_ON, 1);

    Serial.println(digitalRead(CC_TOGGLE));

    delay(SECOND);
    
}


void loop()
{
  Serial.println("Hello loop!");
  digitalWrite(M2A_CLUTCH_ON, 0);
  delay(SECOND);
  
}
