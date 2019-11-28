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
#include <SoftwareSerial.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>

#include "FS.h"
#include "ConfigManager.h"
#include "UDPMessengerService.h"
#include "SerialMessengerService.h"
#include "RGBColorController.h"

/*
	Name:		Specto_Receiver.ino 
	https://github.com/pawel-guz/Specto 
	Author:	gp
*/

ConfigManager configManager = ConfigManager::getInstance();
UDPMessengerService udpMessenger(1234);
SerialMessengerService serialMessenger;

void setup() 
{
	RGBColorController::initDiodes();

	Serial.begin(19200);
	Serial.println("[REBOOT]");
	if (!configManager.load())
		Serial.println("> Unable to load configuration.");

	char ssid[64] = "";
	char pass[64] = "";

	if (!configManager.isDeviceConfigured())
	{
		WiFi.mode(WIFI_STA);
		WiFi.disconnect();
		delay(100);

		IPAddress localIP(192, 168, 1, 1);
		IPAddress gateway(192, 168, 1, 1);
		IPAddress subnet(255, 255, 255, 0);

		WiFi.softAPConfig(localIP, gateway, subnet);
		WiFi.softAP("Specto receiver"); 
	}
	else
	{
		configManager.getSSID(ssid);
		configManager.getPassword(pass);

		WiFi.begin(ssid, pass);
		Serial.print("> Connecting with: ");
		Serial.print(ssid);
		while (WiFi.status() != WL_CONNECTED)
		{
			if (WiFi.status() == WL_CONNECT_FAILED)
			{
				Serial.println("> Unable to connect.");
				break;
			}
			delay(500);
			Serial.print(".");
		}
		if (WiFi.status() == WL_CONNECTED)
			Serial.println("> Connection established.");
	}

	RGBColorController::setColor("000000");
}

void loop() 
{
	serialMessenger.listen();
	udpMessenger.listen();  
	delay(5);
}