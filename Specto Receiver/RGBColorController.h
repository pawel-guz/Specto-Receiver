#pragma once
#include <ESP8266WiFi.h> 
#include "Pinouts.h"

class RGBColorController
{
public:
	enum Diode
	{
		RED = RED_CHANNEL_PIN,
		GREEN = GREEN_CHANNEL_PIN,
		BLUE = BLUE_CHANNEL_PIN
	};
	enum Wave
	{
		Triangular,
		Sin,
		AbsSin,
	};

	static void test();
	static void blink(const char* hexColor, int blinkTimeMs, int wavePeriod, Wave waveType, int repeats, int intervalMs);
	static void shortNotifyBlink(const char* hexColor);
	static void longNotifyBlink(const char* hexColor); 
	static void awaitBlinkInterrupted(const char* hexColor, int t);
	static void setColor(const char* hexColor);	
	static void setColor(const char* hexColor, float intensity);
	static void setColor(const int* r, const int* g, const int* b);
	static void initDiodes();


private:
	static void blinkBit(const char* hexColor, int halfWavePeriod, float invWavePeriod, Wave waveType, int t);
	static int hex2dec(const char* c);
	static void setLed(Diode color, const int* value);
};