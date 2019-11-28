#include "UDPMessengerService.h"
#include <sstream>
UDPMessengerService::UDPMessengerService(uint16_t port)
{
	udp.begin(port);
}

int UDPMessengerService::listen()
{
	int packetSize = udp.parsePacket();
	if (packetSize)
	{
		last = millis();
		char incomingPacket[MAX_PACKET_SIZE];
		int len = udp.read(incomingPacket, MAX_PACKET_SIZE);
		if (len > 0)
			incomingPacket[len] = 0;

		Serial.write(millis() - last);
		//if (millis() - lastPacketArrivalTime > MIN_PACKET_INTERVAL_MS)
		//{
			processMessage(udp.remoteIP(), udp.remotePort(), incomingPacket);
		//}
	}
	return packetSize;
}

void UDPMessengerService::processMessage(IPAddress senderIP, uint16_t senderPort, char *message)
{
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject &root = jsonBuffer.parseObject(message);
	if (!root.success())
		return;

	// quick non-delaying color command handling
	const char *color = root["col"];
	if (color)
	{
		RGBColorController::setColor(color);
		return;
	}

	// other commands
	const char *cmd = root["cmd"];
	JsonObject &params = root["params"];
	JsonObject &reply = jsonBuffer.createObject();
	char replyMessage[MAX_PACKET_SIZE] = "";
	bool sendReply = true;

	if (!strcmp(cmd, "getDeviceInfo"))
	{
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &result = jsonBuffer.createObject();

		getDeviceInfo(result);
		reply["type"] = "deviceInfo";
		reply["result"] = result;
	}
	else if (!strcmp(cmd, "fetchNetworks"))
	{
		sendNetworksList(senderIP, senderPort);
		return;
	}
	else if (!strcmp(cmd, "doReboot"))
	{
		WiFi.mode(WIFI_STA);
		WiFi.disconnect();
		ESP.reset();
		return;
	}
	else if (!strcmp(cmd, "setWiFi"))
	{
		if (!params.success())
			return;

		if (!params.containsKey("ssid"))
			return;

		const char *ssid = params.get<const char*>("ssid");
		const char *pass = "";

		if (params.containsKey("password"))
			pass = params.get<const char*>("password");
		configManager.setPassword(pass);
		configManager.setSSID(ssid);

		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &result = jsonBuffer.createObject(); 

		result["connected"] = configManager.save() ? true : false;
		reply["type"] = "setWiFiFeedback";
		reply["result"] = result;

	}
	else if (!strcmp(cmd, "setColor"))
	{
		if (!params.success())
			return;

		if (!params.containsKey("color"))
			return;

		const char *color = params.get<const char*>("color");
		if (!params.containsKey("expectReply"))
			sendReply = false;
		else
		{
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject &result = jsonBuffer.createObject();

			result["color"] = color;
			reply["type"] = "setColorFeedback";
			reply["result"] = result;
		}

		RGBColorController::setColor(color);
	}
	else 
	{
		reply["type"] = "unknown";
	}

	reply.printTo(replyMessage, MAX_PACKET_SIZE);
	sendPacket(senderIP, senderPort, replyMessage);
}

void UDPMessengerService::sendPacket(IPAddress ip, uint16_t port, const char *content)
{
	udp.beginPacket(ip, port);
	udp.write(content);
	udp.endPacket();
}

void UDPMessengerService::getDeviceInfo(JsonObject &result)
{
	result["serialNumber"] = ESP.getChipId();
}

void UDPMessengerService::sendNetworksList(IPAddress ip, uint16_t port)
{
	int networksCount = WiFi.scanNetworks();

	for (int i = 0; i < networksCount; i++)
	{
		int32_t signalStrenght = WiFi.RSSI();

		if (signalStrenght < -100)
			signalStrenght = 0;
		else if (signalStrenght >= -50)
			signalStrenght = 100;
		else
			signalStrenght = (signalStrenght + 100) * 2;

		StaticJsonBuffer<200> jsonBuffer;
		JsonObject &result = jsonBuffer.createObject();
		JsonObject &network = jsonBuffer.createObject();

		network["SSID"] = WiFi.SSID(i);
		network["signalStrenght"] = signalStrenght;
		network["isProtected"] = (bool)WiFi.encryptionType(i) != ENC_TYPE_NONE;

		result["type"] = "networkData";
		result["result"] = network;

		char resultBuffer[MAX_PACKET_SIZE] = "";
		result.printTo(resultBuffer, MAX_PACKET_SIZE);
		sendPacket(ip, port, resultBuffer);
	}
}



