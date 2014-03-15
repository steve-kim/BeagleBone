#  BBB makefile

ifndef TARGET_MODE
	TARGET_MODE=release
endif

ROOT=.

include ${ROOT}/makedefs_ti

#
# Target Directories that need to be built
#
DIRS=${DRIVERS_BLD} ${PLATFORM_BLD} ${SYSCONFIG_BLD} ${UTILITY_BLD}

# Required library files
APP_LIB=-ldrivers  \
        -lutils    \
        -lplatform \
        -lsystem_config

IMG_LOAD_ADDR=0x80000000

app: main.o lib_ti
	$(LD) -o $@.out $< -T bbb.ld -Map bbb.map $(APP_LIB) $(LDFLAGS) $(RUNTIMELIB) -L $(LPATH) \
		 -L$(LIB_GCC) -L$(LIB_C)
	$(BIN) $(BINFLAGS) $@.out $@.bin
	cd tools; gcc tiimage.c -o a.out; cd -
	$(TOOLS_TI)/a.out $(IMG_LOAD_ADDR) NONE $@.bin $@
	rm app.*

lib_ti:
	@for i in ${DIRS};						\
	do								\
		if [ -f $${i}/makefile ] ;				\
		then							\
			make $(TARGET_MODE) -C $${i} || exit $$?;	\
		fi;							\
	done;
.PHONY: clean

clean:
	find ${ROOT}/ -name *.o -exec rm '{}' \;
	-rm *.map
	@for i in ${DIRS};						\
	do								\
		if [ -f $${i}/makefile ] ;				\
		then							\
			make clean -C $${i} || exit $$?;		\
		fi;							\
	done;
