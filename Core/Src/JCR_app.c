#include "JCR_app.h"

#include "JCR_key.h"
#include "JCR_mug.h"
#include "JCR_pumps.h"
#include "JCR_lcd.h"
#include "JCR_sr04.h"

#include "main.h"
#include "stm32f4xx_hal.h"

typedef enum {
    APP_STATE_IDLE = 0,
    APP_STATE_JUICE,
    APP_STATE_WATER,
    APP_STATE_DONE
} JCR_AppState_t;

static JCR_AppState_t appState = APP_STATE_IDLE;
static uint32_t stateStartTime = 0;

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

void JCR_App_Process() {
    JCR_Mug_Process();
    JCR_sr04_Process();

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
            }
            JCR_App_CheckMugPresent();
            break;

        case APP_STATE_WATER:
            if (HAL_GetTick() - stateStartTime >= WATER_PUMP_TIME_MS) {
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
