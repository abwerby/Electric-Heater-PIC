/*
 * File:   APP_prog.c
 * Author: Abdelrhman Werby
 * 
 * This file contain all the implementation of the tasks functions and the 
 * Application Initialization and de-Initialization function.
 *
 * Created on July 7, 2020, 1:17 AM
 */

#define _XTAL_FREQ  8000000
#include <xc.h>
#include <stdint.h>
#include "BIT_MATH.h"
#include "DIO_int.h" 
#include "SCH_int.h"
#include "SS_int.h"
#include "ADC_int.h"
#include "EXTI_int.h"
#include "I2C_int.h"
#include "EEPROM_int.h"
#include "App_int.h"



/* Globle varibales */
uint8_t Goal_temp = INIT_TEMP;
uint16_t Current_temp = 0;
uint16_t Avg_temp = 0;
uint8_t Heater_Led_state = 0;
uint8_t Heater_state = 0;
uint16_t time_out = SET_TEMP_TIME_OUT;
uint8_t SS_Locked = UNLOCK;


/**
 * Initialization of all MCU peripherals, OS scheduler and crate the tasks.
 * Check for last value saved in EEPROM. 
 * MCU peripherals : GPIO pins, ADC, EXTI, TIM0, I2C.
 * EXT peripherals : Seven segment display.
 *
 * @param void.
 * @return void.
 */
void APP_Init(void)
{
    /* ADC PIN */
    DIO_VidSetPinDirection(GPIOA, PIN2, INPUT);
    /* Heater Pin */
    DIO_VidSetPinDirection(GPIOC, PIN5, OUTPUT);
    /* Cooler  Pin */
    DIO_VidSetPinDirection(GPIOC, PIN2, OUTPUT);
    /* Heater Led Pin */
    DIO_VidSetPinDirection(GPIOB, PIN5, OUTPUT);
    /* ON/OFF EXT-Interrupt  */
    DIO_VidSetPinDirection(GPIOB, PIN0, INPUT);
    /* Temp '+' Button  */
    DIO_VidSetPinDirection(GPIOB, PIN1, INPUT);
    /* Temp '-' Button */
    DIO_VidSetPinDirection(GPIOB, PIN2, INPUT);
    /* Turn ON All PORTB pull up resistor */
    SET_BIT(OPTION_REG, 7);


    DIO_VidSetPinDirection(GPIOB, PIN7, OUTPUT);
    
    /* init timer-0 for scheduler */
    SCH_Init_T0();
    /* init pin RB2 INT0 ON/OFF Button*/
    EXTI_Init();
    /* init Seven segment Display pins */
    SS_Init();
    /* init ADC */
    ADC_vidInit();
    /* init I2C */
    I2C_Master_Init(I2C_SPEED);
    /* Create Set temp Task with 0 'ms' delay and 10 'ms' periodic time */
    SCH_Add_Task(Set_Temp, 0, 50);
    /* Create Update_Temp Task with 10 'ms' delay and 100 'ms' periodic time */
    SCH_Add_Task(Update_Temp,  10, 100);
    /* Create Control_Temp Task with 20 'ms' delay and 1000 'ms' periodic time */
    SCH_Add_Task(Control_Temp, 20, 1000);
    
    /* check if this the first time the MCU RUN and the EEPROM is Empty*/
    if (EEPROM_Read(EEPROM_HERE_BFORE_ADDRESS) != HERE_WE_ARE_AGAIN)
    {
        EEPROM_Write(EEPROM_HERE_BFORE_ADDRESS, HERE_WE_ARE_AGAIN);
        __delay_ms(10);
        EEPROM_Write(EEPROM_SAVE_TEMP_ADDRESS, Goal_temp);
        __delay_ms(10);
    }
    else
    {
        /* OOH here we are again.. so, Read temp saved in the EEPROM */
        Goal_temp = EEPROM_Read(EEPROM_SAVE_TEMP_ADDRESS);
    }
    
}

/**
 * DE-initialization of all MCU peripherals.
 *
 * @param void.
 * @return void.
 */
void APP_DeInit(void)
{
    /* SS Display OFF*/
    DIO_VidSetPortValue(SS_Data_PORT, LOW);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN0, LOW);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN1, LOW);

    /* Heater OFF */
    Heater_state = LOW;
    DIO_VidSetPinValue(GPIOC, PIN5, Heater_state);
    /* Cooler OFF */
    DIO_VidSetPinValue(GPIOC, PIN2, LOW);
    /* Heater LED OFF */
    DIO_VidSetPinValue(GPIOB, PIN5, LOW);
    /* Reset AVG-temp */
    Avg_temp = 0;
}

