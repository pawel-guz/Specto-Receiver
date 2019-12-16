#pragma once 
#include <ArduinoJson.hpp>
#include <ArduinoJson.h> 

class ConfigManager
{
private:
	char *ssid;
	char *pass;  
	char *name;
	ConfigManager();
	static const int RESET_PRESS_MS = 5000;
	int pressStartTime = 0;
public:
	~ConfigManager();
	static ConfigManager& getInstance();
	bool load();
	bool save();
	bool reset();
	bool isDeviceConfigured();
	void setName(const char* ssid);
	void getName(char* buffer);
	void getSSID(char *buf);
	void setSSID(const char *ssid);
	void getPassword(char *buf);
	void setPassword(const char *password);
	void checkForReset(); 
};

