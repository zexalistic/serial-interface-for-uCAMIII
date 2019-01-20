#ifndef SERIAL_H
#define SERIAL_H

#include "include.h"

extern int set_speed(int fd, int speed);

extern int set_Parity(int fd,int databits,int stopbits,int parity);

#endif