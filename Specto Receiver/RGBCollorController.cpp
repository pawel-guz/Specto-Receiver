#include "RGBColorController.h"

static int rgb_buff[3];
static short dec;
static short len;

void RGBColorController::initDiodes()
{
	pinMode(Diode::RED, OUTPUT);
	pinMode(Diode::GREEN, OUTPUT);
	pinMode(Diode::BLUE, OUTPUT);
	setColor("000000"); 
}

void RGBColorController::test()
{ 
	RGBColorController::blink("ff0000", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
	RGBColorController::blink("ffff00", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
	RGBColorController::blink("00ff00", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
	RGBColorController::blink("00ffff", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
	RGBColorController::blink("0000ff", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
	RGBColorController::blink("ff00ff", 1000, 1000, RGBColorController::Wave::AbsSin, 1, 0);
}

void RGBColorController::blink(const char* hexColor, int blinkTimeMs, int wavePeriod, Wave waveType, int repeats, int intervalMs)
{    
	int i = 0; 
	int halfWave = wavePeriod / 2;
	float invWave = 1 / (float)wavePeriod;
	float invHalfWave = 1 / (float)halfWave;
	if (repeats <= 1)
		intervalMs = 0;

	setColor("000000");

	while (i < repeats)
	{
		float t0 = millis();
		int t = 0;

		while (t < blinkTimeMs)
		{    
			blinkBit(hexColor, halfWave, invWave, waveType, t);
			t = (millis() - t0);
		} 
		setColor("000000");
		delay(intervalMs); 
		i++;
	}
}

void RGBColorController::blinkBit(const char* hexColor, int halfWavePeriod, float invWavePeriod, Wave waveType, int t)
{
	float intensity = 0;
	switch (waveType)
	{
		case Wave::Sin:			intensity = sinf(t * 2 * PI * invWavePeriod); break;
		case Wave::AbsSin:		intensity = fabsf(sinf(t * 2 * PI * invWavePeriod)); break;
		case Wave::Triangular:	intensity = 1 - (float)abs(t % (halfWavePeriod * 2) - halfWavePeriod) * invWavePeriod * 0.5; break;
		default:
			break;
	}
	if (intensity < 0)
		intensity = 0;

	setColor(hexColor, intensity);
}

void RGBColorController::shortNotifyBlink(const char* hexColor)
{
	blink(hexColor, 500, 1000, RGBColorController::Wave::AbsSin, 2, 0);
}

void RGBColorController::longNotifyBlink(const char* hexColor)
{
	blink(hexColor, 1000, 2000, RGBColorController::Wave::AbsSin, 1, 0);
}

void RGBColorController::awaitBlinkInterrupted(const char* hexColor, int t)
{ 
	blinkBit(hexColor, 1000, 0.0005, RGBColorController::Wave::AbsSin, t);
}

/// Sets color from hex string
void RGBColorController::setColor(const char* hexColor)
{
	len = 0;
	rgb_buff[0] = 0;
	rgb_buff[1] = 0;
	rgb_buff[2] = 0;
	while (*hexColor && len < 6)
	{
		if ((dec = hex2dec(hexColor++)) >= 0)
		{
			rgb_buff[(len) / 2] += dec * ((len) % 2 ? 1 : 16);
			len++;
		}
	} 

	setColor(rgb_buff, (rgb_buff + 1), (rgb_buff + 2));
}


void RGBColorController::setColor(const char* hexColor, float intensity)
{
	len = 0;
	rgb_buff[0] = 0;
	rgb_buff[1] = 0;
	rgb_buff[2] = 0;
	while (*hexColor && len < 6)
	{
		dec = hex2dec(hexColor++);
		if (dec >= 0)
		{
			rgb_buff[(len) / 2] += dec * ((len) % 2 ? 1 : 16);
			len++;
		}
	} 

	if (intensity != 1)
	{
		for (int i = 0; i < 3; i++) 
			rgb_buff[i] = (int)(intensity * (float)rgb_buff[i]);  
	}

	setColor(rgb_buff, (rgb_buff + 1),  (rgb_buff + 2));
}

/// Sets color from r, g, b byte values
void RGBColorController::setColor(const int* r, const int* g, const int* b)
{
	setLed(Diode::RED, r);
	setLed(Diode::GREEN, g);
	setLed(Diode::BLUE, b);
}
 
int RGBColorController::hex2dec(const char* c)
{
	short h = 0;

	if (*c >= 'a' && *c <= 'f')
		h = *c - 'a' + 10;
	else if (*c >= 'A' && *c <= 'F')
		h = *c - 'A' + 10;
	else if (*c >= '0' && *c <= '9')
		h = *c - '0';
	else h = -1;

	return h;
}

void RGBColorController::setLed(Diode color, const int* value)
{
	analogWrite(color, *value * 4);
}