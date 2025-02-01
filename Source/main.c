#include <STC8G.H>

#include "debug.h"
#include "gpio.h"
#include "power.h"
#include "com.h"
#include "delay.h"
#include "task.h"
#include "clock.h"
#include "power.h"
#include "button.h"
#include "int_hub.h"
#include "tdc.h"
#include "led.h"
#include "sm.h"
#include "task.h"

#define START_WAIT_JIFF 8

void main(void)
{
  uint32_t start_jiff;
  uint8_t  button_cnt;
  uint8_t  power_on = 0;
  
  EA = 1; 
  
  debug_initialize();
  gpio_initialize();
  com_initialize();
  power_initialize();
  
  gpio_enable_hub(1);
  
  power_set_enable(1);
  
  //如果插着USB开机，打开串口
  if(power_adapter_on()) {
    gpio_enable_serial(1);
    debug_onoff(1);
  }
  
  int_hub_initilize(); 
 
  clock_initialize();
  button_initialize();
  
  /* 使能其他外设 */
  tdc_initilize();
  led_initialize();
  
  task_initialize();
  sm_initialize(0);
  
  while(key_mod_pressed());
  
  /* 如果没有插充电器，运行防止误触碰逻辑 */
  
  if(!power_adapter_on()) { 
    start_jiff  = clock_get_now_jiff();
    button_cnt = button_get_down_cnt();
    
    while(clock_diff_now_jiff(start_jiff) <= START_WAIT_JIFF) {
      if(task_test(EV_EX_INT)) {
        int_hub_proc(EV_EX_INT);
        task_clr(EV_EX_INT);
      }
      if(button_cnt != button_get_down_cnt()) {
        start_jiff  = clock_get_now_jiff();
        button_cnt = button_get_down_cnt();
      }
      if(button_cnt >= 3) {
        sm_initialize(1);
        break;
      }
    }
  } else {
    CDBG("power_adapter_on\n"); 
  }
  
  led_power_enable(1);
  
  while(1) {
    task_run();
  }
}