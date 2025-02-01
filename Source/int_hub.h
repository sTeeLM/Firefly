#ifndef __FIREFLY_INT_HUB_H__
#define __FIREFLY_INT_HUB_H__

#include "task.h"

#include <stdint.h>

typedef enum _int_hub_port_t
{
  INT_HUB_P0 = 0,
  INT_HUB_P1,  
  INT_HUB_P2,
  INT_HUB_P3,
  INT_HUB_P4,  
  INT_HUB_P5,
  INT_HUB_P6,
  INT_HUB_P7,  
} int_hub_port_t;

#define INT_HUB_MASK_P0 0x1
#define INT_HUB_MASK_P1 0x2
#define INT_HUB_MASK_P2 0x4
#define INT_HUB_MASK_P3 0x8
#define INT_HUB_MASK_P4 0x10
#define INT_HUB_MASK_P5 0x20
#define INT_HUB_MASK_P6 0x40
#define INT_HUB_MASK_P7 0x80

typedef enum _int_hub_mod_t
{
  INT_HUB_MOD_OUTPUT,  
  INT_HUB_MOD_INPUT,
} int_hub_mod_t;

typedef bit (*INT_PROC)(uint8_t old_state, uint8_t new_state);
uint8_t int_hub_get_state(void);

void int_hub_initilize(void);
void int_hub_set_polarity_inversion(int_hub_port_t port, bit invert);
void int_hub_set_polarity_inversions(uint8_t port_mask, bit invert);
bit int_hub_get_polarity_inversion(int_hub_port_t port);

void int_hub_set_mode(int_hub_port_t port, int_hub_mod_t mod);
void int_hub_set_modes(uint8_t port_mask, int_hub_mod_t mod);
int_hub_mod_t int_hub_get_mode(int_hub_port_t port);

void int_hub_set_out_port(int_hub_port_t port, bit enable);
void int_hub_set_out_ports(uint8_t port_mask, bit enable);
  
bit int_hub_get_out_port(int_hub_port_t port);
bit int_hub_get_in_port(int_hub_port_t port);

void int_hub_proc(enum task_events ev);
void int_hub_dump(void);
#endif
