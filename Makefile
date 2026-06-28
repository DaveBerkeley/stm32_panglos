
PROJECT=TEST
TARGET=nucleo_f446re

MODE = run

.PHONY: all flash

VERBOSE := -v
#VERBOSE := 

all: include
	pio $(MODE) -e ${TARGET} ${VERBOSE}
	#PLATFORMIO_BUILD_FLAGS="-DPROJECT=${PROJECT} -D${PROJECT}" pio $(MODE) -e ${TARGET} ${VERBOSE}

flash: all
	pio $(MODE) -e $(TARGET) --target upload ${VERBOSE}

# Need to create a few soft links for uniform include paths

include:
	mkdir -p include
	ln -s ../third_party/freertos/include include/freertos
	ln -s ../third_party/freertos/portable/GCC/ARM_CM4F include/ARM_CM4F

clean:
	rm -rf .pio third_party/build output.map
	find . -name "*~" | xargs rm
	scons -c

TOOLPATH=~/.platformio/packages/toolchain-gccarmnoneeabi/bin
TOOLPREFIX=arm-none-eabi-
ELF:=.pio/build/${TARGET}/firmware.elf

dump:
	${TOOLPATH}/${TOOLPREFIX}objdump ${ELF} -d -S

nm:
	${TOOLPATH}/${TOOLPREFIX}nm ${ELF}

ar:
	${TOOLPATH}/${TOOLPREFIX}ar t ${ELF}

FRAMEWORK=~/.platformio/packages/framework-stm32cubef4

ctags:
	ctags -R . \
		${FRAMEWORK}/Drivers/STM32F4xx_HAL_Driver \
		${FRAMEWORK}/Drivers/CMSIS/Device/ST/STM32F4xx/Include

openocd:
	openocd -f board/st_nucleo_f4.cfg

gdb: ${ELF}
	gdb-multiarch -x .gdbinit

gdb-native: ./tdd
	gdb -x .gdbinit.native ./tdd

test: ./tdd
	scons
	valgrind -s ./tdd 

#	FIN
