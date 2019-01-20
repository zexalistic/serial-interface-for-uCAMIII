#include "cmd.h"
#include <time.h>

#define	IMG_SIZE 4800	//80*60 

int nwrite;
unsigned char rd_buf[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char img_buf[IMG_SIZE];
int size_read;

int Sync(void){
	unsigned char sync_cmd[6] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00}; 
	unsigned char ack_cmd[6] = {0xAA, 0x0E, 0x0D, 0x01, 0x00, 0x00}; 

	int cnt = 0;

	nwrite = write(fd, sync_cmd , 6);
	printf("%d\r\n",nwrite);
	printf("Send sync\r\n");

	//wait for ack
	while(1){			
		size_read = read(fd, rd_buf, 6);	//read rx
		if(size_read > 0){
			//check ack
			if(rd_buf[0] != 0xAA || rd_buf[1] != 0x0E || rd_buf[2] != 0x0D){		
				printf("ACK error!\r\n");
				printf("%x\t",rd_buf[0]);
				printf("%x\t",rd_buf[1]);
				printf("%x\t",rd_buf[2]);
				printf("%x\t",rd_buf[3]);
				printf("%x\t",rd_buf[4]);
				printf("%x\t",rd_buf[5]);
				exit(1);
			}
			else{
				size_read = read(fd, rd_buf, 6);		//clear buffer. In the experiment, there will be one more ack
				nwrite = write(fd, ack_cmd , 6);
				printf("Sync success!\r\n");
			}
		}
		else{
			if(cnt < 60){
				sleep(1);
				nwrite = write(fd, sync_cmd , 6);		//if no response, write again
				printf("%d\r\n",nwrite);
				cnt++;
			}
			else{
				printf("Sync outime\r\n");
				exit(1);
			}
		}
	}

	return 0;
}

int Pic(void){
	//unsigned char init_cmd[6] = {0xAA, 0x01, 0x00, 0x07, 0x07, 0x07}; 	//JPEG 640*480
	//unsigned char init_cmd[6] = {0xAA, 0x01, 0x00, 0x03, 0x01, 0x07}; 	//8bit Grey RAW 80*60
	//unsigned char init_cmd[6] = {0xAA, 0x01, 0x00, 0x03, 0x03, 0x07}; 	//8bit Grey RAW 160*120
	unsigned char init_cmd[6] = {0xAA, 0x01, 0x00, 0x06, 0x03, 0x07}; 		//16bit RGB RAW 160*120 

	unsigned char snapshot_cmd[6] = {0xAA, 0x05, 0x01, 0x00, 0x00, 0x00}; 	//get a snapshot immediately
	unsigned char getpic_cmd[6] = {0xAA, 0x04, 0x01, 0x00, 0x00, 0x00}; 	//get picture
	unsigned char ack_cmd[6] = {0xAA, 0x0E, 0x0D, 0x01, 0x00, 0x00}; 

	int fp;
	int size_write;

	//initialize
	nwrite = write(fd, init_cmd , 6);
	size_read = read(fd, rd_buf, 6);
	if(rd_buf[0] != 0xAA || rd_buf[1] != 0x0E || rd_buf[2] != 0x01)
		printf("Init error!\r\n");

	//take a snapshot
	nwrite = write(fd, snapshot_cmd , 6);
	size_read = read(fd, rd_buf, 6);
	if(rd_buf[0] != 0xAA || rd_buf[1] != 0x0E || rd_buf[2] != 0x05)
		printf("Snapshot error!\r\n");

	//get picture
	nwrite = write(fd, getpic_cmd , 6);
	size_read = read(fd, rd_buf, 6);
	if(rd_buf[0] != 0xAA || rd_buf[1] != 0x0E || rd_buf[2] != 0x04)
		printf("Get pic error!\r\n");
	size_read = read(fd, rd_buf, 6);
	if(rd_buf[0] != 0xAA || rd_buf[1] != 0x0A || rd_buf[2] != 0x01)
		printf("Data error!\r\n");
	size_read = read(fd, img_buf, IMG_SIZE);
	if(size_read != IMG_SIZE)
		printf("Img error!\r\n");

	//return ack
	nwrite = write(fd, ack_cmd , 6);

	//open file
	fp = open("image1", O_RDWR | O_CREAT);
	if (fp == -1){
		printf("open failure\n");
		return 1;
	}
	size_write = write(fp, img_buf, IMG_SIZE);

	close (fp);

	return 0;
}

