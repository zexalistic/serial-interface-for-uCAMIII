#include "serial.h"
////////////////////////////////////////////////////////////////////////////////  
/** 
*@brief  设置串口通信速率 
*@param  fd     类型 int  打开串口的文件句柄 
*@param  speed  类型 int  串口速度 
*@return  void 
*/
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
		   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,};
		   
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
		  115200, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

int set_speed(int fd, int speed){
	int	i;
	int	status;
	struct	termios	Opt;
	tcgetattr(fd, &Opt);
	for ( i = 0; i < sizeof(speed_arr) / sizeof(int); i++){
		if (speed == name_arr[i]){
			tcflush(fd,TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0){
				perror("tcsetattr fd1");
				return FALSE;
			} 
			tcflush(fd, TCIOFLUSH);
		}
	} 
	return 0;
}

////////////////////////////////////////////////////////////////////////////////  
/** 
*@brief   设置串口数据位，停止位和效验位 
*@param  fd     类型  int  打开的串口文件句柄 
*@param  databits 类型  int 数据位   取值 为 7 或者8 
*@param  stopbits 类型  int 停止位   取值为 1 或者2 
*@param  parity  类型  int  效验类型 取值为N,E,O,,S 
*/
int set_Parity(int fd,int databits,int stopbits,int parity){
	struct termios options;
	if( tcgetattr(fd, &options) != 0){
		perror("SetupSerial 1");
		return 1;
	}
	options.c_cflag &= ~CSIZE;
	switch (databits){
		case 7:		options.c_cflag |= CS7; break;
		case 8: 	options.c_cflag |= CS8; break;
		default:	fprintf(stderr, "Unsupported data size\n"); return FALSE;
	}
	switch (parity){
		case 'n':
		case 'N':	options.c_cflag &= ~PARENB; options.c_iflag &= ~INPCK; break;
		case 'o':
		case 'O':	options.c_cflag |= (PARODD | PARENB); options.c_iflag |= INPCK; break;
		case 'e':
		case 'E':	options.c_cflag |= PARENB;	/*Enalbe parity*/
				options.c_cflag &= ~PARODD;	/*EVEN CHECK*/
				options.c_iflag |= INPCK;	/*Disable parity check*/
				break;
		case 'S':
		case 's':	options.c_cflag &= ~PARENB; options.c_cflag &= ~CSTOPB; break;
		default:	fprintf(stderr, "Unsupported parity\n"); return FALSE;
	}
	/*设置停止位*/
	switch (stopbits){
		case 1:		options.c_cflag &= ~CSTOPB; break;
		case 2:		options.c_cflag |= CSTOPB; break;
		default:	fprintf(stderr,"Unsupported stop bits\n"); return FALSE;
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 150;	/*设置超时 15 seconds */
	options.c_cc[VMIN] = 0;		/*Upadate the options and do it now*/
	if (tcsetattr(fd, TCSANOW, &options) != 0){
		perror("SetupSerial 3");
		return 1;
	}
	options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);	/*Input*/
	options.c_oflag &= ~OPOST;	/*Output*/
	return 0;
}
