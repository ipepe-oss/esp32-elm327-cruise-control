#ifndef ARDUINO_AVR_LEONARDO
#error This file is only for Arduino Leonardo
#endif

#include <SPI.h>
#include "mcp_can.h"

#define PID_ENGINE_RPM      0x0C
#define PID_VEHICLE_SPEED   0x0D
#define CAN_ID_PID          0x7DF

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

const int LED_PIN = 13;

int obdVehicleSpeed = 0;
int obdEngineRPM    = 0;

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


// 0 : no
// 1 : yes
unsigned char flgDriving = 0;
unsigned long timerStop;

void blink()
{
    static int ledstatus = 0;
    
    if(flgDriving == 0)
    {
        static unsigned long timer_s = millis();
        if(millis()-timer_s < 200)return;
        timer_s = millis();
        
        ledstatus = 1-ledstatus;
        digitalWrite(13, ledstatus);
    }
    else
    {
        static unsigned long timer_s = millis();
        if(millis()-timer_s < 1000)return;
        timer_s = millis();
        
        ledstatus = 1-ledstatus;
        digitalWrite(13, ledstatus);
    }
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
            
            CAN.readMsgBuf(&len, dta);    // read data,  len: data length, buf: data buf
            if(dta[1] == 0x41 && dta[2] == __pid)
            {
                return 1;
            }
        }
    }
    
    return 0;
}

void obdProcess()
{
    if(!flgDriving)return;
    static unsigned long timer_s = millis();
    if(millis()-timer_s < 1000)return;
    timer_s = millis();
    
    unsigned char dta[8];
    
    // speed
    if(getPidFromCar(PID_VEHICLE_SPEED, dta))
    {
        obdVehicleSpeed = dta[3];
    }
    
    // rpm
    if(getPidFromCar(PID_ENGINE_RPM, dta))
    {
        obdEngineRPM = (256.0*(float)dta[3]+(float)dta[4])/4.0;
    }
}


// END FILE

void setup() 
{
    Serial.begin(115200);
    Serial1.begin(9600);
    
    //while(!Serial.available());
    
    pinMode(LED_PIN, OUTPUT);
    
    Serial.println("card initialized.");
    
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
    obdProcess();   
    blink();
}
