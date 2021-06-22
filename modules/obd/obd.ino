


void setup() {
  Serial.begin(38400);
  Serial2.begin(38400);
}

void loop() {
  if (Serial.available()) {
    Serial2.write(Serial.read());
  }

  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }
}
