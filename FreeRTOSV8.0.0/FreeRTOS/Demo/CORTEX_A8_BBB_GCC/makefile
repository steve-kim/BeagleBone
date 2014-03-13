#  BBB makefile

#TOOLS
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
BIN=arm-none-eabi-objcopy

#PATHS
VPATH=src include
LDIR=./lib
IDIR=./include
LIB_GCC=${LIB_PATH}/lib/gcc/arm-none-eabi/4.7.3/
LIB_C=${LIB_PATH}/arm-none-eabi/lib/

# Required library files
APP_LIB=-ldrivers  \
        -lutils    \
        -lplatform \
        -lsystem_config

RUNTIMELIB=-lc -lgcc

#FLAGS
CFLAGS=-mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a
CFLAGS+=-c -g -O2 -mlong-calls -fdata-sections -funsigned-char \
            -ffunction-sections -Wall -Dgcc -D am335x -D beaglebone \
            -DSUPPORT_UNALIGNED -DMMCSD -DUARTCONSOLE
CFLAGS+=-I $(IDIR)

LDFLAGS=-e Entry -u Entry -u __aeabi_uidiv -u __aeabi_idiv --gc-sections

BINFLAGS=-O binary

IMG_LOAD_ADDR=0x80000000

app: main.o
	$(LD) -o $@.out $< -T bbb.ld -Map bbb.map $(APP_LIB) $(LDFLAGS) $(RUNTIMELIB) -L $(LDIR) \
		 -L$(LIB_GCC) -L$(LIB_C)
	$(BIN) $(BINFLAGS) $@.out $@.bin
	cd tools; gcc tiimage.c -o a.out; cd -
	./tools/a.out $(IMG_LOAD_ADDR) NONE $@.bin $@
	rm app.*

.PHONY: clean
clean:
	rm *.o; rm *.map
