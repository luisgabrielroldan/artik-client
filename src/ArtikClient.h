/*
 Name:		ArtikClient.h
 Created:	11/4/2017 7:28:55 PM
 Author:	Gabriel Roldán <luisgabrielroldan@gmail.com>
*/

#ifndef ARTIK_CLIENT_H
#define ARTIK_CLIENT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ARTIKCLOUD_API_SERVER		"api.artik.cloud"
#define ARTIKCLOUD_API_PORT			8883

#ifdef DEBUG_ARTIK
#ifdef DEBUG_ESP_PORT
#define ARTIK_DEBUG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define ARTIK_DEBUG(...) Serial.printf( __VA_ARGS__ )
#endif
#endif

#ifndef ARTIK_DEBUG
#define ARTIK_DEBUG(...)
#define NODEBUG_ARTIK
#endif

#define ACTIONS_CALLBACK_SIGNATURE std::function<void(uint8_t*, unsigned int)>

class ArtikClient {
protected:
	WiFiClientSecure _wifiClient;
	PubSubClient _client;
	String _deviceId;
	String _deviceToken;
	ACTIONS_CALLBACK_SIGNATURE _actionsCallback;
	void clientCallbackHandler(char* topic, byte* payload, unsigned int length);
	void suscribeTo(const char * topic);
public:
	ArtikClient(void);
	void begin(const char *deviceId, const char *deviceToken);
	void loop(void);
	void reconnect();
	bool connected();
	void publish(const char *topic, const char *payload);
	void publishMessage(const char *payload);
	void setActionsCallback(ACTIONS_CALLBACK_SIGNATURE actionsCallback);
};

#endif

