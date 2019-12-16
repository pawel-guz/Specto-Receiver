#pragma once
#include <WiFiUdp.h>
#include <WiFiServerSecure.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>  

#include "ConfigManager.h"

class WiFiConnectionManager
{ 
private: 
	static bool inNetwork;
public:
	WiFiConnectionManager();
	~WiFiConnectionManager();  
	static const int CONNECTION_TIMEOUT_MS = 15000;
	static void reconnect(ConfigManager& configManager, bool forceAP);
	static bool isInNetwork();
};

