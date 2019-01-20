#include "include.h"
#include "cmd.h"
#include "serial.h"

/**************************************global*variables************************************/
int fd;
/**************************************main*function************************************/
int main(int argc, char *argv[]){
	char *interface =  NULL, *filename = NULL;
	int i,t = 0;	
	
	int  size_read = 0 ;	
	//unsigned short length,tick_count = 0;	//debug information

	if (argc < 2){
		interface = UART_DEVICE;
	}
	else{
		interface = argv[1];
		//filename = argv[2];
	}

	//Open USB
	fd = open(interface, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1){
		printf("%s Open Error!\n", interface);
		return FALSE;
	}
	printf("Open %s successfully\n", interface);


	set_speed(fd, 115200);
	if (set_Parity(fd, 8, 1, 'N') == FALSE){
		printf("Set Parity Error\n");
		exit(1);
	}
//	tcflush(fd, TCIOFLUSH);

	Sync();
	//Pic();

	close (fd);

	return 0;
}



