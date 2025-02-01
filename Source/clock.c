#include <STC8G.H>

#include "clock.h"
#include "sm.h"
#include "debug.h"
#include "task.h"

// 时钟频率 11059200 HZ
static uint32_t jiff;
static uint32_t sec_now;
static uint8_t timer_sec;
void clock_initialize(void)
{
  CDBG("clock_initialize\n");
  // 输出时钟频率 = (SYSclk)/12/(65536-[RL_TH0, RL_TL0])/2 = 16HZ
  TR0 = 0;        //定时器禁止工作
  AUXR &= ~0x80;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式，16位自动重载
	TL0 = 0x80;			//设置定时初始值
	TH0 = 0x0F;			//设置定时初始值
	ET0 = 0;			  //禁止定时器中断
  
  sec_now = 0;
  jiff    = 0;
  
  TR0 = 1;        //定时器允许工作
  ET0 = 1;        //定时器允许中断
}

static void clock_isr (void) interrupt 1 using 1
{
  jiff ++;
  if(jiff % 4 == 0) {
    task_set(EV_250MS);
    if(jiff % 16 == 0) {
      sec_now ++;
      task_set(EV_1S);
      if(timer_sec) {
        if(!(--timer_sec)) {
          task_set(EV_TIMEO);
        }
      }
    }
  }
  task_set(EV_KEY_SCAN); // 每秒扫描16次
  TF0 = 0;
}

void clock_time_proc(enum task_events ev)
{
  sm_run(ev);
}

uint32_t clock_get_now_sec(void)
{
  return sec_now;
}


uint32_t clock_diff_now_sec(uint32_t sec)
{
  return (uint32_t)(sec_now - sec);
}

uint32_t clock_get_now_jiff(void)
{
  return jiff;
}

uint32_t clock_diff_now_jiff(uint32_t saved_jiff)
{
  return (uint32_t)(jiff - saved_jiff);
}

void clock_set_timer(uint8_t sec)
{
  timer_sec = sec;
}