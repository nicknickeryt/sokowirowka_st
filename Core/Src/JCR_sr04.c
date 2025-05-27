#include "JCR_sr04.h"

#include <stdint.h>
#include <stdio.h>

#include "JCR_lcd.h"
#include "main.h"
#include "sr04.h"
#include "tim.h"

#include "VL53L0X.h"


SENSOR_State sensor_state = SENSOR_IDLE;
uint32_t sr04_measurement_start = 0;

static float sr04_water_ema_distance = 0.0f;
static float sr04_juice_ema_distance = 0.0f;

#define SR04_EMA_ALPHA 0.5f

void SR04_UpdateEma()
{
    // float newValue = (float)sr04_water.distance;
    setActiveAddress_VL53L0X(0x60);
    statInfo_t_VL53L0X distanceStr;
    float newValue = readRangeContinuousMillimeters(&distanceStr);
    sr04_water_ema_distance = sr04_water_ema_distance == 0.0f
                                  ? newValue
                                  : SR04_EMA_ALPHA * newValue + (1.0f - SR04_EMA_ALPHA) * sr04_water_ema_distance;

    setActiveAddress_VL53L0X(0x62);
    newValue = readRangeContinuousMillimeters(&distanceStr);
    sr04_juice_ema_distance = sr04_juice_ema_distance == 0.0f
                                  ? newValue
                                  : SR04_EMA_ALPHA * newValue + (1.0f - SR04_EMA_ALPHA) * sr04_juice_ema_distance;
}

uint32_t JCR_sr04_GetDistanceWater() { return (uint32_t)sr04_water_ema_distance; }
uint32_t JCR_sr04_GetDistanceJuice() { return (uint32_t)sr04_juice_ema_distance; }

void JCR_sr04_Process()
{
    switch (sensor_state)
    {
    case SENSOR_IDLE:
       
        sr04_measurement_start = HAL_GetTick();

        sensor_state = SENSOR_WAITING;
        break;

    case SENSOR_WAITING:
        if (HAL_GetTick() - sr04_measurement_start >= 1)
        {
            sensor_state = SENSOR_READY;
        }
        break;

    case SENSOR_READY:
        SR04_UpdateEma();
        sensor_state = SENSOR_IDLE;
        break;
    }
}
