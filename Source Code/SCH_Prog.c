/*
 * File:   SCH_Prog.c
 * Author: abdelrhman
 *
 * This file contain the scheduler function implementation 
 * and the ISR function for the TIM0 and EXTI (RB0) pin.
 * 
 * Created on July 6, 2020, 11:39 AM
 */


#include <xc.h>
#include <stdint.h>
#include "BIT_MATH.h"
#include "SCH_int.h"
#include "App_int.h"
#include "EEPROM_int.h"
#include "DIO_int.h"

#define NUM_OVF 8
#define TIM0_PRELOAD_Value 48

uint8_t num_ovf = 0;
sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t SCH_state = SCH_STOP;
uint32_t SYS_TICK = 0;

void TIM0_Init(void)
{
    
    /* pre-load value */
    TMR0 = TIM0_PRELOAD_Value;
    /* CLK source */
    CLR_BIT(OPTION_REG, 5);
    SET_BIT(OPTION_REG, 3);
    /* set the Pre-scaler Rate Select bits */
    CLR_BIT(OPTION_REG, 0);
    CLR_BIT(OPTION_REG, 1);
    CLR_BIT(OPTION_REG, 2);
    SET_BIT(INTCON, 2);           // Clear The Interrupt Flag Bit   (TMR0IF)
    SET_BIT(INTCON, 4);           //enable the external interrupt   (INTE)
    

}

/**
 * SCH_Add_Task()
 * Causes a task (function) to be executed at regular intervals
 * or after a user-defined delay
 * @param Pointer to function.
 * @param Delay time.
 * @param periodic time.
 * @return sum of `values`, or 0.0 if `values` is empty.
 */
uint8_t SCH_Add_Task(void (* pFunction)(), const uint16_t DELAY, const uint16_t PERIOD)
{
    uint8_t Index = 0;
    // First find a gap in the array (if there is one)
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }
    // Have we reached the end of the list?
    if (Index == SCH_MAX_TASKS)
    {
        return SCH_MAX_TASKS;
    }
    // If we're here, there is a space in the task array
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    return Index; // return position of task (to allow later deletion)
}

/**
 * SCH_Init_T0()
 * Scheduler initialization function. Prepares scheduler
 * data structures and sets up timer interrupts at required rate.
 * Must call this function before using the scheduler.
 * @param void.
 * @return void.
 */
void SCH_Init_T0(void)
{
    uint8_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_Delete_Task(i);
    }
    // Now set up Timer 0
    TIM0_Init();
}

/*------------------------------------------------------------------*-
SCH_Delete_Task()
Removes a task from the scheduler. Note that this does
*not* delete the associated function from memory:
it simply means that it is no longer called by the scheduler.
TASK_INDEX - The task index. Provided by SCH_Add_Task().
RETURN VALUE: RETURN_ERROR or RETURN_NORMAL
-*------------------------------------------------------------------*/
uint8_t SCH_Delete_Task(const uint8_t TASK_INDEX)
{
    uint8_t Return_code;
    if (SCH_tasks_G[TASK_INDEX].pTask == 0)
    {
        Return_code = RETURN_ERROR;
    }
    else
    {
        Return_code = RETURN_NORMAL;
    }
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
    return Return_code; 
}


/**
 * SCH_Dispatch_Tasks()
 * This is the 'dispatcher' function. When a task (function)
 * is due to run, SCH_Dispatch_Tasks() will run it.
 * This function must be called (repeatedly) from the main loop.
 * 
 * @param void.
 * @return void.
 */
void SCH_Dispatch_Tasks(void)
{
    uint8_t Index;
    // Dispatches (runs) the next task (if one is ready)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if (SCH_tasks_G[Index].RunMe > 0)
        {
            (*SCH_tasks_G[Index].pTask)(); // Run the task
            SCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag
            // Periodic tasks will automatically run again
            // if this is a ' one shot ' task, remove it from the array
            if (SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
}

void SCH_Start(void)
{
    SET_BIT(INTCON, 5);           // Timer0 Interrupt Enable Bit  (TMR0IE)
}

void SCH_Stop(void)
{
    CLR_BIT(INTCON, 5);           // Timer0 Interrupt Enable Bit  (TMR0IE)
}

void __interrupt () ISR(void)
{
    /* Check The Timer 0 Flag Bit */
    if (TMR0IF)
    {
        num_ovf++;
        if (num_ovf == NUM_OVF)
        {
             for (uint8_t Index = 0; Index < SCH_MAX_TASKS; Index++)
             {
                 // Check if there is a task at this location
                 if (SCH_tasks_G[Index].pTask)
                 {
                     if (SCH_tasks_G[Index].Delay == 0)
                     {
                         // The task is due to run
                         SCH_tasks_G[Index].RunMe += 1; // Inc. the 'RunMe' flag
                         if (SCH_tasks_G[Index].Period)
                         {
                             // Schedule periodic tasks to run again
                             SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                         }
                     }
                     else
                     {
                         // Not yet ready to run: just decrement the delay
                         SCH_tasks_G[Index].Delay -= 1;
                     }
                 }
             }
             SYS_TICK++;
             num_ovf = 0;
             TMR0 = TIM0_PRELOAD_Value;
        }
         /* Clear The Flag Bit */
         TMR0IF = 0; 
    }
  
    /* Check The EXTI Flag on Pin RB0 */
    if (INTF)   
    {
      if (SCH_state == SCH_STOP)
      {
          Goal_temp = EEPROM_Read(EEPROM_SAVE_TEMP_ADDRESS);
          /* check if data is valid */
          if(Goal_temp > 75 || Goal_temp < 35)
          {
            /* Back to init value of temp */
            Goal_temp = INIT_TEMP;
          }
          SCH_state = SCH_RUNNING;
          SCH_Start();
      }
      else if (SCH_state == SCH_RUNNING)
      {
        SCH_state = SCH_STOP;
        SCH_Stop();
      }
       INTF = 0;  //  Clear The Flag
    }
}