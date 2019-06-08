#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <dlfcn.h>

#include "run.h"

#include <sys/types.h>

//int run_help(char *p_name);
//int my_test(void);

SYMBOL_INFO app_debug_sym[] = {
	{{"help",(P_ADDR)&run_help},"命令帮助：run 函数名称 参数"},
	{{"test",(P_ADDR)&my_test},"test show"},
	{{NULL,0},NULL}
};

//int run_help(char *p_name,SYMBOL_INFO *app_debug_sym){
int run_help(char *p_name){
	int i = 0;
	printf("%24s	%s %s","run version",__DATE__,__TIME__);
	while(0 != app_debug_sym[i].sym_name_addr.sym_name){
		if(p_name){
			if(0 == strcmp(p_name,app_debug_sym[i].sym_name_addr.sym_name)){
				printf("%24s	%s\n",app_debug_sym[i].sym_name_addr.sym_name,app_debug_sym[i].sym_info);
			}
		}else{
			printf("%24s	%s\n",app_debug_sym[i].sym_name_addr.sym_name,app_debug_sym[i].sym_info);
		}
		i++;
	}
	return 0;
}

int my_test(void){
	printf("...........\n");
	return 0;
}

int find_sym_info_addr(char *name, P_ADDR *addr, SYMBOL_INFO *sym_info){
	int i = 0;
	//printf("strcmp name: %s\n",name);
	while(0 != sym_info[i].sym_name_addr.sym_name){
		//printf("sym_info[i].sym_name_addr.sym_name: %s\n",sym_info[i].sym_name_addr.sym_name);
		if(0 == strcmp(name,sym_info[i].sym_name_addr.sym_name)){
			*addr = sym_info[i].sym_name_addr.sym_addr;
			//printf("*addr : 0x%llx\n",sym_info[i].sym_name_addr.sym_addr);
			return 0;
		}
		i++;
	}
	return -1;
}

int get_sym_addr(char *name, P_ADDR *addr){
	P_ADDR sym_addr;
	void *handle;
	char *error;

	handle = dlopen(NULL, RTLD_NOW);
	if(!handle){
		fprintf(stderr,"%s\n",dlerror());
		exit(1);
	}

	sym_addr = (P_ADDR)dlsym(handle,name);
	//printf("dlsym sym_addr : 0x%llx\n",sym_addr);
	//printf("dlsym: %p\n",dlsym(handle,name));
	//sym_addr = (int)dlsym(RTLD_DEFAULT,name);
	
	if((error = dlerror()) != NULL){
		fprintf(stderr,"%s\n",error);
	}
	
	dlclose(handle);

	if(sym_addr > 0){
		*addr = sym_addr;
		return 0;
	}

	return -1;
}


int main(int argc,char **argv){
	FUNCPTR routine;
	P_ADDR sym_addr;
	int ret;
	char *str_sym_name;
	int func_para[FUNC_PARA_NUM] = {0};
	int i;
	/*
	SYMBOL_INFO app_debug_sym[] = {
		{{"help",(P_ADDR)&run_help},"命令帮助：run 函数名称 参数"},
		{{"test",(P_ADDR)&my_test},"test show"},
		{{NULL,0},NULL}
};
	*/
	
	if(argc < 2){
		//(void)run_help(NULL,app_debug_sym);
		(void)run_help(NULL);
		return 0;
	}

	str_sym_name = argv[1];

	//如输入函数地址，则直接执行

	if(argv[1][0] >= '0' && argv[1][0] <= '9'){
		if(('x' == argv[1][1] || 'X' == argv[1][0])){
			sym_addr = strtoul(argv[1],0,16);
		}else{
			sym_addr = strtoul(argv[1],0,10);
		}
	}

	//参数转换
	for(i = 2; i < argc; i++){
		if((argv[i][0] >= '0') && (argv[i][0] <= '9')){
			if(('x' == argv[i][1]) || ('X' == argv[i][1])){
				func_para[i - 2] = strtoul(argv[i],0,16);
			}else{
				func_para[i - 2] = strtoul(argv[i],0,10);
			}
		}else{
			//参数为字符串地址
			//func_para[i - 2] = (P_ADDR)argv[i];
		}
		//printf("argv  %d , %s -> %d\n",i,argv[i],func_para[i-2]);
	}

	//查找自定义命令
	if(0 == find_sym_info_addr(str_sym_name,&sym_addr,app_debug_sym)){
	//查找当前进程符号表
	}else if(0 == get_sym_addr(str_sym_name,&sym_addr)){
	}else{
		//printf("no cmd func\n");
		//return -1;
	}

	if(sym_addr){
		//routine = (FUNCPTR)sym_addr;
		routine = (FUNCPTR)sym_addr;
		ret = routine(func_para[0],func_para[1],func_para[2],func_para[3],func_para[4],func_para[5],func_para[6],func_para[7]);
		if(ret){
			printf("ret val : %d\n",ret);
		}
	}
	return 0;
}
