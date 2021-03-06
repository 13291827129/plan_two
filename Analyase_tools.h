#ifndef ANALYSE_TOOLS_H_
#define ANALYSE_TOOLS_H_
#include<stdio.h>
// 进行异或校验函数
int checkSum(char* addr,int len,int length);
enum TYPE
{ 	
	//温度
	TEMPERATURE = 0x01,
	//湿度
	HUMIDITY    = 0x02,
	//震动
	VIBRATE     = 0X71,
	//气压
	PRESSURE    = 0x15,
	//光线
	LINGHT      = 0x22,
	//电量
	ELECTRIC    = 0x73,
	//声音
	VOICE       = 0x5d,
};
struct Exchange_data
{ 	
	char macaddr[256];
	signed int temperature = 0x8000;
	signed int humidity = 0x8000;
	signed char vibrate_x = 0x80;
	signed char vibrate_y = 0x80;
	signed char vibrate_z = 0x80;
	float pressure = 0x8000;
	int voice = 0x8000;
	int electric = 0x8000;
};
int Analyase_data(char* data,int length,struct Exchange_data* exchange_value,int len);
//返回数据长度
int Analyase_num(char* data,int length,int len);

#endif
