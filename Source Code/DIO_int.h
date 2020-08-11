/*
 * DIO_int.h
 *
 *  Created on: Sep 29, 2018
 *      Author: werby
 */

#ifndef _DIO_INT_H
#define _DIO_INT_H

/* Port Defines */
#define GPIOA    0
#define GPIOB    1
#define GPIOC    2
#define GPIOD    3

/* PIN Defines */
#define PIN0   0
#define PIN1   1
#define PIN2   2
#define PIN3   3
#define PIN4   4
#define PIN5   5
#define PIN6   6
#define PIN7   7

/* PIN Directions */
#define INPUT  0xff
#define OUTPUT 0x00

/* PIN Value Options */
#define HIGH   0xff
#define LOW    0x00

/* IO Pins */
uint8_t DIO_uint8_tGetPinValue(uint8_t PortId , uint8_t PinNumber) ;
void DIO_VidSetPinValue(uint8_t PortId , uint8_t Pin , uint8_t Value);
void DIO_VidSetPinDirection(uint8_t PortId , uint8_t Pin , uint8_t Value);


/* IO Ports */
uint8_t DIO_uint8_tGetPortValue(uint8_t PortId);
void DIO_VidSetPortValue(uint8_t PortId , uint8_t Value);
void DIO_VidSetPortDirection(uint8_t PortId , uint8_t Value);




#endif
