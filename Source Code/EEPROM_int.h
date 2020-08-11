
#ifndef EEPROM_INT_H
#define	EEPROM_INT_H

/* EEPROM read address */
#define EEPROM_Address_R 0xA1
/* EEPROM write address */
#define EEPROM_Address_W 0xA0

/**
 * Function to write single byte of data to EEPROM.
 * 
 * @param Address of location to save the data.
 * @param Data to save.
 * @return void.
 */
void EEPROM_Write(uint8_t add, uint8_t data);

/**
 * Function to read single byte from Address location in EEPROM. 
 * 
 * @param Address of location to read the data.
 * @return void.
 */
uint8_t EEPROM_Read(uint8_t add);

#endif	/* XC_HEADER_TEMPLATE_H */

