#include<stdio.h>
#include<iostream>
#include<json/json.h>
#include<stdbool.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>
#include<string.h>
#include"Serialtools.h"
#include"Analyase_tools.h"
#include"Mqtt_tools.h"
//#include<wiringPi.h>
#include<signal.h>
#include<sys/time.h>
using namespace std;
#define ADDRESS     "127.0.0.1:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC        "sensor"
#define PAYLOAD      "sensor"
#define QOS          1
#define TIMEOUT      10000L
#define TOPIC_FLAG   "state"     
#define original     0xffffffff
const char* message1 = "thread1";
const char* message2 = "thread2";
const char* message3 = "thread3";
const char* message4 = "thread4";
const char* message5 = "thread5";
const char* message6 = "thread6";
const char* message7 = "thread7";
const char* message8 = "thread8";
const char* USBSERIAL ="/dev/ttyUSB0";	
int fd;
char flag_buf[256] = {};
int number = 1;
pthread_mutex_t mutex_t;
pthread_mutex_t mutex;
MQTT Client(ADDRESS, CLIENTID);	
void sig_alarm_handle(int sig_num);
void*  Recive_And_Send(void *ptr)
{ 	
	char buf[1024] = {};
	int len = 0;
	time_t timep;
	struct tm *p;
	int length;
	char timebuf[256] = {};
	int ret = 0;
	while(1)
	{ 	
		len = 0;
		memset(buf, 0, 1024);
		memset(timebuf, 0, 256);
		tm *p = NULL;
		time(&timep);
		p = localtime(&timep); //取得当地具体时间
		sprintf(timebuf,"%d-%d-%d %d:%d:%d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec );
	//       if(pthread_mutex_lock(&mutex_t) == 0)
	//	{
	//	       printf("%s lock succeed\n", (char*)ptr);	
	//	}
		length = serial_recive(fd, buf);
	//	if(pthread_mutex_unlock(&mutex_t) == 0)
	//	{
	//	       printf("%s unlock succeed\n", (char*)ptr);	
	//	}
		if(false  == length)
		{	
			printf("Serial read error!\n");
			return NULL;
		}
		for(int i = 0;i <length; i++)
		{
			printf("%.2x", buf[i]&0xff);
		}	
		while(len < length)
		{	
			Json::Value value;
			value["time"] = timebuf;
			struct Exchange_data exchange_value = {exchange_value.temperature = original,exchange_value.humidity = original, exchange_value.vibrate_x = original, exchange_value.vibrate_y = original, exchange_value.vibrate_z = original, exchange_value.pressure = original, exchange_value.electric = original, exchange_value.voice = original};
			ret = Analyase_data(buf, length, &exchange_value, len);
			printf("ret ==== %d\n",ret);
			if(ret == -1)
			{	
				break;
	//			continue; 
			}
			if (0 != strcmp("", exchange_value.macaddr))
			{		
				value["id"] = exchange_value.macaddr;
			}
			if (exchange_value.temperature != original)
			{
				value["temp"] = exchange_value.temperature;
			}
			if (exchange_value.humidity != original)
			{
				value["hum"] = exchange_value.humidity;
			}
			if (exchange_value.vibrate_x != original|| exchange_value.vibrate_y != original|| exchange_value.vibrate_z != original)
			{
				value["xx"] = (signed char)exchange_value.vibrate_x;
				value["yy"] = (signed char)exchange_value.vibrate_y;
				value["zz"] = (signed char)exchange_value.vibrate_z;
			}
			if(exchange_value.pressure != original)
			{
				value["pressure"] = exchange_value.pressure;
			}
			if(exchange_value.voice != original)
                        {
                                value["voice"] = exchange_value.voice;
                        }	
			if(exchange_value.electric != original)
                        {
                                value["electric"] = exchange_value.electric;
                        }	
			std::string strOut = value.toStyledString();
	       		pthread_mutex_lock(&mutex);
		//	ret = Mqtt_SengMessage(strOut.c_str(), strOut.size(), ADDRESS, (char*)ptr, TOPIC, QOS);
			ret = Client.Mqtt_SengMessage(strOut.c_str(), strOut.size(), TOPIC, QOS);
			pthread_mutex_unlock(&mutex);
			usleep(10);
			len = Analyase_num(buf, length, len);
		}
	}
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	char* payloadptr;
	int state;
	char  buf[10];
	printf("Message arrived\n");
    	printf("message: %s\n", message->payload);
 //  	printf("   message: \n");
	if (0 == strcmp("reversal",(char*) message->payload))
	{
//		if (digitalRead(1) == 0)
		{
//			digitalWrite(1, HIGH);
//			state = digitalRead(1);
		}
//		else
		{
//			digitalWrite(1, LOW);
//			state = digitalRead(1);	
		}
		if (state)
		{
	        	sprintf(buf,"%s", "true");

		}
		else
		{	
	       		sprintf(buf, "%s", "false");	
		}
		Client.Mqtt_SengMessage(buf, strlen(buf),TOPIC_FLAG, QOS);		
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
}
int main()
{	
	int ret = 0;
	pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
	fd = serial_init(USBSERIAL);	
	Client.Mqtt_init();
//	MQTTClient_setCallbacks(Client.client, NULL, NULL, msgarrvd, NULL);
//	if(Client.Mqtt_subscribe(TOPIC, QOS) != 0)
//	{
//		printf("subscribr error!!!\n");
//		return -1;
//	}	
//	MQTTClient* client = Client.get_client();
//	MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL);
//	wiringPiSetup();
//	pinMode(1, OUTPUT);
//      pinMode(25, OUTPUT);
//      digitalWrite(25, HIGH);
//	signal(SIGALRM, sig_alarm_handle);
//	sprintf(flag_buf,"%s","reversal");
//	struct itimerval olditv;
//      struct itimerval itv;
//      itv.it_interval.tv_sec = 0;
//      itv.it_interval.tv_usec = 20000;
//      itv.it_value.tv_sec = 0;
//      itv.it_value.tv_usec = 500;
//      setitimer(ITIMER_REAL, &itv, &olditv);
	pthread_mutex_init(&mutex_t, NULL);
	pthread_mutex_init(&mutex, NULL);
	if(fd < 0)
	{
	       printf("serial_init error!!!\n");
	       return -1;	       
	}
	set_serial(fd,115200, 8, 'N', 1);
	ret = pthread_create(&thread1, NULL, &Recive_And_Send, message1);
	if (ret != 0)
	{
	       printf("线程1创建失败\n");	
	}
	else
	{
	       printf("线程1创建成功\n");	
	}
	ret = pthread_create(&thread2, NULL, &Recive_And_Send, message2);
	if (ret != 0)
	{
	       printf("线程2创建失败\n");	
	}
	else
	{
	       printf("线程2创建成功\n");	
	}
	
	ret = pthread_create(&thread3, NULL, &Recive_And_Send, message3);
	if (ret != 0)
	{
	       printf("线程3创建失败\n");	
	}
	else
	{
	       printf("线程3创建成功\n");	
	}
	ret = pthread_create(&thread4, NULL, &Recive_And_Send, message4);
	if (ret != 0)
	{
	       printf("线程4创建失败\n");	
	}
	else
	{
	       printf("线程4创建成功\n");	
	}
	ret = pthread_create(&thread5, NULL, &Recive_And_Send, message5);
	if (ret != 0)
	{
	       printf("线程5创建失败\n");	
	}
	else
	{
	       printf("线程5创建成功\n");	
	}
	
	ret = pthread_create(&thread6, NULL, &Recive_And_Send, message6);
	if (ret != 0)
	{
	       printf("线程6创建失败\n");	
	}
	else
	{
	       printf("线程6创建成功\n");	
	}
	ret = pthread_create(&thread7, NULL, &Recive_And_Send, message7);
	if (ret != 0)
	{
	       printf("线程7创建失败\n");	
	}
	else
	{
	       printf("线程7创建成功\n");	
	}
	
	ret = pthread_create(&thread8, NULL, &Recive_And_Send, message8);
	if (ret != 0)
	{
	       printf("线程8创建失败\n");	
	}
	else
	{
	       printf("线程8创建成功\n");	
	}

	while(1);
}	
void sig_alarm_handle(int sig_num)
{
	if (sig_num == SIGALRM)
	{
//		if (digitalRead(23 != number)
		{
			usleep(20000);
//			if (digitalRead(23) != number)
			{
				Client.Mqtt_SengMessage(flag_buf, strlen(flag_buf)+1, "hum", 0);
//				Mqtt_SengMessage(buf, strlen(buf),ADDRESS,CLIENTID,TOPIC,QOS);  
                                printf("send reversal succeed!!\n");
//                              number = digitalRead(23);

			}
		}
	}
//	struct itimerval olditv;
//      struct itimerval itv;
//      itv.it_interval.tv_sec = 0;
//      itv.it_interval.tv_usec = 20000;
//      itv.it_value.tv_sec = 0;
//      itv.it_value.tv_usec = 500;
//      setitimer(ITIMER_REAL, &itv, &olditv);
 
}
