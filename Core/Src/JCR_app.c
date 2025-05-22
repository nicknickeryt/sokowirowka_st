#include "JCR_app.h"

#include <stdio.h>

#include "JCR_key.h"
#include "JCR_lcd.h"
#include "JCR_mug.h"
#include "JCR_pumps.h"
#include "JCR_sr04.h"
#include "JCR_containers.h"

#include "main.h"
#include "stm32f4xx_hal.h"

static JCR_AppState_t appState = APP_STATE_IDLE;
static uint32_t stateStartTime = 0;
static uint32_t lastLcdUpdateTime = 0;

void JCR_App_Init() {
    JCR_Mug_Init();
    JCR_Pumps_Init();
    JCR_Key_Init();
    JCR_Lcd_Init();
    JCR_sr04_Init();

    appState = APP_STATE_IDLE;
}

void JCR_App_CheckMugPresent() {
    if (!JCR_Mug_IsPresent()) {
        JCR_PumpJuice_Off();
        JCR_PumpWater_Off();
        appState = APP_STATE_IDLE;
    }
}

void JCR_App_LcdPrint_Process() {
    uint32_t now = HAL_GetTick();
    if (now - lastLcdUpdateTime < 100) return;

    JCR_Lcd_Clear();

    char bufHeight[16];
    char bufVolume[16];

    sprintf(bufHeight, "%lu mm", JCR_sr04_GetDistanceWater());
    JCR_Lcd_Print(bufHeight, 0, 0);

    sprintf(bufHeight, "%lu mm", JCR_sr04_GetDistanceJuice());
    JCR_Lcd_Print(bufHeight, 0, 1);

    // sprintf(bufVolume, "%lu ml", (uint32_t)JCR_Containers_GetVolumeDeltaCcm());
    // JCR_Lcd_Print(bufVolume, 0, 1);

    lastLcdUpdateTime = now;
}

void JCR_App_SetState(JCR_AppState_t state) {
    appState = state;
    stateStartTime = HAL_GetTick();
}

void JCR_App_Process() { 
    JCR_Mug_Process();
    JCR_sr04_Process();
    JCR_Containers_Process();

    JCR_App_LcdPrint_Process();

    switch (appState) {
        case APP_STATE_IDLE:
            if (JCR_Mug_IsDetected() && JCR_Key_IsPressed()) {
                JCR_Mug_ClearDetected();
                JCR_PumpJuice_On();
                JCR_PumpWater_Off();
                stateStartTime = HAL_GetTick();
                appState = APP_STATE_JUICE;
            }
            break;

        case APP_STATE_JUICE:
            if (HAL_GetTick() - stateStartTime >= JUICE_PUMP_TIME_MS) {
                JCR_PumpJuice_Off();
                JCR_PumpWater_On();
                stateStartTime = HAL_GetTick();
                appState = APP_STATE_WATER;
                JCR_Containers_StartVolumeMeasurement();
            }
            JCR_Containers_Process();

            JCR_App_CheckMugPresent();
            break;

        case APP_STATE_WATER:
            if (HAL_GetTick() - stateStartTime >= WATER_PUMP_MAX_TIME_MS) {
                JCR_PumpWater_Off();
                appState = APP_STATE_DONE;
            }
            JCR_App_CheckMugPresent();
            break;

        case APP_STATE_DONE:
            if (!JCR_Mug_IsPresent()) {
                appState = APP_STATE_IDLE;
            }
            break;
    }
}
