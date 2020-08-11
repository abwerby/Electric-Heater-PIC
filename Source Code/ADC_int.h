
#ifndef ADC_INT_H
#define	ADC_INT_H


#define ADC_CHANNEL_0  0x00
#define ADC_CHANNEL_1  0x01
#define ADC_CHANNEL_2  0x02
#define ADC_CHANNEL_3  0x03
#define ADC_CHANNEL_4  0x04
#define ADC_CHANNEL_5  0x05
#define ADC_CHANNEL_6  0x06
#define ADC_CHANNEL_7  0x07


void ADC_vidInit(void);
uint16_t ADC_GetAdValue(uint8_t ChannelId);



#endif	/* XC_HEADER_TEMPLATE_H */

