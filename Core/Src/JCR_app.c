#include "JCR_app.h"

#include <stdint.h>
#include <stdio.h>

#include "JCR_containers.h"
#include "JCR_dispenser.h"
#include "JCR_key.h"
#include "JCR_lcd.h"
#include "JCR_mug.h"
#include "JCR_pumps.h"
#include "JCR_sensor.h"
#include "JCR_uart.h"
#include "VL53L0X.h"
#include "i2c.h"
#include "main.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "JCR_uart.h"


static JCR_AppState_t appState = APP_STATE_IDLE;
static uint32_t stateStartTime = 0;

char distanceBuf[52];
uint16_t distance;
statInfo_t_VL53L0X distanceStr;

void JCR_App_Init() {
    JCR_Mug_Init();
    JCR_Pumps_Init();
    JCR_Key_Init();
    JCR_Lcd_Init();
    JCR_dispenser_init();
    JCR_uart_init();

    appState = APP_STATE_IDLE;

    for (uint8_t i = 0; i < 52; i++) {
        distanceBuf[i] = ' ';
    }

    /* Sensor1 */
    HAL_GPIO_WritePin(XSHUT1_GPIO_Port, XSHUT1_Pin,
                      1);  // enable sensor1  || 0x53
    HAL_GPIO_WritePin(XSHUT2_GPIO_Port, XSHUT2_Pin,
                      0);  // disable sensor2 || 0x54

    HAL_Delay(100);
    setActiveAddress_VL53L0X(0x52);

    // Initialise the VL53L0X
    initVL53L0X(1, &hi2c3);
    setAddress_VL53L0X(0x60);

    // Configure the sensor for high accuracy and speed in 20 cm.
    setSignalRateLimit(200);
    setVcselPulsePeriod(VcselPeriodPreRange, 10);
    setVcselPulsePeriod(VcselPeriodFinalRange, 14);
    setMeasurementTimingBudget(3000 * 1000UL);
    startContinuous(0);

    /* Sensor2 */
    HAL_GPIO_WritePin(XSHUT1_GPIO_Port, XSHUT1_Pin,
                      1);  // disable sensor1   || 0x53
    HAL_GPIO_WritePin(XSHUT2_GPIO_Port, XSHUT2_Pin,
                      1);  // enable sensor2   || 0x54

    HAL_Delay(100);
    setActiveAddress_VL53L0X(0x52);

    // Initialise the VL53L0X
    initVL53L0X(1, &hi2c3);
    setAddress_VL53L0X(0x62);

    // Configure the sensor for high accuracy and speed in 20 cm.
    setSignalRateLimit(200);
    setVcselPulsePeriod(VcselPeriodPreRange, 10);
    setVcselPulsePeriod(VcselPeriodFinalRange, 14);
    setMeasurementTimingBudget(3000 * 1000UL);
    startContinuous(0);

    HAL_Delay(100);

    JCR_Mug_EXTI_Callback(MUG_DET_Pin);
}

void JCR_App_CheckMugPresent() {
    if (!JCR_Mug_IsPresent()) {
        JCR_PumpJuice_Off();
        JCR_PumpWater_Off();
        appState = APP_STATE_IDLE;
    }
}

void JCR_App_SetState(JCR_AppState_t state) {
    appState = state;
    stateStartTime = HAL_GetTick();
}

JCR_AppState_t JCR_App_GetState() { return appState; }

void JCR_App_Process() {
    JCR_Mug_Process();
    JCR_sensor_process();
    JCR_Containers_Process();
    JCR_LcdPrint_Process();
    JCR_uart_Process();

    switch (appState) {
        case APP_STATE_IDLE:
            if (JCR_Key_IsPressed() || JCR_uart_IsStartPressed()) {
                if (JCR_Mug_IsDetected()) {
                    JCR_Mug_ClearDetected();
                    JCR_uart_SetStartPressed(false);
                    stateStartTime = HAL_GetTick();
                    appState = APP_STATE_JUICE;

                    JCR_Containers_StartVolumeMeasurementJuice();
                    JCR_Containers_StartVolumeMeasurementWater();
                } else {
                    JCR_dispenser_dispenseCup();
                    HAL_Delay(500);
                }
            }
            break;

        case APP_STATE_JUICE:
            if (HAL_GetTick() - stateStartTime >= JUICE_PUMP_MAX_TIME_MS) {
                JCR_PumpJuice_Off();
                JCR_PumpWater_On();
                appState = APP_STATE_WATER;

                stateStartTime = HAL_GetTick();
            } else {
                JCR_PumpJuice_On();
                JCR_PumpWater_Off();
            }

            JCR_App_CheckMugPresent();
            break;

        case APP_STATE_WATER:
            if (HAL_GetTick() - stateStartTime >= WATER_PUMP_MAX_TIME_MS) {
                JCR_PumpWater_Off();
                appState = APP_STATE_DONE;
            } else {
                JCR_PumpJuice_Off();
                JCR_PumpWater_On();
            }

            JCR_App_CheckMugPresent();
            break;

        case APP_STATE_DONE:
            JCR_PumpJuice_Off();
            JCR_PumpWater_Off();
            if (!JCR_Mug_IsPresent()) appState = APP_STATE_IDLE;

            break;
    }
}
