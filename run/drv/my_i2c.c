#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>		//misc_register
#include <linux/fs.h>				//file_operations
#include <linux/uaccess.h>			//copy_from_user

#include "ker.h"

#define MY_I2C_BYTE_WRITE		0x100001
#define MY_I2C_BYTE_READ		0x100002

int my_i2c_byte_write(struct i2c_adapter *adapt,u8 addr,u8 reg,u8 val){
	//return i2c_smbus_xfer(my_mma8653_client,reg,val);
	union i2c_smbus_data data;
	u16 i2c_addr = addr;
	data.byte = val;
	return i2c_smbus_xfer(adapt,i2c_addr,0,I2C_SMBUS_WRITE,reg,I2C_SMBUS_BYTE_DATA,&data);
}

int my_i2c_byte_read(struct i2c_adapter *adapt,u8 addr,u8 reg,u8* val){
	//int ret = 0;
	//ret = i2c_smbus_read_byte_data(my_mma8653_client,reg);
	//*val = ret;
	//printk(KERN_ERR"REG : 0X%X, VAL: 0X%X",reg,*val);
	union i2c_smbus_data data;
	int status;
	u16 i2c_addr = addr;
	status = i2c_smbus_xfer(adapt,i2c_addr,0,I2C_SMBUS_READ,reg,I2C_SMBUS_BYTE_DATA,&data);
	*val=((status < 0) ? status : data.byte);
	return status;
}

static long my_i2c_ioctl(struct file *file,unsigned int cmd,unsigned long arg){
	int ret = 0;
	u8 kindex[4] = {0};
	struct i2c_adapter *my_i2c_adapter;

	switch(cmd){
		case MY_I2C_BYTE_WRITE:
			if(copy_from_user(kindex,(u8 *)arg,sizeof(kindex)))
				return -EFAULT;
			switch(kindex[0]){
				case 0:
					my_i2c_adapter = i2c_get_adapter(0);
					break;
				case 1:
					my_i2c_adapter = i2c_get_adapter(1);
					break;
				case 2:
					my_i2c_adapter = i2c_get_adapter(2);
					break;
				default:
					return -1;
			}
			ret = my_i2c_byte_write(my_i2c_adapter,kindex[1],kindex[2],kindex[3]);
			break;
		case MY_I2C_BYTE_READ:
			if(copy_from_user(kindex,(u8 *)arg,sizeof(kindex)))
				return -EFAULT;
			switch(kindex[0]){
				case 0:
					my_i2c_adapter = i2c_get_adapter(0);
					break;
				case 1:
					my_i2c_adapter = i2c_get_adapter(1);
					break;
				case 2:
					my_i2c_adapter = i2c_get_adapter(2);
					break;
				default:
					return -1;
			}
			ret = my_i2c_byte_read(my_i2c_adapter,kindex[1],kindex[2],&kindex[3]);
			if(copy_to_user((u8 *)arg,kindex,sizeof(kindex)))
				return -EFAULT;
			break;
		default:
			return -1;
	}

	return ret;
}

static struct file_operations my_i2c_fops = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl = my_i2c_ioctl
};

static struct miscdevice my_i2c_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "my_i2c",
	.fops = &my_i2c_fops
};

int my_i2c_init(void){
	misc_register(&my_i2c_misc);
	return 0;
}

void my_i2c_exit(void){
	misc_deregister(&my_i2c_misc);
	return ;
}
