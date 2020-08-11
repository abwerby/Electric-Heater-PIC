
#ifndef SS_INT_H
#define	SS_INT_H


#define SS_Data_PORT        GPIOD
#define SS_Control_PORT     GPIOA
#define SS_Conrol_PIN0      PIN4
#define SS_Conrol_PIN1      PIN5

/**
 * Control Temp task run every 1000 'ms', Controlling the heater and cooler.
 * Heater-ON/Cooler-OFF if : AVG_TEMP <= SET_TEMP - 5. 
 * Heater-OFF/Cooler-ON if : AVG_TEMP >= SET_TEMP + 5.
 * 
 * @param void.
 * @return void.
 */
void SS_Init();

/**
 * Function to display number in the seven segment display.
 * 
 * @param num to be printed.
 * @return void.
 */
void SS_Display(uint8_t num);

#endif	/* XC_HEADER_TEMPLATE_H */

