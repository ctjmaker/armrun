KERNELPATH = /opt/arm6818/kernel
OBJNAME = ctj_module

obj-m := $(OBJNAME).o
$(OBJNAME)-objs := ker.o my_mma8653.o my_i2c.o my_netlink.o my_irq.o

all:
	make -C ${KERNELPATH} M=$(PWD) modules
	cp $(OBJNAME).ko /opt/tftpdir
clean:
	make -C ${KERNELPATH} M=$(PWD) clean
	rm -rf *.ko
	rm -rf *.o
