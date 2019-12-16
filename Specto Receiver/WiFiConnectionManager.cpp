#pragma once
#include "WiFiConnectionManager.h" 
#include "RGBColorController.h"
#include "ConfigManager.h"
#include "Colors.h" 

WiFiConnectionManager::WiFiConnectionManager()
{
}

WiFiConnectionManager::~WiFiConnectionManager()
{
}

bool  WiFiConnectionManager::inNetwork = false;

void WiFiConnectionManager::reconnect(ConfigManager& configManager, bool forceAP)
{ 
	char ssid[64] = "";
	char pass[64] = "";  
	char name[64] = "";
	configManager.getName(name);

	if (forceAP || !configManager.load() || !configManager.isDeviceConfigured())
	{
		Serial.println("[WIFIMANAGER]\t Setting AP");
		WiFi.disconnect();
		WiFi.mode(WIFI_AP);
		delay(100);

		IPAddress localIP(192, 168, 1, 1);
		IPAddress gateway(192, 168, 1, 1);
		IPAddress subnet(255, 255, 255, 0);

		WiFi.softAPConfig(localIP, gateway, subnet);
		WiFi.softAP(name); 
		RGBColorController::shortNotifyBlink(COL_WIFI_AP);
		inNetwork = false;
	}
	else
	{
		Serial.println("[WIFIMANAGER]\t Trying WiFi connection...");
		configManager.getSSID(ssid);
		configManager.getPassword(pass);

		WiFi.disconnect();
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, pass);
		Serial.print("[WIFIMANAGER]\t Connecting with: ");
		Serial.print(ssid);
		
		float t = 0, t0 = millis();
		while (WiFi.status() != WL_CONNECTED && t < CONNECTION_TIMEOUT_MS)
		{
			if (WiFi.status() == WL_CONNECT_FAILED) 
				break; 

			RGBColorController::awaitBlinkInterrupted(COL_AWAIT, t);  
			t = millis() - t0;
			yield();
		}

		Serial.println();
		if (WiFi.status() == WL_CONNECTED)
		{
			Serial.println("[WIFIMANAGER]\t Connection established."); 
			RGBColorController::shortNotifyBlink(COL_WIFI_OK);
			inNetwork = true;

		}
		else
		{
			Serial.println("[WIFIMANAGER]\t Unable to connect.");
			RGBColorController::shortNotifyBlink(COL_WIFI_ERROR);
			if(configManager.reset())
				reconnect(configManager, true);
		}
	}
	return;
} 

bool WiFiConnectionManager::isInNetwork()
{
	return inNetwork;
}