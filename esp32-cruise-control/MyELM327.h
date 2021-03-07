#include "ObdPid.h"
#include "BluetoothSerial.h"

const char * const ADAPTER_ADDRESS = "OBDII";
//const char * const ADAPTER_ADDRESS = "V-LINK";

// request commands
const char * const RESET_ELM = "ATZ";
const char * const ECHO_OFF = "ATE0";
const char * const PRINTING_SPACES_OFF = "ATS0";
const char * const CONNECT_TO_ECU = "ATSP0";
const char * const WARMUP_COMMAND = "0111";
// expected responses
const char * const OK_RESPONSE = "OK";
const char * const ELM_INTRO = "ELM327 v2.1";
// helpful constants
const uint8_t ONE_SECOND = 1000;
// obd setup
BluetoothSerial SerialBT;
ObdPid obd_speed(String("0D"), String("SPEED"), 500);
ObdPid obd_throttle(String("11"), String("THROTTLE"), 300);
// obd/elm327 status flags
bool one_time_connect = true;

bool sendCommand(String command, String expected_output, uint8_t timeout){
  Serial.println("[ELM327-BT] sendCommand: " + command);
  SerialBT.print(String(command));
  SerialBT.setTimeout(timeout);
  String result = String(SerialBT.readStringUntil(10));
  Serial.println("[ELM327-BT] received: " + result);
  return (result.indexOf(expected_output) > -1);
}

void setupELM327(){
    Serial.println("[ELM327-BT] Start setupELM327");
    SerialBT.begin("ESPCC", true); // bool BluetoothSerial::begin(String localName, bool isMaster)
}

void setupELM327Connection(){
  if(!one_time_connect) return;
  one_time_connect = false;
  while(!SerialBT.connect("OBDII")) delay(ONE_SECOND);
  sendCommand(RESET_ELM, ELM_INTRO, 10*ONE_SECOND) &&
    sendCommand(ECHO_OFF, OK_RESPONSE, ONE_SECOND) &&
    sendCommand(PRINTING_SPACES_OFF, OK_RESPONSE, ONE_SECOND) &&
    sendCommand(CONNECT_TO_ECU, OK_RESPONSE, ONE_SECOND) &&
    sendCommand(WARMUP_COMMAND, "4111", 10*ONE_SECOND);
}


void loopELM327(){
  if (Serial.available()) { SerialBT.write(Serial.read()); }
  if (SerialBT.available()) { Serial.write(SerialBT.read()); }
  setupELM327Connection();
      //SerialBT.setTimeout(1000);
      //Serial.setTimeout(1000);
    //Serial.println(SerialBT.readStringUntil(10));
    //SerialBT.println(Serial.readStringUntil(10));
}

//TimedAction btConnectionManager = TimedAction(100, loopELM327);


// "UNABLE TO CONNECT" - ELM STATUS FAILURE
// "NO DATA" - ELM STATUS FAILURE
// "?" - ELM STATUS FAILURE
// "ELM327 v2.1" only after initialization

// RPM REQ: 010C
// RPM RES: 41 0C 0C 2D ~100ms
// RPM RES: 41 0C 00 00 - 0 rpm
// SPEED REQ: 010D
// SPEED RES: 41 0D 00
// THROTTLE REQ: 0111
// THROTTLE RES: 41 11 11 - idle
// THROTTLE RES: 41 11 11 - idle
// THROTTLE RES: 41 11 E4 - full


//To get the actual RPM value, convert the hex number to decimal, and divide it by four:
//
//0x0FA0 = 4000
//4000 / 4 = 1000 rpm

//https://www.obdsol.com/knowledgebase/obd-software-development/reading-real-time-data/
//https://en.wikipedia.org/wiki/OBD-II_PIDs

//queueCommand(SERVICE_01 + SPEED_PID)
//queueCommand(SERVICE_01 + SPEED_PID)
//queueCommand(SERVICE_01 + SPEED_PID, true) true for putting command in front of queue
// queueCommand(SERVICE_01 + SPEED_PID) return false if queue has overflowed
// when commands are queued, responses are parsed in async
// when commands are sent, responsed are parsed in sync
