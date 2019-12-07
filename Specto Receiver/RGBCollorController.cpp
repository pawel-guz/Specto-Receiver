#include "RGBColorController.h"

static int rgb_buff[3];
static short dec;
static short len;

void RGBColorController::initDiodes()
{
	pinMode(Diode::RED, OUTPUT);
	pinMode(Diode::GREEN, OUTPUT);
	pinMode(Diode::BLUE, OUTPUT);
}

/// Sets color from hex string
void RGBColorController::setColor(const char* s)
{
	len = 0;
	rgb_buff[0] = 0;
	rgb_buff[1] = 0;
	rgb_buff[2] = 0;
	while (*s && len < 6)
	{
		if ((dec = hex2dec(s++)) >= 0)
		{
			rgb_buff[(len) / 2] += dec * ((len) % 2 ? 1 : 16);
			len++;
		}
	}
	setColor(rgb_buff, rgb_buff + 1, rgb_buff + 2);
}

/// Sets color from r, g, b byte values
void RGBColorController::setColor(const int* r, const int* g, const int* b)
{
	setLed(Diode::RED, r);
	setLed(Diode::GREEN, g);
	setLed(Diode::BLUE, b);
}


void RGBColorController::setLed(Diode color, const int* value)
{
	analogWrite(color, *value * 4);
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
