 /*
 * File:   APP_int.h
 * Author: Abdelrhman Werby
 * 
 * This file contain all the prototype of the tasks functions and the 
 * Application Initialization and de-Initialization function.
 *
 * Created on July 7, 2020, 1:17 AM
 */

#ifndef APP_INT_H
#define	APP_INT_H

/* Hash Defines */
#define I2C_SPEED   50000
#define HERE_WE_ARE_AGAIN   0x97
#define EEPROM_HERE_BFORE_ADDRESS   0x30
#define EEPROM_SAVE_TEMP_ADDRESS    0x00
#define TEMP_THRSH 5
#define NUM_READING 10
#define LOCK 1
#define UNLOCK 0
#define SET_TEMP_TIME_OUT 5000
#define INIT_TEMP   60
#define MIN_TEMP    35 
#define MAX_TEMP    75

/* SET TEMP entered by the user */
extern uint8_t Goal_temp;
/* Current TEMP of the water */
extern uint16_t Current_temp;


/**
 * Initialization of all MCU peripherals, OS scheduler and crate the tasks.
 * Check for last value saved in EEPROM. 
 * MCU peripherals : GPIO pins, ADC, EXTI, TIM0, I2C.
 * EXT peripherals : Seven segment display.
 *
 * @param void.
 * @return void.
 */
void APP_Init(void);

/**
 * DE-initialization of all MCU peripherals.
 *
 * @param void.
 * @return void.
 */
void APP_DeInit(void);

/**
 * Set Temp task run every 10 'ms', check if the user press the '+/'-' buttons.
 * if True  : Take the new set temp and save it to EEPROM. 
 * 
 * @param void.
 * @return void.
 */
void Set_Temp(void);

/**
 * Update temp task run every 100 'ms', Read the LM35 temp with ADC and scale
 * the value.
 * Max LM35 Output = 1500 'mv' when the temp is 150 C corresponding to 307 in ADC Range.
 * Current_temp = (ADC_Value * 150) / 307 .
 * 
 * @param void.
 * @return void.
 */
void Update_Temp(void);



/**
 * Control Temp task run every 1000 'ms', Controlling the heater and cooler.
 * Heater-ON/Cooler-OFF if : AVG_TEMP <= SET_TEMP - 5. 
 * Heater-OFF/Cooler-ON if : AVG_TEMP >= SET_TEMP + 5.
 * 
 * @param void.
 * @return void.
 */
void Control_Temp(void);


#endif	/* XC_HEADER_TEMPLATE_H */

