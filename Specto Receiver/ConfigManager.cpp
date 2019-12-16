#pragma once
#include "ConfigManager.h"   
#include "WiFiConnectionManager.h"
#include "RGBColorController.h" 
#include "Pinouts.h"
#include "FS.h"

ConfigManager::ConfigManager()
{
	SPIFFS.begin();
	this->ssid = new char[64];
	this->pass = new char[64];
	this->name = new char[64];
	pinMode(RESET_CONFIG_BUTTON_PIN, INPUT);
}

ConfigManager::~ConfigManager()
{
	delete[] this->ssid;
	delete[] this->pass;
	delete[] this->name;
}

ConfigManager& ConfigManager::getInstance()
{
	static ConfigManager instance;
	return instance;
}

bool ConfigManager::load()
{
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile)
		return false;

	size_t size = configFile.size();
	char* buffer = new char[size];
	configFile.readBytes(buffer, size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buffer);
	if (!json.success())
	{
		delete buffer;
		return false;
	}
	if (json.containsKey("ssid"))
	{ 
		const char* ssid = json["ssid"];
		strcpy(this->ssid, ssid);
	}
	if (json.containsKey("pass"))
	{
		const char* pass = json["pass"];
		strcpy(this->pass, pass);
	}
	if(json.containsKey("name"))
	{
		const char* name = json["name"];
		strcpy(this->name, name);
	}
	delete buffer;
	configFile.close();
	return true;
}

bool ConfigManager::save()
{
	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile)
		return false;

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["name"] = this->name;
	json["ssid"] = this->ssid;
	json["pass"] = this->pass;
	json.printTo(configFile);

	Serial.println("[CONFIGMANAGER]\t Saving config:"); 	
	Serial.print("Name: ");
	Serial.print(this->name);
	Serial.println();
	Serial.print("SSID: ");
	Serial.print(this->ssid);
	Serial.println();
	Serial.print("PASS: ");
	Serial.println(this->pass);  

	configFile.close();
	return true;
}

bool ConfigManager::reset()
{ 
	this->ssid[0] = '\0';
	this->pass[0] = '\0';
	Serial.println("[CONFIGMANAGER]\t Reseting name, ssid and pass.");
	return save();
}

bool ConfigManager::isDeviceConfigured()
{
	bool result = this->ssid[0] != '\0';
	 
	Serial.print("[CONFIGMANAGER]\t Is device configured? : "); 
	Serial.println(result);

	return result;
}

void ConfigManager::setName(const char* name)
{
	strcpy(this->name, name);
}

void ConfigManager::getName(char* buffer)
{
	if(this->name[0] == '\0')
		strcpy(this->name, "Specto receiver");

	strcpy(buffer, this->name);
}

void ConfigManager::getSSID(char *buffer)
{
	strcpy(buffer, this->ssid);
}

void ConfigManager::setSSID(const char *ssid)
{
	strcpy(this->ssid, ssid);
}

void ConfigManager::getPassword(char *buffer)
{
	strcpy(buffer, this->pass);
}

void ConfigManager::setPassword(const char *password)
{
	strcpy(this->pass, password);
}

void ConfigManager::checkForReset()
{ 
	if (digitalRead(RESET_CONFIG_BUTTON_PIN) == HIGH)
	{ 
		if (pressStartTime == 0)
			pressStartTime = millis();
		else if (millis() - pressStartTime > RESET_PRESS_MS)
		{
			pressStartTime = 0;  
			Serial.println("[RESET BUTTON]");
			if(reset())
				WiFiConnectionManager::reconnect(*this, true);
		}
	}
}
