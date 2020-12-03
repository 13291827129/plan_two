#include"Analyase_tools.h"
#include"Serialtools.h"
#include<stdio.h>
#include<string.h>
#define headr 0xAA
#define checkStartIndex 2
//异或检查
int checkSum(char* addr,int len,int length)
{	
	
	int leng = Analyase_num(addr,length,len);
	int ret = 0, i = 0;
	for(i = checkStartIndex+len; i < leng-3; i++)
	{	
		ret ^= (addr[i]&0xff);
	}
	return ret;
} 

//数据长度计算，从len开始第一条数据的长度，方便下面的解析
int Analyase_num(char* data, int length, int len)
{	 	
	if(headr != (data[0+len]&0xff) || headr != (data[1+len]&0xff))
	{	
		printf("%.2x %.2xdata error!!!!\n", data[0+len], data[1+len]);
		return -1;	
		
	}
	int count = data[2+len];
	int length_data = 12+len;
	int num = 0;
	while(count)
	{
		num = data[++length_data];
		length_data += num+2;
		count--;
	}
	length_data += 2;
//	printf("length_data == %d,length == %d\n", length_data, length);
	return length_data;                       //返回数据长度
}
//分析数据函数
int Analyase_data(char* data, int length, struct Exchange_data* exchange_value, int len)
{ 	
		
	if(headr != (data[0 + len]&0xff) || headr != (data[1 + len]&0xff))
	{	
		printf("data error!!!!\n");
		return -1;
	}	
	int ret = checkSum(data, len, length);
	int leng = Analyase_num(data, length, len);
	printf("leng = %d\n", leng);
	if(ret != (data[leng-3]&0xff))
	{	
		printf("data[length - 3 + len] = %d\n", data[leng-3]);
		printf("checkSum error!!!\n"); 
		return -1;
	}
	int count = data[2+len];
	int length_data = 12;
	char buf[12+len] = {};
	sprintf(buf,"%x%x%x%x%x%x", data[3+len]&0xff, data[4+len]&0xff, data[5+len]&0xff, data[6+len]&0xff, data[7+len]&0xff, data[8+len]&0xff);      //将MAC地址从16进制子面值改成字符串
	printf("buf :%s\n", buf);
	strcpy(exchange_value->macaddr, buf);	
	while(count)
	{
		if ((data[length_data+len] & 0XFF) == TEMPERATURE)
		{	
			ret = data[++length_data+len];
			printf("temp:%.2x%.2x\n", data[length_data+1+len]&0xff, data[length_data+2+len]&0xff);
			exchange_value->temperature = (((data[length_data+1+len])*256) + (data[length_data+2+len]&0xff))/100;
			length_data += ret+2;
		}
		 else if ((data[length_data+len] & 0XFF) == HUMIDITY)
		{
			ret = data[++length_data+len];
			printf("HUMIDITY:%.2x\n", data[length_data+1+len]);
			exchange_value->humidity = data[length_data+1+len];
			length_data += ret+2;
		}
		else if ((data[length_data+len] & 0xFF) == VIBRATE)
		{
			ret = data[++length_data+len];
			printf("VIBRATE_x:%.2x VIBRATE_y:%.2x VIBRATE_z:%.2x\n", data[length_data+1+len]&0xff, data[length_data+2+len]&0xff, data[length_data+3+len]&0xff); 
			exchange_value->vibrate_x = data[length_data+1+len]&0xff;
			exchange_value->vibrate_y = data[length_data+2+len]&0xff;
			exchange_value->vibrate_z = data[length_data+3+len]&0xff;
			length_data += ret+2;
		}
		else if ((data[length_data+len] & 0XFF) == VOICE)
		{
			ret = data[++length_data+len];
			printf("VOICE:%.2x\n",data[length_data+1+len]&0xff);
			exchange_value->voice = data[length_data+1+len]&0xff;
			length_data+= ret+2;
		}
		else if ((data[length_data+len] & 0xFF) == ELECTRIC)	
		{
			ret = data[++length_data+len];
                        printf("ELECTRIC:%.2x\n",data[length_data+1+len]&0xff);
                        exchange_value->electric = data[length_data+1+len]&0xff;
                        length_data+= ret+2;
		}	
		else if ((data[length_data+len] & 0xFF) == PRESSURE)
		{ 
			ret = data[++length_data+len];
		//	printf("PRESSURE:%.2x%.2x%.2x%.2x %d\n",data[length_data+1], data[length_data+2]&0xff, data[length_data+3], data[length_data+4],sizeof(float));
		//	exchange_value->pressure = (((data[length_data+1]&0xff)*16777216) + ((data[length_data+2]&0xff)*65536) + ((data[length_data+3]&0xff)*256) + (data[length_data+4]&0xff))/1000;
		//	printf("exchange_value->pressure:%f\n",exchange_value->pressure);
			length_data += ret+2;
		}
		else
		{
			ret = data[++length_data+len];
			length_data += ret+2;
		}
/*		else if ((data[length_data+len] & 0xFF) == LINGHT)
		{	
			int ret = data[++length_data+len];
			length_data += ret+2;
		}
		else if ((data[length_data+len] & 0xFF) == ELECTRIC)
		{
			int ret = data[++length_data+len];
			length_data += ret+2;
		}
*/
		count--;
	}
	return length_data;		
}
