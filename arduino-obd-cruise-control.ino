#include "TimedAction.h"
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncWiFiManager.h"

AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server,&dns);

void setup(){
    Serial.begin(115200);
    delay(3000);
    wifiManager.autoConnect("AutoConnectAP", "Password");
    Serial.println("Setup finished");
}

void helloLoop(){
 Serial.println("Hello loop"); 
}

TimedAction helloThread = TimedAction(5000, helloLoop);
void loop(){
  helloThread.check();
  wifiManager.loop();
}
