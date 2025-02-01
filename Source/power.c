#include <STC8G.H>

#include "power.h"
#include "gpio.h"
#include "debug.h"
#include "int_hub.h"
#include "task.h"
#include "led.h"
#include "delay.h"
#include "button.h"

#define POWER_STATE_MASK 0x4

static bit power_int_flag;

bit power_int_cb(uint8_t old_state, uint8_t new_state)
{
  CDBG("power_int_cb state = 0x%02bx->0x%02bx\n", old_state, new_state);
  if((old_state ^ new_state) & POWER_STATE_MASK ) {
    if((old_state & POWER_STATE_MASK) == 0  && (new_state & POWER_STATE_MASK) != 0) {
      task_set(EV_ADP_ON);
      CDBG("power_int_cb: adapter on\n");
    } else {
      task_set(EV_ADP_OFF);
      CDBG("power_int_cb: adapter off\n");
    }
    return 1;
  }
  return 0;
}

void power_initialize(void)
{
  CDBG("power_initialize\n");
  ELVD = 0;
  power_int_flag = 0;
}

bit power_adapter_on(void)
{
  return int_hub_get_in_port(INT_HUB_P2) == 1; 
}

void power_set_enable(bit enable)
{
  PCON &= ~0x20; // clear LVDF
  POWER_EN = enable;
}

bit power_sleep(void)
{
  led_power_enable(0);
  int_hub_get_state(); // clear int
  task_clr(EV_EX_INT);
  power_enable_lvo(0);
  CDBG("enter power_sleep\n");
  power_int_flag = 1;
  PCON |= 0x2; // into PD
  CDBG("leave power_sleep\n");
  if(task_test(EV_EX_INT)) {
    CDBG("clear power_int_flag\n");
    power_int_flag = 0; // 按键和插电都会清除这个flag
  }
  led_power_enable(1);
  power_enable_lvo(1);
  return !power_int_flag; 
}

void power_enable_lvo(bit enable)
{
  PCON &= ~0x20; // clear LVDF
  ELVD = enable;
}

static void power_isr (void) interrupt 6 using 1
{
  task_set(EV_LP);
  PCON &= ~0x20; // clear LVDF
}