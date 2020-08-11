/*
 * File:   main.c
 * Author: Abdelrhman Werby
 *
 * This file contain the main function and the super loop of the entire system
 * 
 * Created on July 4, 2020, 9:32 AM
 */


#include "config.h"
#include <xc.h>
#include <stdint.h>
#include "SCH_int.h"
#include "App_int.h"
#include "SS_int.h"


int main(void) 
{
    /* Initialization of all MCU peripherals */
    APP_Init();
    
    /* Super Loop */
    while(1)
    {
        /* check the scheduler state (NOTE: only changed by the EXTI ISR)*/
        if (SCH_state == SCH_RUNNING)
        {
            SCH_Dispatch_Tasks();
            SS_Display(Current_temp);
        }
        else 
        {
            /* De-initialize all MCU peripherals and go to sleep to reduce power */
            APP_DeInit();
            SLEEP();
        }
        
    }
    return 0;
}

