#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Mqtt_tools.h"
//#include<wiringPi.h>
//MQTTClient client;
/*
volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = (char*)message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}
int Mqtt_SengMessage(const char* payload,int len,char* address,char* clientid,char* topic,int qos)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    MQTTClient_create(&client, address, clientid,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }	
    pubmsg.payload = (char*)payload;
    pubmsg.payloadlen = len;
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    printf("Waiting for publication of %s\n""on topic %s for client with ClientID: %s\n",payload, topic, clientid);
    while(deliveredtoken != token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
int Mqtt_subscribe(char* CLIENTID, char* ADDRESS,char* TOPIC, int QOS)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;
 
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
 
    MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL);
 
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);
//    while(1);	
 
    MQTTClient_unsubscribe(client, TOPIC);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}*/

MQTT::MQTT(char* adress,char* clientid)
{
	strcpy(this->address,adress); 
	strcpy(this->clientid,clientid);
}
MQTT::MQTT(){}
int MQTT::Mqtt_init()
{
	conn_opts = MQTTClient_connectOptions_initializer;
 	pubmsg = MQTTClient_message_initializer;
        MQTTClient_create(&client, address, clientid,MQTTCLIENT_PERSISTENCE_NONE, NULL);	    
	MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL);
	conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
               printf("Failed to connect, return code %d\n", rc);
               exit(EXIT_FAILURE);
        }
	return 0;
		
}
MQTT::~MQTT()
{
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
}

	
int MQTT::Mqtt_SengMessage(const char* payload,int len,char* topic,int qos)
{	
	if (client == NULL)
	{
		printf("connect error!!!\n");
	}	
	pubmsg.payload = (char*)payload;
	pubmsg.payloadlen = len;
	pubmsg.qos = qos;
	pubmsg.retained = 0;
	MQTTClient_publishMessage(client, topic, &pubmsg, &token);
	printf("Waiting for publication of %s\n""on topic %s for client with ClientID: %s\n",payload, topic, clientid);
	return 0;
}


int MQTT::Mqtt_subscribe(char* TOPIC, int QOS)
{	
		
//	MQTTClient_setCallbacks(client, NULL, NULL, nong->msgarrvd, NULL);
	// 订阅主题，参数：句柄、主题、QoS质量
    	if ((rc = MQTTClient_subscribe(client, TOPIC, QOS)) != MQTTCLIENT_SUCCESS)
    	{
    		printf("Failed to subscribe, return code %d\n", rc);
		MQTTClient_destroy(&client);
    		return -1;
    	}
	return 0;
}
/*
MQTTClient* MQTT::get_client()
{
	if( client == NULL)
	{
	        printf("connect error!!\n");
       		return NULL;
 	}	
	return &client;	
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	char* payloadptr;
	int state;
	char  buf[10];
	printf("Message arrived\n");
    	printf("     topic: %s\n", topicName);
   	printf("   message: \n");
	if (digitalRead(1) == 0)
	{
		digitalWrite(1, HIGH);
		state = digitalRead(1);
		sprintf(buf,"%d",state);
	//	Client.Mqtt_SengMessage(buf, strlen(buf)+1,TOPIC_FLAG, QOS);		
	}
	else
	{
		digitalWrite(1, LOW);
		state = digitalRead(1);
		sprintf(buf,"%d",state);
	//	Client.Mqtt_SengMessage(buf, strlen(buf)+1,TOPIC_FLAG, QOS);		
	}
    	payloadptr = (char*)message->payload;
    	for (int i=0; i<message->payloadlen; i++)
    	{
        	putchar(*payloadptr++);
    	}
    	putchar('\n');
    	MQTTClient_freeMessage(&message);
    	MQTTClient_free(topicName);
    	return 1;
}*/
