#pragma once

class ConfigManager
{
private:
	char *ssid;
	char *pass;
	ConfigManager();
	static const int RESET_BUTTON = 2;
	static const int RESET_PRESS_MS = 5000;
	int pressStartTime = 0;
public:
	~ConfigManager();
	static ConfigManager& getInstance();
	bool load();
	bool save();
	bool reset();
	bool isDeviceConfigured();
	void getSSID(char *buf);
	void setSSID(const char *ssid);
	void getPassword(char *buf);
	void setPassword(const char *password);
	void checkForReset();

};

