/*
*/
#ifndef __KER_H__
#define __KER_H__

//#include <linux/init.h>
//#include <linux/module.h>

int my_mma8653_init(void);
void my_mma8653_exit(void);
int my_i2c_init(void);
void my_i2c_exit(void);
int my_netlink_init(void);
void my_netlink_exit(void);
int btn_init(void);
void btn_exit(void);
int send_usrmsg(char *pbuf,uint16_t len);

#endif
