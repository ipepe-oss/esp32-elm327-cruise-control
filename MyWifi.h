#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "ESPAsyncWiFiManager.h"

AsyncWebServer server(80);
DNSServer dns;
AsyncWiFiManager wifiManager(&server,&dns);

void setupWifiManager(){
  wifiManager.autoConnect("AutoConnectAP", "Password");
}

void loopWifiManager(){
  wifiManager.loop();
}
