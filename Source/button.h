#ifndef __FIREFLY_BUTTON_H__
#define __FIREFLY_BUTTON_H__

#include "cext.h"
#include "task.h"

#include <stdint.h>

void button_initialize(void);
void button_scan_proc(enum task_events ev);
void button_mod_proc(enum task_events ev);
uint8_t button_get_down_cnt(void);
void button_reset_down_cnt(void);
bit button_int_cb(uint8_t old_state, uint8_t new_state);
bit key_mod_pressed(void);
#endif
