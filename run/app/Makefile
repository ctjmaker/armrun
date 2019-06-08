APP = run
#CC	= gcc
CC	= arm-cortex_a9-linux-gnueabi-gcc
OBJ = mma8653.o i2c.o netlink.o
CFLAG = -Wall -ldl -rdynamic

all:$(APP)

.PHONY:$(APP)
$(APP):$(OBJ)
	$(CC) -o $@ $^ run.c $(CFLAG)
	cp run /opt/tftpdir

.PHONY:clean
clean:
	rm -f $(APP) $(OBJ)
