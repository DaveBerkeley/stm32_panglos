
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void heap_init();
void FATAL(const char *msg);

uint32_t get_idle_count();

void tx_uart(const char *text);

#ifdef __cplusplus
}
#endif

//  FIN
