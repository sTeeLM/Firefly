#include "sm_firefly.h"
#include "task.h"
#include "sm.h"
#include "cext.h"
#include "debug.h"
#include "led.h"
#include "tdc.h"
#include "power.h"
#include "gpio.h"
#include "delay.h"

static void firefly_start_light(uint8_t to_state, enum task_events ev, led_type_t led, tdc_type_t tdc)
{
  led_clean();
  if(sm_cur_state != to_state) {
    
    power_enable_lvo(1);
    
    led_blink(led, 100, 3);
    if(tdc != TDC_TYPE_NONE) {
      tdc_trigger(tdc);
    } else {
      
      led_enable(led, 1);
    }
    
    if(ev == EV_ADP_ON) {
      gpio_enable_serial(1);
      debug_onoff(1);
    } else if(ev == EV_ADP_OFF) {
      gpio_enable_serial(0);
      debug_onoff(0);
    }
    
    return;
  }
  
  if(ev == EV_TIMEO && tdc != TDC_TYPE_1S) {
    led_blink(led, 200, 1);
    tdc_trigger_sleep(tdc);
  } else {
    led_blink(led, 200, 1);
    tdc_trigger(tdc);
  }
}

static void do_firefly_flu_blink_none(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  if(ev == EV_INIT || (ev == EV_KEY_MOD_PRESS && sm_cur_state == SM_FIREFLY_POWER_OFF)) {
    delay_ms(200);
    power_set_enable(1); 
    led_power_enable(1);
  }  
  
  firefly_start_light(to_state, ev, LED_TYPE_FLU, TDC_TYPE_NONE);
}

static void do_firefly_flu_blink_5s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_FLU, TDC_TYPE_5S);
}

static void do_firefly_flu_blink_15s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_FLU, TDC_TYPE_15S);
}

static void do_firefly_white_blink_none(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_WHITE, TDC_TYPE_NONE);
}

static void do_firefly_white_blink_1s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_WHITE, TDC_TYPE_1S);
}

static void do_firefly_white_blink_5s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_WHITE, TDC_TYPE_5S);
}

static void do_firefly_red_blink_none(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_RED, TDC_TYPE_NONE);
}

static void do_firefly_red_blink_1s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_RED, TDC_TYPE_1S);
}

static void do_firefly_red_blink_5s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_RED, TDC_TYPE_5S);
}

static void do_firefly_org_blink_none(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_ORG, TDC_TYPE_NONE);
}

static void do_firefly_org_blink_1s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_ORG, TDC_TYPE_1S);
}

static void do_firefly_org_blink_5s(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_ORG, TDC_TYPE_5S);
}

static void do_firefly_utr_blink_none(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  firefly_start_light(to_state, ev, LED_TYPE_UTR, TDC_TYPE_NONE);
}

static void do_firefly_power_off(uint8_t to_func, uint8_t to_state, enum task_events ev)
{
  uint8_t i;
  led_clean();
  for(i = 0 ; i < 3 ; i ++) {
    led_blink(LED_TYPE_RED, 50, 1);
    led_blink(LED_TYPE_WHITE, 50, 1);
    led_blink(LED_TYPE_ORG, 50, 1);
    led_blink(LED_TYPE_UTR, 50, 1);
  }
  led_power_enable(0);
  power_set_enable(0);
}

