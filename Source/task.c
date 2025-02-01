#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "clock.h"
#include "task.h"
#include "debug.h"
#include "sm.h"

/* hardware*/
#include "button.h"
#include "int_hub.h"

const char * code task_names[] =
{
  "EV_INIT",
  "EV_KEY_SCAN",  
  "EV_EX_INT", 
  "EV_ADP_ON",
  "EV_ADP_OFF",
  "EV_250MS",
  "EV_1S",
  "EV_KEY_MOD_DOWN",  
  "EV_KEY_MOD_PRESS",
  "EV_KEY_MOD_LPRESS",
  "EV_KEY_MOD_UP",
  "EV_TIMEO",
  "EV_LP",
};

static void null_proc(enum task_events ev)
{
  sm_run(ev);
}


static const TASK_PROC task_procs[EV_CNT] = 
{
  null_proc,
  /* EV_SCAN_KEY */
  button_scan_proc, 
  /* EV_EX_INT */
  int_hub_proc,
  /* EV_ADP_ON */
  null_proc,
  /* EV_ADP_OFF */
  null_proc,
  /* EV_250MS*/
  clock_time_proc,
  /* EV_1S*/
  clock_time_proc,
  /* EV_KEY_MOD_XX */
  button_mod_proc,
  button_mod_proc,
  button_mod_proc,
  button_mod_proc,
  null_proc,
  null_proc,
};


uint16_t ev_bits0;
uint16_t ev_bits1;

void task_initialize (void)
{
  CDBG("task_initialize\n");
  ev_bits0 = 0;
  ev_bits1 = 0;
}

void task_run(void)
{
  uint8_t c;
  for(c = 0; c < EV_CNT; c++) {
    if(task_test(c)) {
      task_clr(c);
      task_procs[c](c);
    }
  }
}

void task_dump(void)
{
  uint8_t i;
  for (i = 0 ; i < EV_CNT; i ++) {
    CDBG("[%02bd][%s] %c\n", i, task_names[i], task_test(i) ? '1' : '0');
  }
}
