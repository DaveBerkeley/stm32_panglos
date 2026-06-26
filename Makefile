
PROJECT=TEST

TARGET=nucleo_f446re


MODE = run


.PHONY: all flash

VERBOSE = -v
#VERBOSE = 

all:
	pio $(MODE) -e ${TARGET} ${VERBOSE}
	#PLATFORMIO_BUILD_FLAGS="-DPROJECT=${PROJECT} -D${PROJECT}" pio $(MODE) -e ${TARGET} ${VERBOSE}

clean:
	rm -rf .pio

TOOLPATH=~/.platformio/packages/toolchain-gccarmnoneeabi/bin
TOOLPREFIX=arm-none-eabi-

dump: all
	${TOOLPATH}/${TOOLPREFIX}objdump .pio/build/${TARGET}/firmware.elf -d -S

nm:	all
	${TOOLPATH}/${TOOLPREFIX}nm .pio/build/${TARGET}/firmware.elf

#	FIN
