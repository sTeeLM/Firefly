#ifndef __FIREFLY_POWER_H__
#define __FIREFLY_POWER_H__

#include <stdint.h>

void power_initialize(void);
void power_set_enable(bit enable);
bit power_adapter_on(void);
bit power_sleep(void);
void power_enable_lvo(bit enable);
bit power_int_cb(uint8_t old_state, uint8_t new_state);

#endif
