 /*
 * File:   SCH_int.h
 * Author: abdelrhman
 *
 * @brief This file contain the scheduler functions prototypes and the Hash defines to interface with the scheduler.
 * 
 * Created on July 6, 2020, 11:39 AM
 */

#ifndef SCH_INT_H
#define	SCH_INT_H

/*----- Hash Defines ----- */

#define SCH_MAX_TASKS (3)  /* Number of tasks */
#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (0xAA)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (0xAA)
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (0xA0)
#define ERROR_SCH_LOST_SLAVE (0x80)
#define RETURN_NORMAL  0
#define RETURN_ERROR   1
#define SCH_RUNNING 1
#define SCH_STOP    0


extern uint8_t SCH_state;
extern uint32_t SYS_TICK;

/**
 * sTask data struct store all data associate with each task
 * 
 * pTasks: pointer to task function.
 * Delay:  Delay (ticks) until the function will (next) be run
 * Period: Interval (ticks) between subsequent runs..
 * RunMe:  Incremented (by scheduler) when task is due to execute.
 *
 */
typedef struct data
{
    // Pointer to the task (must be a 'void (void)' function)
    void (* pTask)(void);
    // Delay (ticks) until the function will (next) be run
    uint16_t Delay;
    // Interval (ticks) between subsequent runs.
    uint16_t Period;
    // Incremented (by scheduler) when task is due to execute
    uint8_t RunMe;
} sTask;



/* ------ Public function prototypes -------- */

/**
 * SCH_Init_T0()
 * Scheduler initialization function. Prepares scheduler
 * data structures and sets up timer interrupts at required rate.
 * Must call this function before using the scheduler.
 * @param void.
 * @return void.
 */
void SCH_Init_T0(void);

/**
 * SCH_Dispatch_Tasks()
 * This is the 'dispatcher' function. When a task (function)
 * is due to run, SCH_Dispatch_Tasks() will run it.
 * This function must be called (repeatedly) from the main loop.
 * 
 * @param void.
 * @return void.
 */
void SCH_Dispatch_Tasks(void);

/**
 * SCH_Start()
 * Start the scheduler by enable the TIM0 interrupt.
 * @param void.
 * @return void.
 */
void SCH_Start(void);

/**
 * SCH_Stop()
 * Stop the scheduler by disable the TIM0 interrupt.
 * @param void.
 * @return void.
 */
void SCH_Stop(void);

/**
 * SCH_Add_Task()
 * Causes a task (function) to be executed at regular intervals
 * or after a user-defined delay
 * 
 * @param Pointer to function.
 * @param Delay time.
 * @param periodic time.
 * @return Error state.
 */
uint8_t SCH_Add_Task(void (*) (void), const uint16_t, const uint16_t);

/**
 * SCH_Delete_Task()
 * Removes a task from the scheduler. Note that this does
 * *not* delete the associated function from memory:
 * it simply means that it is no longer called by the scheduler.
 * 
 * @param TASK_INDEX - The task index. Provided by SCH_Add_Task().
 * @return RETURN_ERROR or RETURN_NORMAL.
 */
uint8_t SCH_Delete_Task(const uint8_t);



/* ------- Private function prototype ------- */
void TIM0_Init();


#endif	/* XC_HEADER_TEMPLATE_H */

