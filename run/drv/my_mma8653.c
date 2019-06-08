#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>		//misc_register
#include <linux/fs.h>				//file_operations
#include <linux/uaccess.h>			//copy_from_user

#include "ker.h"

#define MY_MMA8653_NAME "my_mma8653"

#define MY_MMA8653_WRITE	0x100001
#define MY_MMA8653_READ	0x100002

static struct i2c_client *my_mma8653_client;
static struct i2c_board_info my_board_info = {
	.type = MY_MMA8653_NAME,
	.addr = 0x1D,
	//.addr = 0xff, printk error info: i2c i2c-2: Invalid 7-bit I2C address 0xff
};
static const struct i2c_device_id my_mma8653_id[] = {
	{MY_MMA8653_NAME,0},
	{}
};

int my_mma8653_write(u8 reg,u8 val){
	return i2c_smbus_write_byte_data(my_mma8653_client,reg,val);
}

int my_mma8653_read(u8 reg,u8* val){
	int ret = 0;
	ret = i2c_smbus_read_byte_data(my_mma8653_client,reg);
	*val = ret;
	//printk(KERN_ERR"REG : 0X%X, VAL: 0X%X",reg,*val);
	//printk(KERN_ERR"client->flags: 0X%X",my_mma8653_client->flags); //flag = 0
	return ret;
}

static long my_mma8653_ioctl(struct file *file,unsigned int cmd,unsigned long arg){
	u8 kindex[2] = {0};
	int ret = 0;
	switch(cmd){
		case MY_MMA8653_WRITE:
			if(copy_from_user(kindex,(u8 *)arg,sizeof(kindex)))
				return -EFAULT;
			ret = my_mma8653_write(kindex[0],kindex[1]);
			break;
		case MY_MMA8653_READ:
			if(copy_from_user(kindex,(u8 *)arg,sizeof(kindex)))
				return -EFAULT;
			ret = my_mma8653_read(kindex[0],&kindex[1]);
			if(copy_to_user((u8 *)arg,kindex,sizeof(kindex)))
				return -EFAULT;
			break;
		default:
			return -1;
	}
	return ret;
}

static struct file_operations my_mma8653_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl = my_mma8653_ioctl
};

static struct miscdevice my_mma8653_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "my_i2c2_mma8653",
	.fops = &my_mma8653_fops
};

static int my_mma8653_probe(struct i2c_client *client,const struct i2c_device_id *id){
	printk(KERN_ERR"%s\n",__func__);
	//printk(KERN_ERR"smbus_xfer : %pF\n",client->adapter->algo->smbus_xfer);//NULL
	//printk(KERN_ERR"master_xfer : %pF\n",client->adapter->algo->master_xfer);//s3c24xx_i2c_xfer
	misc_register(&my_mma8653_misc);
	return 0;
}

static int __devexit my_mma8653_remove(struct i2c_client *client){
	printk(KERN_ERR"%s\n",__func__);
	misc_deregister(&my_mma8653_misc);
	return 0;
}

static struct i2c_driver my_mma8653_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "my_mma8653_driver",
	},
	.id_table = my_mma8653_id,
	.probe = my_mma8653_probe,
	.remove = __devexit_p(my_mma8653_remove),
};

int my_mma8653_init(void){
	struct i2c_adapter *my_mma8653_adapt;

	my_mma8653_adapt = i2c_get_adapter(2);

	my_mma8653_client = i2c_new_device(my_mma8653_adapt,&my_board_info);

	i2c_put_adapter(my_mma8653_adapt);

	return i2c_add_driver(&my_mma8653_driver);

}

void my_mma8653_exit(void){
	i2c_unregister_device(my_mma8653_client);

	i2c_del_driver(&my_mma8653_driver);
	return ;
}
