#include <STC8G.H>

#include "int_hub.h"
#include "gpio.h"
#include "debug.h"
#include "tdc.h"
#include "task.h"
#include "i2c.h"
#include "sm.h"
#include "button.h"
#include "power.h"

#define INT_HUB_I2C_ADDR 0x70 /* 0 1 1 1 A2 A1 A0 R/W*/

static uint8_t saved_state;

#define INT_HUB_CB_CNT 2
static INT_PROC code int_hub_cb[INT_HUB_CB_CNT] = 
{
  button_int_cb,
  power_int_cb,
};

void int_hub_proc(enum task_events ev)
{
  uint8_t new_state;
  uint8_t i;
  
  CDBG("int_hub_proc ev = %s\n", task_names[ev]);
  
  new_state = int_hub_get_state();
  
  for(i = 0 ; i < INT_HUB_CB_CNT ; i ++) {
    if(int_hub_cb[i](saved_state, new_state))
      break;
  }
  
  saved_state = new_state;
  
  sm_run(ev);
}

static void int_hub_isr (void) interrupt 2 using 1
{
  task_set(EV_EX_INT);
  IE1 = 0;
}

void int_hub_dump()
{
  uint8_t dat;
  I2C_Init();
  
  I2C_Get(INT_HUB_I2C_ADDR, 0x0, &dat);
  CDBG("int_hub: input port 0x%02bx\n", dat);
  
  I2C_Get(INT_HUB_I2C_ADDR, 0x1, &dat);
  CDBG("int_hub: output port 0x%02bx\n", dat);
  
  I2C_Get(INT_HUB_I2C_ADDR, 0x2, &dat);
  CDBG("int_hub: polarity inversion 0x%02bx\n", dat);
  
  I2C_Get(INT_HUB_I2C_ADDR, 0x3, &dat);
  CDBG("int_hub: config 0x%02bx\n", dat);   
}

uint8_t int_hub_get_state(void)
{
  uint8_t dat;
  I2C_Get(INT_HUB_I2C_ADDR, 0x0, &dat);
  CDBG("int_hub_get_state 0x%02bx\n", dat);
  return dat;
}

void int_hub_initilize(void)
{
  CDBG("int_hub_initilize\n");
  I2C_Init();
  
  /* all port set to input */
  I2C_Put(INT_HUB_I2C_ADDR, 0x3, 0xff); 
  /* no Polarity Inversion */
  I2C_Put(INT_HUB_I2C_ADDR, 0x2, 0x0); 
  /* Output Port set 1 */
  I2C_Put(INT_HUB_I2C_ADDR, 0x1, 0xff);  
  
  int_hub_dump();
  
  saved_state = int_hub_get_state();
  
  EX1 = 1;
  IT1 = 1; // 下降沿触发
}

static void int_hub_sets(uint8_t port_mask, uint8_t addr, bit b)
{
  uint8_t dat;
  
  CDBG("int_hub_sets [%bu][%c]: %c\n", (uint8_t)port_mask, 
    addr == 0x0 ? 'I': (addr == 0x1 ? 'O' : (addr == 0x2 ? 'P' : (addr == 0x3 ? 'C' : '-')) ), 
    b ? '1' : '0');
  
  I2C_Get(INT_HUB_I2C_ADDR, addr, &dat);

  if(b) {
    dat |= port_mask;
  } else {
    dat &= ~port_mask;
  }
  I2C_Put(INT_HUB_I2C_ADDR, addr, dat);
}

static void int_hub_set(int_hub_port_t port, uint8_t addr, bit b)
{
  uint8_t dat;
  
  CDBG("int_hub_set [%bu][%c]: %c\n", (uint8_t)port, 
    addr == 0x0 ? 'I': (addr == 0x1 ? 'O' : (addr == 0x2 ? 'P' : (addr == 0x3 ? 'C' : '-')) ), 
    b ? '1' : '0');
  
  I2C_Get(INT_HUB_I2C_ADDR, addr, &dat);
  if(b) {
    dat |= (1 << port);
  } else {
    dat &= ~(1 << port);
  }
  I2C_Put(INT_HUB_I2C_ADDR, addr, dat);
}

static bit int_hub_get(int_hub_port_t port, uint8_t addr)
{
  uint8_t dat;
  I2C_Get(INT_HUB_I2C_ADDR, addr, &dat);
  return (dat & (1 << port)) != 0;
}

void int_hub_set_polarity_inversion(int_hub_port_t port, bit invert)
{
  int_hub_set(port, 0x2, invert);
}

bit int_hub_get_polarity_inversion(int_hub_port_t port)
{
  return int_hub_get(port, 0x2);
}

void int_hub_set_mode(int_hub_port_t port, int_hub_mod_t mod)
{
  int_hub_set(port, 0x3, mod == INT_HUB_MOD_INPUT ? 1 : 0);
}

int_hub_mod_t int_hub_get_mode(int_hub_port_t port)
{
  return (int_hub_mod_t)int_hub_get(port, 0x3);
}

void int_hub_set_out_port(int_hub_port_t port, bit enable)
{
  int_hub_set(port, 0x1, enable);
}

bit int_hub_get_out_port(int_hub_port_t port)
{
  return int_hub_get(port, 0x1);
}

bit int_hub_get_in_port(int_hub_port_t port)
{
  return int_hub_get(port, 0x0);
}

void int_hub_set_polarity_inversions(uint8_t port_mask, bit invert)
{
  int_hub_sets(port_mask, 0x2, invert);
}

void int_hub_set_modes(uint8_t port_mask, int_hub_mod_t mod)
{
  int_hub_sets(port_mask, 0x3, mod == INT_HUB_MOD_INPUT ? 1 : 0);
}

void int_hub_set_out_ports(uint8_t port_mask, bit enable)
{
  int_hub_sets(port_mask, 0x1, enable);
}
