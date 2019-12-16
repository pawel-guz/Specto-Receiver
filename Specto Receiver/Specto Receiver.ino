#include <SoftwareSerial.h>  
#include "FS.h"
#include "ConfigManager.h"
#include "UDPMessengerService.h"
#include "WiFiConnectionManager.h"
#include "SerialMessengerService.h"
#include "RGBColorController.h"
#include "Pinouts.h"

/*
	Name:		Specto_Receiver.ino 
	https://github.com/pawel-guz/Specto 
	Author:	gp
*/ 


ConfigManager configManager = ConfigManager::getInstance(); 
UDPMessengerService udpMessenger(1234);
SerialMessengerService serialMessenger;

void setup() 
{
	Serial.begin(19200);
	Serial.println("[REBOOT]");

	configManager.load();
	RGBColorController::initDiodes();
	WiFiConnectionManager::reconnect(configManager, false);
}

void loop() 
{
	serialMessenger.listen();
	udpMessenger.listen(); 
	configManager.checkForReset();
} 