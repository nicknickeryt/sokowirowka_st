#include "JCR_sr04.h"

#include <stdint.h>
#include <stdio.h>

#include "JCR_lcd.h"
#include "main.h"
#include "sr04.h"
#include "tim.h"

sr04_t sr04_water;
sr04_t sr04_juice;

SR04_State sr04_state = SR04_IDLE;
uint32_t sr04_measurement_start = 0;

static float sr04_water_ema_distance = 0.0f;
static float sr04_juice_ema_distance = 0.0f;

#define SR04_EMA_ALPHA 0.3f

void JCR_sr04_Init()
{
    // sr04_water.trig_port = TRIG1_GPIO_Port;
    // sr04_water.trig_pin = TRIG1_Pin;
    sr04_water.echo_htim = &htim3;
    sr04_water.echo_channel = TIM_CHANNEL_2;
    sr04_init(&sr04_water);

    sr04_juice.trig_port = TRIG2_GPIO_Port;
    sr04_juice.trig_pin = TRIG2_Pin;
    sr04_juice.echo_htim = &htim3;
    sr04_juice.echo_channel = TIM_CHANNEL_1;
    sr04_init(&sr04_juice);
}

void SR04_UpdateEma()
{

    float newValue = (float)sr04_water.distance;
    sr04_water_ema_distance = sr04_water_ema_distance == 0.0f
                                  ? newValue
                                  : SR04_EMA_ALPHA * newValue + (1.0f - SR04_EMA_ALPHA) * sr04_water_ema_distance;

    newValue = (float)sr04_juice.distance;
    sr04_juice_ema_distance = sr04_juice_ema_distance == 0.0f
                                  ? newValue
                                  : SR04_EMA_ALPHA * newValue + (1.0f - SR04_EMA_ALPHA) * sr04_juice_ema_distance;
}

uint32_t JCR_sr04_GetDistanceWater() { return (uint32_t)sr04_water_ema_distance; }
uint32_t JCR_sr04_GetDistanceJuice() { return (uint32_t)sr04_juice_ema_distance; }

void JCR_sr04_Process()
{
    switch (sr04_state)
    {
    case SR04_IDLE:
        sr04_trigger(&sr04_water);
        sr04_trigger(&sr04_juice);
        sr04_measurement_start = HAL_GetTick();

        sr04_state = SR04_WAITING;
        break;

    case SR04_WAITING:
        if (HAL_GetTick() - sr04_measurement_start >= 100)
        {
            sr04_state = SR04_READY;
        }
        break;

    case SR04_READY:
        SR04_UpdateEma();
        sr04_state = SR04_IDLE;
        break;
    }
}
