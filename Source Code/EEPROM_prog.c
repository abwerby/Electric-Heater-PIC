/*
 * File:   EEPROM_prog.c
 * Author: abdelrhman
 *
 * Created on July 7, 2020, 10:00 PM
 */


#include <xc.h>
#include <stdint.h>
#include "I2C_int.h"
#include "EEPROM_int.h"


void EEPROM_Write(uint8_t add, uint8_t data)
{
    I2C_Master_Start();
    // Wait Until EEPROM Is IDLE
    while(I2C_Master_Write(EEPROM_Address_W))
      I2C_Master_RepeatedStart();
    I2C_Master_Write((uint8_t)add);
    I2C_Master_Write(data);
    I2C_Master_Stop();
}


uint8_t EEPROM_Read(uint8_t add)
{
    uint8_t Data;
    I2C_Master_Start();
    // Wait Until EEPROM Is IDLE
    while(I2C_Master_Write(EEPROM_Address_W))
      I2C_Master_RepeatedStart();

    I2C_Master_Write(add);
    I2C_Master_Start(); // I2C_Master_RepeatedStart();

    I2C_Master_Write(EEPROM_Address_R);
    Data = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return Data;
}
