#include "I2C_LCD.h"

#define MODE_JUICE 0
#define MODE_WATER 1
#define MODE_STATUS 2

void JCR_Lcd_Init();

void JCR_Lcd_Clear();
void JCR_Lcd_Print(char *str, uint8_t col, uint8_t line);

void JCR_LcdPrint_Process();

void JCR_Lcd_EXTI_Callback(uint16_t GPIO_Pin);

uint8_t JCR_Lcd_GetSettingMode();