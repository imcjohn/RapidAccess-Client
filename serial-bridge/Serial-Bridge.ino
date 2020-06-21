// include modules for wifi
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "configuration.h"

// Constants
#define BAUD_SERIAL 115200
#define SSH_PORT 22
#define RXBUFFERSIZE 1024

// Working Variables 
WiFiServer server(SSH_PORT);
ESP8266WebServer webInterface(80);
IPAddress wifiIP;
Status auth = Disconnected;
Config conf;

void setup() {
  delay(1000);
  // Initialize hardware
  Serial.begin(BAUD_SERIAL);
  Serial.setRxBufferSize(RXBUFFERSIZE);
  // Load configuration
  boolean useDefaults = !SPIFFS.begin(); // only force use defaults if SPIFFS initialization fails
  conf = loadConfig(useDefaults);
  // Start WiFi
  WiFi.persistent(false);
  if (conf.station){
    WiFi.mode(WIFI_STA);
    WiFi.begin(conf.ssid, conf.pass);
    int c = 0;
    while (WiFi.status() != WL_CONNECTED && c < 20) {
      delay(500);
      c++;
    }
    if (WiFi.status() != WL_CONNECTED){
      Config defaults = loadConfig(true);
      WiFi.softAP(defaults.ssid, defaults.pass); // in the case of failed connect, use default wifi creds
    }
  }
  else{
    WiFi.softAP(conf.ssid, conf.pass);
  }
  wifiIP = WiFi.softAPIP();
  // Start mDns
  MDNS.begin(conf.host);
  // start SSH forwarding server
  server.begin();
  server.setNoDelay(true);
  // start WebUI
  startWebUI();  
}

void loop() {
  if (auth != Authenticated) {
    auth = handleAuthentication();
    if (auth == Reset){
      Config def = loadConfig(true);
      dumpConfig(def);
      delay(1000);
      ESP.restart();
    }
  }
  else {
    handleSerial();
  }
  webInterface.handleClient();
}
