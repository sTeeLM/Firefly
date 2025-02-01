#include <intrins.h>
#include "delay.h"
#include "cext.h"


// 时钟频率11059200
static void internal_delay_10us(void) 
{
  uint8_t data i;

	_nop_();
	i = 35;
  while (--i);
}

static void internal_delay_ms(void)
{
	uint8_t data i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

void delay_10us(uint32_t t)
{
  while(t --) {
    internal_delay_10us();
  }
}

void delay_ms(uint32_t t) 
{     
  while(t--) {      
    internal_delay_ms();
  } 
}
