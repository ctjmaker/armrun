/*
*/

#ifndef __RUN_H__
#define __RUN_H__

#define FUNC_PARA_NUM 8
//#define __ENV64__

#if defined (__ENV64__)
	typedef unsigned long long P_ADDR;
#else
	typedef unsigned int P_ADDR;
#endif

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned int u16;
typedef signed int s16;
typedef unsigned char u8;
typedef signed char s8;


typedef int (*FUNCPTR)(int,int,int,int,int,int,int,int);

//符号名称地址
typedef struct _SYMBOL_ADDR{
	char *sym_name;
	P_ADDR sym_addr;
}SYMBOL_ADDR;

//符号描述
typedef struct _SYMBOL_INFO{
	SYMBOL_ADDR sym_name_addr;
	char *sym_info;
}SYMBOL_INFO;

int run_help(char *p_name);
int my_test(void);
int mma8653_reg_write(unsigned char reg,unsigned char val);
int mma8653_reg_show(unsigned char reg,unsigned int count);
int i2c_reg_byte_write(u8 adapt,u8 addr,u8 reg,u8 count);
int i2c_reg_byte_show(u8 adapt,u8 addr,u8 reg,u8 count);

#endif
