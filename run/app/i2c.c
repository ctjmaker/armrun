#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "run.h"

#define I2CDEV "/dev/my_i2c"
#define MY_I2C_BYTE_WRITE		0x100001
#define MY_I2C_BYTE_READ		0x100002

int i2c_reg_byte_write(u8 adapt,u8 addr,u8 reg,u8 val){
	int ret = 0;
	int fd = 0;
	char uindex[4] = {0};
	int cmd = MY_I2C_BYTE_WRITE;
	uindex[0] = adapt;
	uindex[1] = addr;
	uindex[2] = reg;
	uindex[3] = val;

	fd = open(I2CDEV,O_RDWR);

	if(fd < 0){
		perror("open I2C");
		return -1;
	}
	
	ret = ioctl(fd,cmd,uindex);

	close(fd);

	return ret;
}

int i2c_reg_byte_read(u8 adapt,u8 addr,u8 reg,u8 *val){
	int ret = 0;
	int fd = 0;
	char uindex[4] = {0};
	int cmd = MY_I2C_BYTE_READ;
	uindex[0] = adapt;
	uindex[1] = addr;
	uindex[2] = reg;

	fd = open(I2CDEV,O_RDWR);

	if(fd < 0){
		perror("open I2C");
		return -1;
	}
	
	ret = ioctl(fd,cmd,uindex);

	*val = uindex[3];

	close(fd);

	return ret;
}

int i2c_reg_byte_show(u8 adapt,u8 addr,u8 reg,u8 count){
	char i = 0;
	int ret = 0;
	int n = count;
	unsigned char val = 0;
	if(n > 100){
		n = 100;
	}
	if(0 == n){
		n = 1;
	}

	for(i = 0; i < n; i++){
		ret = i2c_reg_byte_read(adapt,addr,i,&val);
		printf("i2c adapt: 0x%x---addr: 0x%x---reg: 0x%x---0x%x\n",adapt,addr,i,val);
	}

	return ret;
}
