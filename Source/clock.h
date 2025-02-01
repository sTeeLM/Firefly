#ifndef __FIREFLY_CLOCK_H__
#define __FIREFLY_CLOCK_H__

#include "task.h"

void clock_initialize(void);
void clock_time_proc(enum task_events ev);
uint32_t clock_get_now_sec(void);
uint32_t clock_diff_now_sec(uint32_t sec);
void clock_set_timer(uint8_t timer_sec);
uint32_t clock_get_now_jiff(void);
uint32_t clock_diff_now_jiff(uint32_t saved_jiff);

#endif