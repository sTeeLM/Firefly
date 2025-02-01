#include "gpio.h"
#include "debug.h"


void gpio_initialize(void)
{
  P0M0 = 0x00;
  P0M1 = 0xFF;
  P1M0 = 0x00;
  P1M1 = 0xFF;
  P2M0 = 0x00;
  P2M1 = 0xFF;
  P3M0 = 0x04; // POWER_EN推挽输出
  P3M1 = 0xFB;
  P4M0 = 0x00;
  P4M1 = 0xFF;
  P5M0 = 0x00;
  P5M1 = 0xFF;
  P_SW1 = 0x00;
  
  I2C_SDA   = 1;
  I2C_SCL   = 1;
  POWER_EN  = 0;
  INT_HUB   = 1;  
  
}

/*
 enable serial port
 P3 ^ 0 准双向
 P3 ^ 1 准双向
*/
void gpio_enable_serial(bit enable)
{
  if(enable) {
    P3M0 &= ~0x03;
    P3M1 &= ~0x03;
  } else {
    P3M0 &= ~0x03;
    P3M1 |= 0x03;
  }
}
/*
  P5 ^ 4 准双向
  P5 ^ 5 准双向
*/
void gpio_enable_hub(bit enable)
{
  if(enable) {
    P5M0 &= ~0x30;
    P5M1 &= ~0x30;
  } else {
    P5M0 &= ~0x30;
    P5M1 |= 0x30;
  }   
}

