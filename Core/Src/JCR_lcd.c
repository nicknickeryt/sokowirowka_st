#include "JCR_lcd.h"

#include <stdio.h>

#include "JCR_containers.h"
#include "JCR_sensor.h"
#include "JCR_pumps.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"


void JCR_Lcd_Init() {
    I2C_LCD_Init(I2C_LCD_1);
    I2C_LCD_SetCursor(I2C_LCD_1, 0, 0);
    I2C_LCD_WriteString(I2C_LCD_1, "    MIKSOSOK");
    I2C_LCD_SetCursor(I2C_LCD_1, 0, 1);
    I2C_LCD_WriteString(I2C_LCD_1, "================");

    HAL_Delay(0);
}

void JCR_Lcd_Clear() { I2C_LCD_Clear(I2C_LCD_1); }

void JCR_Lcd_Print(char *str, uint8_t col, uint8_t line) {
    I2C_LCD_SetCursor(I2C_LCD_1, col, line);
    I2C_LCD_WriteString(I2C_LCD_1, str);
}

static uint8_t settingMode = 0;

void JCR_LcdPrint_Process() {
    uint32_t now = HAL_GetTick();
    static uint32_t lastUpdate = 0;
    static uint32_t lastBlink = 0;
    static uint8_t blinkState = 1;

    if (now - lastUpdate < 1) return;
    lastUpdate = now;

    // Check if either pump is active
    bool pumpActive = JCR_PumpJuice_IsOn() || JCR_PumpWater_IsOn();

    if (!pumpActive) {
        if (now - lastBlink >= 200) {
            lastBlink = now;
            blinkState = !blinkState;
        }
    } else {
        blinkState = 1; // Always show values, no blink
    }

    char line0[17] = "                ";
    char line1[17] = "                ";

    uint32_t targetJ = JCR_Containers_GetTargetMlJuice();
    uint32_t targetW = JCR_Containers_GetTargetMlWater();
    uint32_t currJ = JCR_Containers_GetVolumeDeltaCcmJuice();
    uint32_t currW = JCR_Containers_GetVolumeDeltaCcmWater();

    if (!pumpActive) {
        if (settingMode == MODE_JUICE) {
            if (!blinkState)
                sprintf(line0, "           %3luml", targetW);  // Hide juice label
            else
                sprintf(line0, "%3luml      %3luml", targetJ, targetW);
        } else if (settingMode == MODE_WATER) {
            if (!blinkState)
                sprintf(line0, "%3luml           ", targetJ);  // Hide water label
            else
                sprintf(line0, "%3luml      %3luml", targetJ, targetW);
        } else
            sprintf(line0, "%3luml      %3luml", targetJ, targetW);
    } else {
        sprintf(line0, "%3luml      %3luml", targetJ, targetW);
    }

    sprintf(line1, "%3luml      %3luml", currJ, (uint32_t) ((float) currW / 1.19f));

    JCR_Lcd_Clear();
    JCR_Lcd_Print(line0, 0, 0);
    JCR_Lcd_Print(line1, 0, 1);
}

void JCR_Lcd_SwitchSettingMode() { settingMode = !settingMode; }

void JCR_Lcd_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_ENCODER_Pin) {
        if (!HAL_GPIO_ReadPin(BUTTON_ENCODER_GPIO_Port, BUTTON_ENCODER_Pin))
            JCR_Lcd_SwitchSettingMode();
    }
}

uint8_t JCR_Lcd_GetSettingMode() {
    return settingMode;
}