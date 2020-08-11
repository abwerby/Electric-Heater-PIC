/*
 * DIO.c
 *
 *  Created on: Sep 29, 2018
 *      Author: werby
 */
#include <xc.h>
#include "BIT_MATH.h"
#include "stdint.h"
#include "DIO_reg.h"
#include "DIO_int.h"




void DIO_VidSetPortDirection(uint8_t PortId , uint8_t Value)
{
	switch(PortId)
	{
	case 0 :
		DDRA_Reg = Value ;
		break;
	case 1 :
		DDRB_Reg = Value ;
		break;
	case 2 :
		DDRC_Reg = Value ;
		break;
	case 3 :
		DDRD_Reg = Value ;
		break;
	}
}

void DIO_VidSetPortValue(uint8_t PortId , uint8_t Value)
{
	switch(PortId)
	{
	case 0 :
		PORTA_Reg = Value ;
		break;
	case 1 :
		PORTB_Reg = Value ;
		break;
	case 2 :
		PORTC_Reg = Value ;
		break;
	case 3 :
		PORTD_Reg = Value ;
		break;
	}
}

void DIO_VidSetPinDirection(uint8_t PortId , uint8_t Pin , uint8_t Value)
{
	switch(PortId)
	{
	case 0 :
		if(Value == 0xff)
		{
			SET_BIT(DDRA_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(DDRA_Reg,Pin);
		}
		break;
	case 1 :
		if(Value == 0xff)
		{
			SET_BIT(DDRB_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(DDRB_Reg,Pin);
		}
		break;
	case 2 :
		if(Value == 0xff)
		{
			SET_BIT(DDRC_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(DDRC_Reg,Pin);
		}
		break;
	case 3 :
		if(Value == 0xff)
		{
			SET_BIT(DDRD_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(DDRD_Reg,Pin);
		}
		break;
	}
}

void DIO_VidSetPinValue(uint8_t PortId , uint8_t Pin , uint8_t Value)
{
	switch(PortId)
	{
	case 0 :
		if(Value == 0xff)
		{
			SET_BIT(PORTA_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(PORTA_Reg,Pin);
		}
		break;
	case 1 : 
		if(Value == 0xff)
		{
			SET_BIT(PORTB_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(PORTB_Reg,Pin);
		}
		break;
	case 2 :
		if(Value == 0xff)
		{
			SET_BIT(PORTC_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(PORTC_Reg,Pin);
		}
		break;
	case 3 :
		if(Value == 0xff)
		{
			SET_BIT(PORTD_Reg,Pin);
		}
		else if(Value == 0)
		{
			CLR_BIT(PORTD_Reg,Pin);
		}
		break;
	}
}

uint8_t DIO_uint8_tGetPortValue(uint8_t PortId)
{
	uint8_t Value ;
	switch(PortId)
	{
	case 0 :
		 Value = PORTA_Reg ;
		break;
	case 1 :
		Value  = PORTB_Reg ;
		break;
	case 2 :
		 Value = PORTC_Reg ;
		break;
	case 3 :
		 Value = PORTD_Reg ;
		break;
	}
	return Value ;
}

uint8_t DIO_uint8_tGetPinValue(uint8_t PortId , uint8_t PinNumber)
{
	uint8_t Value ;
	switch(PortId)
	{
	case 0 :
		 Value = GET_BIT(PORTA_Reg,PinNumber) ;
		break;
	case 1 :
		Value = GET_BIT(PORTB_Reg,PinNumber) ;
		break;
	case 2 :
		 Value = GET_BIT(PORTC_Reg,PinNumber) ;
		break;
	case 3 :
		 Value = GET_BIT(PORTD_Reg,PinNumber) ;
		break;
	}
	return Value ;
}
