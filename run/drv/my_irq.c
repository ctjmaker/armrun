#include <linux/init.h>
#include <linux/module.h>
#include <asm/gpio.h>
#include <mach/platform.h>

//中断相关头
#include <linux/irq.h>
#include <linux/interrupt.h>

#include <linux/input.h>

#include "ker.h"

struct btn_resource{
	char *name;	//按键名称
	int gpio;	//按键对应的GPIO号
	int code;	//按键值
};

static struct btn_resource btn_info[] = {
	{
		.name = "KEY_UP",
		.gpio = PAD_GPIO_A + 28,
		.code = KEY_UP
	},
	{
		.name = "KEY_DOWN",
		.gpio = PAD_GPIO_B + 30,
		.code = KEY_DOWN
	},
	{
		.name = "KEY_LEFT",
		.gpio = PAD_GPIO_B + 31,
		.code = KEY_LEFT
	},
	{
		.name = "KEY_RIGHT",
		.gpio = PAD_GPIO_B + 9,
		.code = KEY_RIGHT
	}
};

static struct btn_resource *pdata;

//tasklet延后处理函数
static void btn_tasklet_function(unsigned long data){
	unsigned int state;
	char *mychar = "!!!!!!!!!!!!!!!!";
	state = gpio_get_value(pdata->gpio);
	//3.打印按键信息
	//printk(KERN_ERR"%s: 按键%s:, 键值:%d, 状态:%s\n",__func__, pdata->name, pdata->code, state ? "松开":"按下");
	printk(KERN_ERR"底半部: 按键%s:, 键值:%d, 状态:%s, data = %#x\n", pdata->name,pdata->code, state ? "松开":"按下", *(int *)data);
	send_usrmsg(mychar,strlen(mychar));
	return ;
}

static int g_data = 0x5555;
//定义初始化tasklet对象
static DECLARE_TASKLET(btn_tasklet,//对象名
					btn_tasklet_function,//延后处理函数
					(unsigned long)&g_data); //传递的参数

//顶半部
static irqreturn_t button_isr(int irq, void *dev){
	printk(KERN_ERR"button_isr\n");
	//1.获取到当前操作的按键对应的硬件信息
	pdata = (struct btn_resource *)dev;
	//2.获取按键的操作状态
	tasklet_schedule(&btn_tasklet);
	return IRQ_HANDLED;//中断处理函数执行成功
}

int btn_init(void){
	int i;
	irqreturn_t ret;
	printk(KERN_ERR"btn_init\n");
	for(i = 0; i < ARRAY_SIZE(btn_info); i++){
		gpio_request(btn_info[i].gpio,btn_info[i].name);
		ret = request_irq(//中断号
				gpio_to_irq(btn_info[i].gpio),
				//中断处理函数
				button_isr,
				//双边沿触发
				//IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING,
				IRQF_TRIGGER_FALLING,
				//中断名称
				btn_info[i].name,
				//给中断处理函数传递的参数
				&btn_info[i]);
	}
	return 0;
}

void btn_exit(void){
	int i;
	//1.释放GPIO资源和中断资源
	for (i = 0; i < ARRAY_SIZE(btn_info); i++) {
		gpio_free(btn_info[i].gpio);
		free_irq(//中断号
				gpio_to_irq(btn_info[i].gpio),
				//给中断处理函数传递的参数
				&btn_info[i]);
	}
	return ;
}
