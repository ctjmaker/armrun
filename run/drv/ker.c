#include <linux/init.h>
#include <linux/module.h>

#include "ker.h"

int __init ker_init(void){
	my_mma8653_init();
	my_i2c_init();
	my_netlink_init();
	btn_init();
	return 0;
}

void __exit ker_exit(void){
	my_mma8653_exit();
	my_i2c_exit();
	my_netlink_exit();
	btn_exit();
	return;
}

module_init(ker_init)
module_exit(ker_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ctj");
MODULE_DESCRIPTION("kernel ko init");
