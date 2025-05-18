#include "JCR_lcd.h"

#include "stm32f4xx_hal.h"

void JCR_Lcd_Init() {
    I2C_LCD_Init(I2C_LCD_1);
    I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
    I2C_LCD_WriteString(I2C_LCD_1, "    MIKSOSOK");
    I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
    I2C_LCD_WriteString(I2C_LCD_1, "================");

    HAL_Delay(1000);
}

void JCR_Lcd_Clear() { I2C_LCD_Clear(I2C_LCD_1); }

void JCR_Lcd_Print(char *str, uint8_t col, uint8_t line) {
    I2C_LCD_SetCursor(I2C_LCD_1, col, line);
    I2C_LCD_WriteString(I2C_LCD_1, str);
}