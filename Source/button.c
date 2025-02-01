#include <STC8G.H>

#include "button.h"
#include "cext.h"
#include "task.h"
#include "debug.h"
#include "sm.h"
#include "delay.h"
#include "gpio.h"
#include "clock.h"
#include "int_hub.h"
#include "power.h"

#define KEY_LPRESS_DELAY 3 // 长按时间
#define KEY_STATE_MASK 0x1
static bit mod_down;
static bit mod_lpress_send;
static uint32_t mod_down_sec;
static uint8_t mod_down_cnt;

bit button_int_cb(uint8_t old_state, uint8_t new_state)
{
  CDBG("button_int_cb state = 0x%02bx->0x%02bx\n", old_state, new_state);
  if((old_state ^ new_state) & KEY_STATE_MASK ) {
    if((old_state & KEY_STATE_MASK) == 0  && (new_state & KEY_STATE_MASK) != 0) {
      mod_down_cnt ++;
      CDBG("button_int_cb mod_down_cnt %bu\n", mod_down_cnt);
    }
    return 1;
  }
  return 0;
}

uint8_t button_get_down_cnt(void)
{
  return mod_down_cnt;
}

void button_reset_down_cnt(void)
{
  mod_down_cnt = 0;
}

void button_initialize(void)
{
  mod_down = 0;
  mod_lpress_send = 0;
  mod_down_cnt = 0;
}

bit key_mod_pressed(void)
{
  return int_hub_get_in_port(INT_HUB_P0);
}

void button_scan_proc(enum task_events ev)
{
  if(key_mod_pressed()) {
    if(mod_down == 0) {
      mod_down = 1;
      task_set(EV_KEY_MOD_DOWN);
      mod_down_sec = clock_get_now_sec();
    }
  } else {
    if(mod_down == 1) {
      if(!mod_lpress_send) {
        task_set(EV_KEY_MOD_PRESS);
      }
      mod_down = 0;
      mod_lpress_send = 0;
      task_set(EV_KEY_MOD_UP);
    }
  }
  
 
  if(mod_down) {
    if(clock_diff_now_sec(mod_down_sec) > KEY_LPRESS_DELAY) {
      task_set(EV_KEY_MOD_LPRESS);
      mod_lpress_send = 1;
    }
  }
}


void button_mod_proc(enum task_events ev)
{
  switch (ev) {
    case EV_KEY_MOD_DOWN:
      CDBG("button_mod_proc EV_KEY_MOD_DOWN\n");  
      break;           
    case EV_KEY_MOD_PRESS:
      CDBG("button_mod_proc EV_KEY_MOD_PRESS\n");  
      break;
    case EV_KEY_MOD_LPRESS:
      CDBG("button_mod_proc EV_KEY_MOD_LPRESS\n");
      break; 
    case EV_KEY_MOD_UP:
      CDBG("button_mod_proc EV_KEY_MOD_UP\n");  
      break;  
    default:
      ;
  }
  
  sm_run(ev);
}
