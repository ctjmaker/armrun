#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "run.h"

#define MMA8653DEV "/dev/my_i2c2_mma8653"
#define MY_MMA8653_WRITE	0x100001
#define MY_MMA8653_READ		0x100002

int mma8653_reg_write(unsigned char reg,unsigned char val){
	int ret = 0;
	int fd = 0;
	char uindex[2] = {0};
	int cmd = MY_MMA8653_WRITE;
	uindex[0] = reg;
	uindex[1] = val;

	fd = open(MMA8653DEV,O_RDWR);

	if(fd < 0){
		perror("open MMA8653DEV");
		return -1;
	}
	
	ret = ioctl(fd,cmd,uindex);

	close(fd);

	return ret;
}

int mma8653_reg_read(unsigned char reg,unsigned char *val){
	int ret = 0;
	int fd = 0;
	char uindex[2] = {0};
	int cmd = MY_MMA8653_READ;
	uindex[0] = reg;

	fd = open(MMA8653DEV,O_RDWR);

	if(fd < 0){
		perror("open MMA8653DEV");
		return -1;
	}
	
	ret = ioctl(fd,cmd,uindex);

	*val = uindex[1];

	close(fd);

	return ret;
}

int mma8653_reg_show(unsigned char reg,unsigned int count){
	char i = 0;
	int ret = 0;
	int n = count;
	unsigned char val = 0;
	if(n > 100){
		n = 100;
	}

	for(i = 0; i < n; i++){
		ret = mma8653_reg_read(i,&val);
		printf("mma8653 0x%x---------0x%x\n",i,val);
	}

	return ret;
}
