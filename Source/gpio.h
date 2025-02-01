#ifndef __FIREFLY_GPIO_H__
#define __FIREFLY_GPIO_H__

#include <STC8G.H>
/* com */
// P3.0 as RxD
// P3.1 as TxD
sbit SERIAL_RXD   = P3 ^ 0;
sbit SERIAL_TXD   = P3 ^ 1;

/* i2c */
sbit I2C_SDA      = P5 ^ 4;
sbit I2C_SCL      = P5 ^ 5;

/* power en */
sbit POWER_EN      = P3 ^ 2;

/* INT */
sbit INT_HUB       = P3 ^ 3;

void gpio_initialize (void);
void gpio_enable_serial(bit enable);
void gpio_enable_hub(bit enable);
#endif
