#include "ConfigManager.h"
#include <ArduinoJson.h>
#include "RGBColorController.h"
#include "FS.h"

ConfigManager::ConfigManager()
{
	SPIFFS.begin();
	this->ssid = new char[64];
	this->pass = new char[64];

	pinMode(RESET_BUTTON, INPUT_PULLUP);
}

ConfigManager::~ConfigManager()
{
	delete[] this->ssid;
	delete[] this->pass;
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
	char *buffer = new char[size];
	configFile.readBytes(buffer, size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buffer);
	if (!json.success())
	{
		delete buffer;
		return false;
	}
	const char* ssid = json["ssid"];
	strcpy(this->ssid, ssid);
	const char* pass = json["pass"];
	strcpy(this->pass, pass);

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
	json["ssid"] = this->ssid;
	json["pass"] = this->pass;
	json.printTo(configFile);

	configFile.close();
	return true;
}

bool ConfigManager::reset()
{
	setSSID("");
	setPassword("");
	return save();
}

bool ConfigManager::isDeviceConfigured()
{
	return this->ssid[0] != '\0';
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
	if (digitalRead(RESET_BUTTON) == LOW)
	{
		if (pressStartTime == 0)
			pressStartTime = millis();
		else if (millis() - pressStartTime > RESET_PRESS_MS)
		{
			pressStartTime = 0;

			RGBColorController::setColor("ff0000");
			delay(100);
			RGBColorController::setColor("000000"); 

			reset();
			ESP.restart();
		}
	}
}
