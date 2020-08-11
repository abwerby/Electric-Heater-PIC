/*
 * File:   EXTI_prog.c
 * Author: abdelrhman
 *
 * Created on July 7, 2020, 12:43 AM
 */


#include <xc.h>
#include <stdint.h>
#include "BIT_MATH.h"
#include "EXTI_int.h"


void EXTI_Init(void)
{
    INTEDG = 1;
    INTE = 1;
    GIE = 1;
}