#ifndef __FIREFLY_LED_H__
#define __FIREFLY_LED_H__

#include <stdint.h>

typedef enum _led_type_t
{
  LED_TYPE_FLU = 0, // fluorescence
  LED_TYPE_WHITE,
  LED_TYPE_RED,
  LED_TYPE_ORG,
  LED_TYPE_UTR,
}led_type_t;

void led_initialize(void);
void led_enable(led_type_t type, bit enable);
void led_power_enable(bit enable);
void led_blink(led_type_t type, uint16_t delayms, uint8_t cnt);
void led_clean(void);
#endif