#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//include <asm/termios.h>
#include<termios.h>
#include<stdbool.h>
#include<string.h>
#include "Serialtools.h"
//串口初始化
int serial_init(char* DEV_NAME)
{ 
	int fd;
	fd = open(DEV_NAME, O_RDWR);
//	fd = open(DEV_NAME, O_RDWR | O_NOCTTY);
	if(fd < 0)
 	{
		perror(DEV_NAME);
		return -1; 
	} 
	return fd;
}
//设置串口配置
int set_serial(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{ 
	struct termios newtio, oldtio;
    	/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    	if (tcgetattr(fd, &oldtio) != 0)
    	{
       		perror("SetupSerial 1");
        	return -1;
    	}
    	bzero(&newtio, sizeof(newtio));
 	/*步骤一，设置字符大小*/
    	newtio.c_cflag |= CLOCAL | CREAD;
    	newtio.c_cflag &= ~CSIZE;
 	/*设置停止位*/
    	switch (nBits)
    	{
        	case 7:
            	newtio.c_cflag |= CS7;
            	break;
        	case 8:
            	newtio.c_cflag |= CS8;
            	break;
    	}
 
    	/*设置奇偶校验位*/
    	switch (nEvent)
    	{
       		case 'O':    //奇数
            	newtio.c_cflag |= PARENB;
            	newtio.c_cflag |= PARODD;
            	newtio.c_iflag |= (INPCK | ISTRIP);
            	break;
        	case 'E':    //偶数
            	newtio.c_iflag |= (INPCK | ISTRIP);
            	newtio.c_cflag |= PARENB;
            	newtio.c_cflag &= ~PARODD;
            	break;
        	case 'N':    //无奇偶校验位
            	newtio.c_cflag &= ~PARENB;
            	break;
    	}
 	/*设置波特率*/
    	switch (nSpeed)
    	{
       		case 2400:
            	cfsetispeed(&newtio, B2400);
            	cfsetospeed(&newtio, B2400);
            	break;
        	case 4800:
            	cfsetispeed(&newtio, B4800);
            	cfsetospeed(&newtio, B4800);
            	break;
        	case 9600:
            	cfsetispeed(&newtio, B9600);
            	cfsetospeed(&newtio, B9600);
            	break;
        	case 115200:
            	cfsetispeed(&newtio, B115200);
            	cfsetospeed(&newtio, B115200);
            	break;
        	case 460800:
            	cfsetispeed(&newtio, B460800);
            	cfsetospeed(&newtio, B460800);
            	break;
        	default:
            	cfsetispeed(&newtio, B9600);
            	cfsetospeed(&newtio, B9600);
            	break;
    	}
 
    	/*设置停止位*/
    	if (nStop == 1)
    	{
       		newtio.c_cflag &= ~CSTOPB;
    	}
    	else if (nStop == 2)
    	{
        	newtio.c_cflag |= CSTOPB;
	}
	/*设置等待时间和最小接收字符*/
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN]  = 0;
     
	/*处理未接收字符*/
	tcflush(fd, TCIFLUSH); 
	/*激活新配置*/
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}
	printf("set done!\n");
	return 0;
}
//串口写函数
bool serial_write(int fd, char* buf,int length)
{
	int len;
	len = write(fd, buf, length);
	printf("writr:%d\n",length);
	if (len < 0)
	{
		printf("write data error\n"); 
		return false;
	}
	return true;
}
//串口读函数
int  serial_recive(int fd,char* buf)
{
	int ret;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	ret = select(fd+1, &fds, NULL, NULL, NULL);
	if(ret < 0 )
	{
	       return -1;	
	}
	int length = read(fd, buf, 1024);
	printf("readret = %d\n",length);
	if (ret <= 0)
	{
		printf("read error\n"); 
		return -1;
	}
	return length;
} 


 

