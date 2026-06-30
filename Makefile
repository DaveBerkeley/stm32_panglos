
PROJECT=TEST
TARGET=nucleo_f446re

MODE = run
ARGS:=

.PHONY: all flash

all: include
	pio $(MODE) -e ${TARGET} ${ARGS}

flash: all
	pio $(MODE) -e $(TARGET) --target upload ${ARGS}

# Need to create a few soft links for uniform include paths

include:
	mkdir -p include
	ln -s ../third_party/freertos/include include/freertos
	ln -s ../third_party/freertos/portable/GCC/ARM_CM4F include/ARM_CM4F

clean:
	scons -c
	rm -rf .pio third_party/build output.map
	find . -name "*~" | xargs rm

TOOLPATH=~/.platformio/packages/toolchain-gccarmnoneeabi/bin
TOOLPREFIX=arm-none-eabi-
ELF:=.pio/build/${TARGET}/firmware.elf

dump:
	${TOOLPATH}/${TOOLPREFIX}objdump ${ELF} -d -S ${ARGS}

nm:
	${TOOLPATH}/${TOOLPREFIX}nm ${ELF} ${ARGS}

ar:
	${TOOLPATH}/${TOOLPREFIX}ar t ${ELF} ${ARGS}

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
	gdb -x .gdbinit.native ./tdd ${ARGS}

test: ./tdd
	scons
	valgrind -s ./tdd --gtest_filter="-Main.App" ${ARGS}

#	FIN
