#include <stdlib.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <dlfcn.h>
int add(int a,int b)
{
	    printf("add\n");
		    return a*b;
};
int main()
{
	char *err;
	int (*func)(int ,int );
	void *h=dlopen(NULL,RTLD_NOW);

	//void *h=dlopen(RTLD_DEFAULT,RTLD_NOW);
	func=dlsym(h,"add");
	printf("sizeof(int) : %ld\n",sizeof(int));
	printf("sizeof(unsigned long long) : %ld\n",sizeof(unsigned long long));
	printf("func : %p\n",func);
	err=dlerror();
	if(err!=NULL)
		printf("%s\n",err);
	printf("%d\n",(*func)(12,1000));
	dlclose(h);
	return 0;
}