static const struct sm_trans_slot code  sm_trans_firefly_init[] = {
  {EV_INIT, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_NONE, do_firefly_flu_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_flu_blink_none[] = {
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_5S, do_firefly_flu_blink_5s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_OFF, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_5S, do_firefly_flu_blink_5s}, 
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_flu_blink_5s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_5S, do_firefly_flu_blink_5s},  
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_15S, do_firefly_flu_blink_15s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off}, 
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_NONE, do_firefly_flu_blink_none},  
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_flu_blink_15s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_15S, do_firefly_flu_blink_15s},  
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_NONE, do_firefly_white_blink_none},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off}, 
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_NONE, do_firefly_flu_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_white_blink_none[] = {
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_1S, do_firefly_white_blink_1s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_OFF, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_5S, do_firefly_white_blink_5s}, 
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_white_blink_1s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_1S, do_firefly_white_blink_1s},  
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_5S, do_firefly_white_blink_5s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_NONE, do_firefly_white_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_white_blink_5s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_5S, do_firefly_white_blink_5s},  
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_RED_BLINK_NONE, do_firefly_red_blink_none},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_WHITE_BLINK_NONE, do_firefly_white_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_red_blink_none[] = {
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_RED_BLINK_1S, do_firefly_red_blink_1s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_OFF, SM_FIREFLY, SM_FIREFLY_RED_BLINK_5S, do_firefly_red_blink_5s}, 
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_red_blink_1s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_RED_BLINK_1S, do_firefly_red_blink_1s}, 
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_RED_BLINK_5S, do_firefly_red_blink_5s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_RED_BLINK_NONE, do_firefly_red_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_red_blink_5s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_RED_BLINK_5S, do_firefly_red_blink_5s}, 
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_NONE, do_firefly_org_blink_none},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_RED_BLINK_NONE, do_firefly_red_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_org_blink_none[] = {
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_1S, do_firefly_org_blink_1s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_OFF, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_5S, do_firefly_org_blink_5s}, 
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_org_blink_1s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_1S, do_firefly_org_blink_1s}, 
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_5S, do_firefly_org_blink_5s},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_NONE, do_firefly_org_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_org_blink_5s[] = {
  {EV_TIMEO, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_5S, do_firefly_org_blink_5s}, 
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_UTR_BLINK_NONE, do_firefly_utr_blink_none},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_ADP_ON, SM_FIREFLY, SM_FIREFLY_ORG_BLINK_NONE, do_firefly_org_blink_none},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_utr_blink_none[] = {
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_NONE, do_firefly_flu_blink_none},
  {EV_KEY_MOD_LPRESS, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_LP, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {NULL, NULL, NULL, NULL}
};

static const struct sm_trans_slot code  sm_trans_firefly_power_off[] = {
  {EV_INIT, SM_FIREFLY, SM_FIREFLY_POWER_OFF, do_firefly_power_off},
  {EV_KEY_MOD_PRESS, SM_FIREFLY, SM_FIREFLY_FLU_BLINK_NONE, do_firefly_flu_blink_none},
  {NULL, NULL, NULL, NULL}
};

const struct sm_state_slot code sm_function_firefly[] = {
  {"SM_FIREFLY_INIT", sm_trans_firefly_init},
  {"SM_FIREFLY_FLU_BLINK_NONE", sm_trans_firefly_flu_blink_none},
  {"SM_FIREFLY_FLU_BLINK_5S", sm_trans_firefly_flu_blink_5s},
  {"SM_FIREFLY_FLU_BLINK_15S", sm_trans_firefly_flu_blink_15s},
  {"SM_FIREFLY_WHITE_BLINK_NONE", sm_trans_firefly_white_blink_none},
  {"SM_FIREFLY_WHITE_BLINK_1S", sm_trans_firefly_white_blink_1s}, 
  {"SM_FIREFLY_WHITE_BLINK_5S", sm_trans_firefly_white_blink_5s},
  {"SM_FIREFLY_RED_BLINK_NONE", sm_trans_firefly_red_blink_none},
  {"SM_FIREFLY_RED_BLINK_1S", sm_trans_firefly_red_blink_1s}, 
  {"SM_FIREFLY_RED_BLINK_5S", sm_trans_firefly_red_blink_5s},
  {"SM_FIREFLY_ORG_BLINK_NONE", sm_trans_firefly_org_blink_none},
  {"SM_FIREFLY_ORG_BLINK_1S", sm_trans_firefly_org_blink_1s},
  {"SM_FIREFLY_ORG_BLINK_5S", sm_trans_firefly_org_blink_5s},
  {"SM_FIREFLY_UTR_BLINK_NONE", sm_trans_firefly_utr_blink_none},
  {"SM_FIREFLY_POWER_OFF", sm_trans_firefly_power_off},
};