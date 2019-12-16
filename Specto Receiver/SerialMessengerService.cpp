#include "SerialMessengerService.h" 

SerialMessengerService::SerialMessengerService()
{
}

SerialMessengerService::~SerialMessengerService()
{
}

int SerialMessengerService::listen()
{
	while (Serial.available())
	{
		data = (byte)Serial.read();

		// process signal
		if (data == SERIAL_SIGNAL)
		{
			if (state == AWAIT)
				Serial.write(SERIAL_SIGNAL); // basic handshake
			else
				state = AWAIT;
		}
		else
		{
			// process color transmission
			switch (state) {
			case AWAIT:
			case RED:
				red = data;
				state = GREEN;
				break;
			case GREEN:
				green = data;
				state = BLUE;
				break;
			case BLUE:
				blue = data;
				state = END;
				RGBColorController::setColor(&red, &green, &blue);
				return 1;
				break;
			default:
				state = END;
				return 0;
				break;
			}
		}
	}
	return 0;
}
