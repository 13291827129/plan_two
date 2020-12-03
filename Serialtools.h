#ifndef SERIALTOOLS_H_
#define SERIALTOOLS_H_
#include<stdbool.h>
int serial_init(char* DEV_NAME);
bool serial_write(int fd, char* buf,int length);
int serial_recive(int fd,char* buf);
int set_serial(int fd, int nSpeed, int nBits, char nEvent, int nStop);
//int length;
#endif
