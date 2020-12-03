#ifndef MQTT_TOOLS_H_
#define MQTT_TOOLS_H_
#include "MQTTClient.h"
#include "string.h"
class MQTT
{
public:	
	MQTT(char* adress,char* clientid);
	MQTT();
	int Mqtt_init();
	int Mqtt_SengMessage(const char* payload,int len,char* topic,int qos);
	int Mqtt_subscribe(char* TOPIC, int QOS);
//	MQTTClient get_client();
	~MQTT();

private:
	MQTTClient client;
 	MQTTClient_connectOptions conn_opts;
	MQTTClient_message pubmsg;
	MQTTClient_deliveryToken token;
	char address[20];
	char clientid[20];
	int rc;
};
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
/*
//volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlost(void *context, char *cause);
int Mqtt_SengMessage(const char* payload,int len,char* address,char* clientid,char* topic,int qos);
int Mqtt_subscribe(char* CLIENTID, char* ADDRESS,char* TOPIC, int QOS);
*/
#endif
