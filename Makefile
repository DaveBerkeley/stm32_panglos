
PROJECT=TEST

TARGET=nucleo_f446re


MODE = run


.PHONY: all flash

VERBOSE = -v
#VERBOSE = 

# Need to create a few soft links for uniform include paths

all: include/freertos include/ARM_CM4F
	pio $(MODE) -e ${TARGET} ${VERBOSE}
	#PLATFORMIO_BUILD_FLAGS="-DPROJECT=${PROJECT} -D${PROJECT}" pio $(MODE) -e ${TARGET} ${VERBOSE}

include:
	mkdir include

include/freertos: include
	ln -s ../third_party/freertos/include $@

include/ARM_CM4F: include
	ln -s ../third_party/freertos/portable/GCC/ARM_CM4F $@

clean:
	rm -rf .pio

TOOLPATH=~/.platformio/packages/toolchain-gccarmnoneeabi/bin
TOOLPREFIX=arm-none-eabi-

dump: all
	${TOOLPATH}/${TOOLPREFIX}objdump .pio/build/${TARGET}/firmware.elf -d -S

nm:	all
	${TOOLPATH}/${TOOLPREFIX}nm .pio/build/${TARGET}/firmware.elf

#	FIN
