#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


#define BAUNDRATE B115200
#define UART_DEVICE "/dev/ttyUSB0"

#define FALSE -1
#define TRUE 0

#define min(a,b) (a<b)?a:b

extern	int fd;

#endif
