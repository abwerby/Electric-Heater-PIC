
#ifndef I2C_INT_H
#define	I2C_INT_H


void I2C_Master_Init(const uint32_t baud);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
void I2C_ACK();
void I2C_NACK();
uint8_t I2C_Master_Write(uint8_t);
uint8_t I2C_Read_Byte(void);


#endif	

