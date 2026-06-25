
PROJECT=TEST

TARGET=nucleo_f446re


MODE = run


.PHONY: all flash


all:
	PLATFORMIO_BUILD_FLAGS="-DPROJECT=${PROJECT} -D${PROJECT}" pio $(MODE) -e $(TARGET) 

clean:
	rm -rf .pio

#	FIN
