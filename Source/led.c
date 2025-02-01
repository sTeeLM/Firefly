#include "led.h"
#include "debug.h"
#include "int_hub.h"
#include "delay.h"

void led_initialize(void)
{
	CDBG("led_initialize\n");
}

void led_clean(void)
{
  led_enable(LED_TYPE_FLU, 0);
  led_enable(LED_TYPE_WHITE, 0);
  led_enable(LED_TYPE_RED, 0);
  led_enable(LED_TYPE_ORG, 0); 
  led_enable(LED_TYPE_UTR, 0);  
}

void led_power_enable(bit enable)
{
  uint8_t led_mask = 
    INT_HUB_MASK_P3
    |INT_HUB_MASK_P4
    |INT_HUB_MASK_P5
    |INT_HUB_MASK_P6
    |INT_HUB_MASK_P7;
  
  if(enable) {
    /* 开启LED电源 */
    int_hub_set_out_port(INT_HUB_P1, 1);
    int_hub_set_mode(INT_HUB_P1, INT_HUB_MOD_OUTPUT);
    
    int_hub_set_out_ports(led_mask, 0);
    int_hub_set_modes(led_mask, INT_HUB_MOD_OUTPUT);

    
  } else { /* 设置为input, 减少漏电 */
    /* 关闭LED电源 */
    int_hub_set_mode(INT_HUB_P1, INT_HUB_MOD_OUTPUT);
    int_hub_set_out_port(INT_HUB_P1, 0);    
    
    /* 关闭LED电源 */
    int_hub_set_mode(INT_HUB_P1, INT_HUB_MOD_INPUT);
    int_hub_set_out_port(INT_HUB_P1, 0);
    
    delay_ms(200); // 等待放电，不然LED会闪
    
    int_hub_set_modes(led_mask, INT_HUB_MOD_INPUT);
    int_hub_set_out_ports(led_mask, 1);
  }
}

void led_enable(led_type_t type, bit enable)
{
  switch(type) {
    case LED_TYPE_FLU:
      int_hub_set_out_port(INT_HUB_P3, enable);
      break;
    case LED_TYPE_WHITE:
      int_hub_set_out_port(INT_HUB_P4, enable);
      break;
    case LED_TYPE_RED:
      int_hub_set_out_port(INT_HUB_P5, enable);
      break;
    case LED_TYPE_ORG:
      int_hub_set_out_port(INT_HUB_P6, enable);
      break;
    case LED_TYPE_UTR:
      int_hub_set_out_port(INT_HUB_P7, enable);
      break;
  }
}

void led_blink(led_type_t type, uint16_t delayms, uint8_t cnt)
{
  while(cnt --) {
    led_enable(type, 1);
    delay_ms(delayms);
    led_enable(type, 0);
    delay_ms(delayms);
  }
}
