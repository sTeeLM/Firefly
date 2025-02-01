#ifndef __FIREFLY_I2C_H__
#define __FIREFLY_I2C_H__

void I2C_Init();
void I2C_Delay();
void I2C_Start();
void I2C_Stop();
void I2C_Write(uint8_t dat);
uint8_t I2C_Read();
bit I2C_GetAck();
void I2C_PutAck(bit ack);

bit I2C_Put(uint8_t SlaveAddr, uint8_t SubAddr, uint8_t dat);  
bit I2C_Get(uint8_t SlaveAddr, uint8_t SubAddr, uint8_t * dat);

bit I2C_Gets(uint8_t SlaveAddr, uint8_t SubAddr, uint8_t Size, uint8_t *dat);
bit I2C_Puts(uint8_t SlaveAddr, uint8_t SubAddr, uint8_t Size, uint8_t *dat);

#endif
