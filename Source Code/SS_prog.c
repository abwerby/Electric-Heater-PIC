/*
 * File:   SS_prog.c
 * Author: abdelrhman
 *
 * Created on July 5, 2020, 8:49 PM
 */

#define _XTAL_FREQ  8000000
#include <xc.h>
#include <stdint.h>
#include "DIO_int.h"
#include "SS_int.h"

uint8_t segments_code[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};

void SS_Init()
{
    DIO_VidSetPortDirection(SS_Data_PORT, OUTPUT);
    DIO_VidSetPinDirection(SS_Control_PORT, SS_Conrol_PIN0, OUTPUT);
    DIO_VidSetPinDirection(SS_Control_PORT, SS_Conrol_PIN1, OUTPUT);
}

void SS_Display(uint8_t num)
{

    DIO_VidSetPortValue(SS_Data_PORT, segments_code[num%10]);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN0, LOW);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN1, HIGH);
    __delay_ms(5);
    DIO_VidSetPortValue(SS_Data_PORT, segments_code[(num/10)%10]);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN0, HIGH);
    DIO_VidSetPinValue(SS_Control_PORT, SS_Conrol_PIN1, LOW);
    __delay_ms(5);
}