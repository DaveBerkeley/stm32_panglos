
file ./.pio/build/nucleo_f446re/firmware.elf

tar ext:3333

monitor reset halt
b main
b Error_Handler
b Error_HandlerX
tui enable
set tui mouse-events off

