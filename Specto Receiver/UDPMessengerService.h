#pragma once
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "ConfigManager.h" 
 
class UDPMessengerService
{
private:
	static const int MAX_PACKET_SIZE = 512;
	const int MIN_PACKET_INTERVAL_MS = 5;
	unsigned long last = 0;
	ConfigManager configManager = ConfigManager::getInstance(); 
	WiFiUDP udp;

	void processMessage(IPAddress senderIP, uint16_t senderPort, char *message);
	void sendNetworksList(IPAddress ip, uint16_t port);
	void sendPacket(IPAddress ip, uint16_t port, const char* content);
	void getDeviceInfo(JsonObject &result);
public:
	UDPMessengerService(uint16_t port);
	int listen();
};

