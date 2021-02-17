#include "ObdPid.h"
#include "BluetoothSerial.h"
#include "TimedAction.h"

const uint8_t SECOND = 1000;

BluetoothSerial SerialBT;
ObdPid obd_speed("0D", 500);
ObdPid obd_throttle("11", 300);


bool sendCommand(String command, String expected_output, uint8_t timeout){
  return true;
}

void setupELM327(){
    SerialBT.begin("ESPCC", true); // bool BluetoothSerial::begin(String localName, bool isMaster)
    while(!SerialBT.connect("V-LINK")){
      Serial.println("Couldn't connect to OBD scanner - Phase 1");
      delay(1000);
    }
    Serial.println("Connected to BTSerial");

    while(!sendCommand(String("ATZ"), String("ELM327 v2.1"), 10*SECOND)){
      // waiting to connect
    }
    while(!sendCommand(String("AT SP0"), String("OK"), 10*SECOND)){
      // waiting to connect
    }
}

void loopELM327(){
      if (Serial.available()) {
        Serial.println("Sending command");
        SerialBT.write(Serial.read());
      }

      if (SerialBT.available()) {
        Serial.write(SerialBT.read());
      }
}
TimedAction btConnectionManager = TimedAction(100, loopELM327);


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

/// ELM327 communication timeout for reset = 30 seconds
// Cruise control all params have fresh data timeout =
// ELM327 command queue


//https://www.obdsol.com/knowledgebase/obd-software-development/reading-real-time-data/
//https://en.wikipedia.org/wiki/OBD-II_PIDs

//queueCommand(SERVICE_01 + SPEED_PID)
//queueCommand(SERVICE_01 + SPEED_PID)
//queueCommand(SERVICE_01 + SPEED_PID, true) true for putting command in front of queue
// queueCommand(SERVICE_01 + SPEED_PID) return false if queue has overflowed
// when commands are queued, responses are parsed in async
// when commands are sent, responsed are parsed in sync
