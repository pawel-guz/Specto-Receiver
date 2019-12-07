#pragma once
#include <ESP8266WiFi.h>

class RGBColorController
{
public:
	static void setColor(const char* s);
	static void setColor(const int* r, const int* g, const int* b);
	static void initDiodes();
	enum Diode
	{
		RED = 5,
		GREEN = 4,
		BLUE = 0
	};

private:
	static int hex2dec(const char* c);
	static void setLed(Diode color, const int* value);
};