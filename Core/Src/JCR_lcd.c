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

    char line0[17] = "                ";
    char line1[17] = "                ";

    if (settingMode == MODE_STATUS) {

        #define CONTAINER_MIN_MM 28
        #define CONTAINER_MAX_MM 237
        #define BAR_WIDTH 15
    
        uint32_t juiceLevel = JCR_sensor_GetDistanceJuice();
        uint32_t waterLevel = JCR_sensor_GetDistanceWater();
    
        // Ograniczenie wartości
        if (juiceLevel < CONTAINER_MIN_MM) juiceLevel = CONTAINER_MIN_MM;
        if (juiceLevel > CONTAINER_MAX_MM) juiceLevel = CONTAINER_MAX_MM;
        if (waterLevel < CONTAINER_MIN_MM) waterLevel = CONTAINER_MIN_MM;
        if (waterLevel > CONTAINER_MAX_MM) waterLevel = CONTAINER_MAX_MM;
    
        // Oblicz poziom wypełnienia (0.0 - 1.0)
        float juiceRatio = 1.0f - ((float)(juiceLevel - CONTAINER_MIN_MM) / (CONTAINER_MAX_MM - CONTAINER_MIN_MM));
        float waterRatio = 1.0f - ((float)(waterLevel - CONTAINER_MIN_MM) / (CONTAINER_MAX_MM - CONTAINER_MIN_MM));
    
        // Oblicz liczbę znaków wypełnienia
        int juiceBars = (int)(juiceRatio * BAR_WIDTH + 0.5f);
        int waterBars = (int)(waterRatio * BAR_WIDTH + 0.5f);
    
        char line0[20];
        char line1[20];
    
        // Budowanie pasków
        line0[0] = 'S';
        line1[0] = 'W';
        for (int i = 0; i < BAR_WIDTH; i++) {
            line0[i + 1] = (i < juiceBars) ? '#' : '_';
            line1[i + 1] = (i < waterBars) ? '#' : '_';
        }
        line0[BAR_WIDTH + 1] = '\0';
        line1[BAR_WIDTH + 1] = '\0';
    
        JCR_Lcd_Clear();
        JCR_Lcd_Print(line0, 0, 0);
        JCR_Lcd_Print(line1, 0, 1);
        return;
    }
    

    if (!pumpActive) {
        if (now - lastBlink >= 200) {
            lastBlink = now;
            blinkState = !blinkState;
        }
    } else {
        blinkState = 1; // Always show values, no blink
    }


    uint32_t targetJ = JCR_Containers_GetTargetMlJuice();
    uint32_t targetW = JCR_Containers_GetTargetMlWater();
    uint32_t currJ = JCR_Containers_GetVolumeDeltaCcmJuice();
    uint32_t currW = JCR_Containers_GetVolumeDeltaCcmWater();

    sprintf(line1, "%3luml      %3luml", currJ, (uint32_t) ((float) currW / 1.19f));
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
        sprintf(line0, " PROSZE CZEKAC");
    }


    JCR_Lcd_Clear();
    JCR_Lcd_Print(line0, 0, 0);
    JCR_Lcd_Print(line1, 0, 1);
}

void JCR_Lcd_SwitchSettingMode() { settingMode = settingMode == MODE_STATUS ? MODE_JUICE : settingMode + 1; }

void JCR_Lcd_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == BUTTON_ENCODER_Pin) {
        if (!HAL_GPIO_ReadPin(BUTTON_ENCODER_GPIO_Port, BUTTON_ENCODER_Pin))
            JCR_Lcd_SwitchSettingMode();
    }
}

uint8_t JCR_Lcd_GetSettingMode() {
    return settingMode;
}