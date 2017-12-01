/*
 Name:		ArtikClient.cpp
 Created:	11/4/2017 7:28:55 PM
 Author:	Gabriel Roldán <luisgabrielroldan@gmail.com>
*/

#include "ArtikClient.h"
#include <functional>

using namespace std::placeholders;

ArtikClient::ArtikClient() {
	_actionsCallback = NULL;
}

void ArtikClient::clientCallbackHandler(char * topic, byte * payload, unsigned int length)
{
	ARTIK_DEBUG("New msg [%s]: %s\n", topic, payload);
	if (strncmp("/v1.1/actions", topic, 13) == 0 && _actionsCallback) {
		_actionsCallback(payload, length);
	}
}

void ArtikClient::reconnect()
{
	while (!_client.connected()) {
		ARTIK_DEBUG("Connecting to Artik...\n");
		if (_client.connect("", _deviceId.c_str(), _deviceToken.c_str())) {
			ARTIK_DEBUG("Connected to Artik.\n");
			suscribeTo("actions");
			suscribeTo("errors");
		}
	}
}

bool ArtikClient::connected()
{
	return _client.connected();
}

void ArtikClient::publish(const char * topic, const char * payload)
{
	char topicPath[50];
	sprintf(topicPath, "/v1.1/%s/%s", topic, _deviceId.c_str());
	_client.publish(topicPath, payload);
	ARTIK_DEBUG("Publish [%s]: %s\n", topicPath, payload);
}

void ArtikClient::publishMessage(const char * payload)
{
	publish("messages", payload);
}

void ArtikClient::setActionsCallback(ACTIONS_CALLBACK_SIGNATURE actionsCallback)
{
	_actionsCallback = actionsCallback;
}

void ArtikClient::suscribeTo(const char * topic)
{
	char topicPath[50];
	sprintf(topicPath, "/v1.1/%s/%s", topic, _deviceId.c_str());
	_client.subscribe(topicPath);
	ARTIK_DEBUG("Suscribed to: %s\n", topicPath);
}

void ArtikClient::begin(const char * deviceId, const char * deviceToken)
{
	_deviceId = deviceId;
	_deviceToken = deviceToken;

	_client.setClient(_wifiClient);
	_client.setServer(ARTIKCLOUD_API_SERVER, ARTIKCLOUD_API_PORT);
	_client.setCallback(std::bind(&ArtikClient::clientCallbackHandler, this, _1, _2, _3));
}

void ArtikClient::loop(void)
{
	_client.loop();
}
