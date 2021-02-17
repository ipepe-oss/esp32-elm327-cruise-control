#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncWiFiManager.h"

AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server,&dns);

void setupWifi(){
  wifiManager.autoConnect("ESPCC-AP", "Password");
}

void loopWifi(){
  wifiManager.loop();
}
