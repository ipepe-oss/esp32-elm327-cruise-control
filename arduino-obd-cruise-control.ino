#ifndef ARDUINO_AVR_LEONARDO
#error This file is only for Arduino Leonardo
#endif

#include <SPI.h>
#include "mcp_can.h"

// OBD CONFIGURATION (PIDS)
#define PID_ENGINE_RPM      0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_THROTTLE_POSITION 0x11
#define PID_ENGINE_COOLANT_TEMP 0x05
#define CAN_ID_PID          0x7DF

int obdVehicleSpeed     = 0;
int obdEngineRPM        = 0;
int obdThrottlePosition = 0;
int obdEngineCoolantTemp = 0;

const int CAN_SPI_CS_PIN = 9;
MCP_CAN CAN(CAN_SPI_CS_PIN); // Set CS pin

// Cytron motor controller pins:
const int M2A_CLUTCH_ON = A1;
const int CC_TOGGLE     = A0;
const int M1A_SPEED_UP  = 11;
const int M1B_SLOW_DOWN = 10;

// Other configurations
const int LED_PIN = 13;

// Miscellaneous
const int SECOND = 1000;

void set_mask_filt()
{
    // set mask, set both the mask to 0x3ff
    CAN.init_Mask(0, 0, 0x7FC);
    CAN.init_Mask(1, 0, 0x7FC);

    // set filter, we can receive id from 0x04 ~ 0x09

    CAN.init_Filt(0, 0, 0x7E8);                 
    CAN.init_Filt(1, 0, 0x7E8);

    CAN.init_Filt(2, 0, 0x7E8);
    CAN.init_Filt(3, 0, 0x7E8);
    CAN.init_Filt(4, 0, 0x7E8); 
    CAN.init_Filt(5, 0, 0x7E8);
}

// OBD

// return:
// 0 - timeout
// 1 - ok

unsigned char getPidFromCar(unsigned char __pid, unsigned char *dta)
{
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
    
    unsigned long timer_s = millis();
    
    while(1)
    {
        if(millis()-timer_s > 100)
        {
            obdVehicleSpeed = 0;
            obdEngineRPM    = 0;
            return 0;
        }
        
        unsigned char len = 0;
        
        if(CAN_MSGAVAIL == CAN.checkReceive())                   // check if get data
        {
          Serial.println("CAN_MSGAVAIL == CAN.checkReceive()");
            
            CAN.readMsgBuf(&len, dta);    // read data,  len: data length, buf: data buf
            if(dta[1] == 0x41 && dta[2] == __pid)
            {
              Serial.println("dta[1] == 0x41 && dta[2] == __pid");
                return 1;
            }
        }else{
          Serial.println("CAN.checkReceive(): ");
          Serial.println(CAN.checkReceive());
          Serial.println(CAN.checkError());
          delay(10);
        }
    }
    
    return 0;
}

//void safetyChecks(){
//    if (millis() - timeSinceTargetSpeedWasClose > SECOND*30){
//
//    }
//}

void obdProcess()
{
    static unsigned long timer_s = millis();
    if(millis()-timer_s < 1000)return;
    timer_s = millis();
    
    unsigned char dta[8];
    Serial.println("Getting obd data");
    
    // speed
    if(getPidFromCar(PID_VEHICLE_SPEED, dta))
    {
      Serial.println("Success getting Speed PID");
        obdVehicleSpeed = dta[3];
    }
    
    // rpm
    if(getPidFromCar(PID_ENGINE_RPM, dta))
    {
      Serial.println("Success getting RPM PID");
        obdEngineRPM = (256.0*(float)dta[3]+(float)dta[4])/4.0;
    }

    //throttle
    if(getPidFromCar(PID_THROTTLE_POSITION, dta))
    {
      Serial.println("Success getting Throttle PID");
        obdThrottlePosition = dta[3];
    }
    if(getPidFromCar(PID_ENGINE_COOLANT_TEMP, dta)){
      Serial.println("Success getting engine coolant temp data");
      obdEngineCoolantTemp = dta[3];
    }
}


// END FILE

void setup() 
{
    delay(3000);
    Serial.begin(115200);
    Serial1.begin(9600);

    Serial.println("Hello setup!");
    
    pinMode(LED_PIN, OUTPUT);
    
    while (CAN_OK != CAN.begin(CAN_500KBPS))    // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    set_mask_filt();
}


void loop()
{
  Serial.println("Hello loop!");
  Serial.println(obdThrottlePosition);
  Serial.println(obdVehicleSpeed);
  Serial.println(obdEngineRPM);
  Serial.println(obdEngineCoolantTemp);

  obdProcess();
}
