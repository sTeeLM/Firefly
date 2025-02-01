#ifndef __FIREFLY_TASK_H__
#define __FIREFLY_TASK_H__

#include <stdint.h>

// max 32
enum task_events
{
  EV_INIT                = 0,
  EV_KEY_SCAN            = 1, // 扫描按键
  EV_EX_INT              = 2, // 有外部中断  
  EV_ADP_ON              = 3, // adapter pluged on
  EV_ADP_OFF             = 4, // adapter pluged off
  EV_250MS               = 5, // 大约每250ms转一下
  EV_1S                  = 6, // 大约每1s转一下 
  EV_KEY_MOD_DOWN        = 7, // mod按下 
  EV_KEY_MOD_PRESS       = 8, // mod键短按
  EV_KEY_MOD_LPRESS      = 9, // mod键长按
  EV_KEY_MOD_UP          = 10, // mod抬起
  EV_TIMEO               = 11, // 超时（或者虚拟按键）
  EV_LP                  = 12, // low power
  EV_CNT  
};

extern uint16_t ev_bits0;
extern uint16_t ev_bits1;

extern const char * code task_names[];

typedef void (*TASK_PROC)(enum task_events);

void task_initialize (void);

void task_dump(void);

// 这些宏也在中断里被调用，所以不能是带参数函数，只能拿宏实现了
#define task_set(ev1)             \
  do{                             \
    if(ev1 < 16)                  \
      ev_bits0 |= 1<<ev1;         \
    else                          \
      ev_bits1 |= 1<<(ev1 - 16);  \
  }while(0)

#define task_clr(ev1)               \
  do{                               \
    if(ev1 < 16)                    \
      ev_bits0 &= ~(1<<ev1);        \
    else                            \
      ev_bits1 &= ~(1<<(ev1 - 16)); \
  }while(0)
    
#define  task_test(ev1)             \
  (ev1 < 16 ? (ev_bits0 & (1<<ev1)) : (ev_bits1 & (1<<(ev1 - 16))))

void task_run(void);

#endif