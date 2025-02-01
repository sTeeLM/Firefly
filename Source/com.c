#include <STC8G.H>
#include <stdio.h>

#include "debug.h"
#include "com.h"
#include "cext.h"
#include "delay.h"

#define    OLEN  32                             // size of serial transmission buffer
static unsigned   char  ostart;                 // transmission buffer start index
static unsigned   char  oend;                   // transmission buffer end index
static char idata outbuf[OLEN];                 // storage for transmission buffer

#define    ILEN  32                              // size of serial receiving buffer
static unsigned   char  istart;                 // receiving buffer start index
static unsigned   char  iend;                   // receiving buffer end index
static char idata inbuf[ILEN];                  // storage for receiving buffer

bit sendfull;                                   // flag: marks transmit buffer full
bit sendactive;                                 // flag: marks transmitter active

/*--------------------------------------------------------------------------------
 * Serial Interrupt Service Routine
 *------------------------------------------------------------------------------*/
static void com_isr (void) interrupt 4 using 2
{
char c;

  //----- Received data interrupt. -----
  if (RI) {
    c = SBUF;                                   // read character
    RI = 0;                                     // clear interrupt request flag
    if (istart + ILEN != iend) {
      inbuf[iend++ & (ILEN-1)] = c;             // put character into buffer
    }
  }

  //----- Transmitted data interrupt. -----
  if (TI != 0) {
    TI = 0;                                     // clear interrupt request flag
    if (ostart != oend) {                       // if characters in buffer and
      SBUF = outbuf[ostart++ & (OLEN-1)];       // transmit character
      sendfull = 0;                             // clear 'sendfull' flag
    }
    else {                                      // if all characters transmitted
      sendactive = 0;                           // clear 'sendactive'
    }
  }
}


/*--------------------------------------------------------------------------------
 * Function to initialize the serial port and the UART baudrate.
 *------------------------------------------------------------------------------*/
void com_initialize (void) {
  istart = 0;                                  // empty transmit buffers
  iend = 0;
  ostart = 0;                                  // empty transmit buffers
  oend = 0;
  sendactive = 0;                              // transmitter is not active
  sendfull = 0;                                // clear 'sendfull' flag

  /* 波特率 115200 */
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFA;			//设置定时初始值
	TH1 = 0xFA;			//设置定时重载值
	ET1 = 0;			  //禁止定时器中断
	TR1 = 1;			  //定时器1开始计时

  REN = 1;
  ES = 1;                                      // enable serial interrupts

}

//   SCON  = 0xDA;        // SCON: mode 1, 8-bit UART, enable rcvr      */
//   TMOD |= 0x20;        // TMOD: timer 1, mode 2, 8-bit reload        */
//   TH1   = 0xFD;        // TH1:  reload value for 1200 baud @ 12MHz   */
//   TR1   = 1;           // TR1:  timer 1 run                          */
//   TI    = 1;           // TI:   set TI to send first char of UART    */

/*--------------------------------------------------------------------------------
 * putbuf: write a character to SBUF or transmission buffer
 *------------------------------------------------------------------------------*/
void putbuf (char c) {
  if (!sendfull) {                             // transmit only if buffer not full
    if (!sendactive) {                         // if transmitter not active:
      sendactive = 1;                          // transfer first character direct
      SBUF = c;                                // to SBUF to start transmission
    }
    else {
      ES = 0;                                  // disable serial interrupts during buffer update
      outbuf[oend++ & (OLEN-1)] = c;           // put char to transmission buffer
      if (((oend ^ ostart) & (OLEN-1)) == 0) {
         sendfull = 1;
      }                                        // set flag if buffer is full
      ES = 1;                                  // enable serial interrupts again
    }
  }
}


/*--------------------------------------------------------------------------------
 * Replacement routine for the standard library putchar routine.
 * The printf function uses putchar to output a character.
 *------------------------------------------------------------------------------*/
char putchar (char c) {
  if (c == '\n') {                             // expand new line character:
    while (sendfull);                          // wait until there is space in buffer
    putbuf (0x0D);                             // send CR before LF for <new line>
  }
  while (sendfull);                            // wait until there is space in buffer
  putbuf (c);                                  // place character into buffer
  return (c);
}


/*--------------------------------------------------------------------------------
 * Replacement routine for the standard library _getkey routine.
 * The getchar and gets functions uses _getkey to read a character.
 *------------------------------------------------------------------------------*/
char _getkey (void) {
  char c;
  while (iend == istart) {
     ;                                         // wait until there are characters
  }
  ES = 0;                                      // disable serial interrupts during buffer update
  c = inbuf[istart++ & (ILEN-1)];
  ES = 1;                                      // enable serial interrupts again
  return (c);
}

bit com_try_get_char(char * c) 
{
  if (iend == istart) {
     return 0;                                         // wait until there are characters
  }
  ES = 0;                                      // disable serial interrupts during buffer update
  *c = inbuf[istart++ & (ILEN-1)];
  ES = 1;                                      // enable serial interrupts again
  return 1;
}


/*
ret: 
1: ok, full bytes received
0: timeout, non or partial bytes received
*/
bit com_recv_buffer(uint8_t * buffer, uint16_t * len, uint16_t timeoms)
{
  uint16_t save_len;
  char c;
  
  save_len = *len;
  *len = 0;
  
  while(1) {
    if(com_try_get_char(&c)) {
      buffer[(*len)++] = c;
      if((*len) == save_len)
        break;
    } else {
      if(timeoms > 0) {
        delay_ms(1);
        if(timeoms --)
          break;
      }
    }
  }
  
  return save_len == *len;
}

void com_send_buffer(uint8_t * buffer, uint16_t len)
{
  uint8_t i = 0;
  
  while(len --) {
    while (sendfull);
    putbuf(buffer[i++]);
  }
}