/**
 * Update temp task run every 100 'ms', Read the LM35 temp with ADC and scale
 * the value.
 * Max LM35 Output = 1500 'mv' when the temp is 150 C corresponding to 307 in ADC Range.
 * Current_temp = (ADC_Value * 150) / 307 .
 * 
 * @param void.
 * @return void.
 */
void Update_Temp(void)
{
    Current_temp = ((uint32_t)ADC_GetAdValue(ADC_CHANNEL_2)*150)/307;
    Avg_temp += Current_temp;
}

/**
 * Control Temp task run every 1000 'ms', Controlling the heater and cooler.
 * Heater-ON/Cooler-OFF if : AVG_TEMP <= SET_TEMP - 5. 
 * Heater-OFF/Cooler-ON if : AVG_TEMP >= SET_TEMP + 5.
 * 
 * @param void.
 * @return void.
 */
void Control_Temp(void)
{
    Avg_temp = Avg_temp / NUM_READING;
    if (Avg_temp <= Goal_temp - TEMP_THRSH)
    {
        /* Heater ON */
        Heater_state = HIGH;
        DIO_VidSetPinValue(GPIOC, PIN5, Heater_state);
        /* Cooler OFF */
        DIO_VidSetPinValue(GPIOC, PIN2, LOW);
    }
    else if (Avg_temp >= Goal_temp + TEMP_THRSH)
    {
        /* Heater OFF */
        Heater_state = LOW;
        DIO_VidSetPinValue(GPIOC, PIN5, Heater_state);
        /* Cooler ON */
        DIO_VidSetPinValue(GPIOC, PIN2, HIGH);
        /* Heater LED ON */
        DIO_VidSetPinValue(GPIOB, PIN5, HIGH);
    }
    if (Heater_state == HIGH)
    {
        Heater_Led_state = ~Heater_Led_state; 
        DIO_VidSetPinValue(GPIOB, PIN5, Heater_Led_state);
    }
    /* Reset Average Temp */
    Avg_temp = 0;

}

/**
 * Set Temp task run every 10 'ms', check if the user press the '+/'-' buttons.
 * if True  : Take the new set temp and save it to EEPROM. 
 * 
 * @param void.
 * @return void.
 */
void Set_Temp(void)
{
    if (DIO_uint8_tGetPinValue(GPIOB, PIN1) == LOW || DIO_uint8_tGetPinValue(GPIOB, PIN2) == LOW)
    {   
        uint8_t DIS_TOG = 0xff;
        /* software de-bounce */
        while(DIO_uint8_tGetPinValue(GPIOB, PIN1) == LOW || DIO_uint8_tGetPinValue(GPIOB, PIN2) == LOW);
        /* save the press time */
        uint32_t last_press_time = SYS_TICK;
        uint32_t last_toggel_time = SYS_TICK;
        /* Loop for 5 seconds (using -> SYS_TICK) */
        while (SYS_TICK - last_press_time < SET_TEMP_TIME_OUT && SCH_state != SCH_STOP)
        {   
            if (DIO_uint8_tGetPinValue(GPIOB, PIN1) == LOW && Goal_temp > MIN_TEMP)
            {
                /* software de-bounce */
                while(DIO_uint8_tGetPinValue(GPIOB, PIN1) == LOW);
                /* save the press time */
                last_press_time = SYS_TICK;
                Goal_temp -= 5;
            }
            else if (DIO_uint8_tGetPinValue(GPIOB, PIN2) == LOW && Goal_temp < MAX_TEMP)
            {
                /* software de-bounce */
                while(DIO_uint8_tGetPinValue(GPIOB, PIN2) == LOW);
                /* save the press time */
                last_press_time = SYS_TICK;
                Goal_temp += 5;
            }
            
            /* Toggle the SS Display every 1000 ms */
            if (SYS_TICK - last_toggel_time > 1000)
            {
                last_toggel_time = SYS_TICK;
                DIS_TOG = ~DIS_TOG;

            }
            else if (DIS_TOG == 0xff)
            {
                /* Display the User Set Temp*/
                SS_Display(Goal_temp);
            }
            else
            {
                /* Turn OFF SS Display */
                DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN0, LOW);
                DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN1, LOW);
            } 
            
            /* Reset Average Temp */
            Avg_temp = 0;
            /* Heater OFF for safety */
            Heater_state = LOW;
            DIO_VidSetPinValue(GPIOC, PIN5, Heater_state);
            /* Cooler OFF for safety */
            DIO_VidSetPinValue(GPIOC, PIN2, LOW);
            /* Heater LED OFF */
            DIO_VidSetPinValue(GPIOB, PIN5, LOW);
        }
        /* Save the new Goal TEMP entered by the user to EEPROM */
        EEPROM_Write(EEPROM_SAVE_TEMP_ADDRESS, Goal_temp);        
    }

}