/*
 * File:   ADC_Prog.c
 * Author: abdelrhman
 *
 * Created on July 6, 2020, 12:03 AM
 */

#define _XTAL_FREQ  8000000

#include <xc.h>
#include <stdint.h>
#include "BIT_MATH.h"
#include "ADC_int.h"


void ADC_vidInit(void)
{
    /* A/D Port Configuration Control bits */
    CLR_BIT(ADCON1, 0);
    CLR_BIT(ADCON1, 1);
    CLR_BIT(ADCON1, 2);
    CLR_BIT(ADCON1, 3);
    /* A/D Result Format Select bit ( Right justified ) */
    SET_BIT(ADCON1, 7);
    /* A/D Conversion Clock Select bits (FOSC/2)*/
    SET_BIT(ADCON0, 6);
    CLR_BIT(ADCON0, 7);
    SET_BIT(ADCON1, 6);
    /* A/D On bit */
    SET_BIT(ADCON0, 0);
     
}

uint16_t ADC_GetAdValue(uint8_t ChannelId)
{
    /* Analog Channel Select bits */
    ADCON0 |= ChannelId << 3;
    /* A/D Conversion Status bit */
    SET_BIT(ADCON0, 2);
    /* wait Conversion to End */
    while(GET_BIT(ADCON0, 2) == 1);
    /* return A/D value*/
    return ((ADRESH << 8) + ADRESL);
    
}