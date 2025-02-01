#ifndef __FIREFLY_TDC_H__
#define __FIREFLY_TDC_H__

#include <stdint.h>

typedef enum _tdc_type_t
{
  TDC_TYPE_NONE, // none
  TDC_TYPE_1S,
  TDC_TYPE_5S,
  TDC_TYPE_15S, 
} tdc_type_t;

void tdc_initilize(void);
void tdc_trigger(tdc_type_t type);
void tdc_trigger_sleep(tdc_type_t type);

#endif
