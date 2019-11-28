#pragma once
#include "FS.h"

class SerialMessengerService
{
private:
	static const int SERIAL_SIGNAL = 255;
	enum State
	{
		AWAIT,
		RED,
		GREEN,
		BLUE,
		END
	};
	int red;
	int green;
	int blue;
	int data;
	State state = END;
public:
	SerialMessengerService();
	~SerialMessengerService();
	int listen();
};

