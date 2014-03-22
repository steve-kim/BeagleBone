#  BBB makefile

IMG_LOAD_ADDR=0x80000000

ROOT=..
TI_TOOLS=$(ROOT)/tools/ti_image/

include ${ROOT}/build/makedefs


#LIB_GCC=${LIB_PATH}/lib/gcc/arm-none-eabi/4.7.3/
#LIB_C=${LIB_PATH}/arm-none-eabi/lib/

#
# Required library files
#
APP_LIB=-ldrivers  \
        -lutils    \
        -lplatform \
        -lsystem_config

#GCC=arm-none-eabi-gcc
#LD=arm-none-eabi-ld
#BIN=arm-none-eabi-objcopy

#RUNTIMELIB=-lc -lgcc

#
# Linker options
#
#LDFLAGS=-e Entry -u Entry -u __aeabi_uidiv -u __aeabi_idiv --gc-sections

#
# Device and EVM definitions
#
DEVICE=am335x
EVM=beaglebone

TARGET_MODE=Release

FREERTOS_SOURCE= ${FREERTOS_SRC}/list.o \
                ${FREERTOS_SRC}/queue.o \
                ${FREERTOS_SRC}/tasks.o \
                ${FREERTOS_SRC}/timers.o \
                ${FREERTOS_PORT_SRC}/port.o \
		${FREERTOS_PORT_SRC}/portASM.o \
		${FREERTOS_HEAP_SRC}/heap_1.o 

DEMO_SOURCE=${ROOT}/applications/main.o \
            ${ROOT}/applications/freertos_hooks.o

#CFLAGS=-mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a
#CFLAGS+=-c -g -O2 -mlong-calls -fdata-sections -funsigned-char \
            -ffunction-sections -Wall -Dgcc -D${DEVICE} -D${EVM} \
            -DSUPPORT_UNALIGNED -DMMCSD -DUARTCONSOLE
app: ${FREERTOS_SOURCE} ${DEMO_SOURCE}
	$(LD) -o $@.out $^ -T bbb.ld -Map bbb.map $(LPATH) $(APP_LIB) $(LDFLAGS) $(RUNTIMELIB) -L. \
		 -L$(LIB_GCC) -L$(LIB_C)
	$(BIN) $(BINFLAGS) $@.out $@.bin
	gcc $(TI_TOOLS)tiimage.c -o a.out;
	./a.out $(IMG_LOAD_ADDR) NONE $@.bin $@_ti.bin; mv $@_ti.bin $@

%.o: %.c
	$(CC) $(CFLAGS) $(IPATH) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(IPATH) $< -o $@


