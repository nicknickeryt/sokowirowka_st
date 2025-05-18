#include "JCR_sr04.h"

#include <stdint.h>
#include <stdio.h>

#include "JCR_lcd.h"
#include "main.h"
#include "sr04.h"
#include "tim.h"

sr04_t sr04;

SR04_State sr04_state = SR04_IDLE;
uint32_t sr04_measurement_start = 0;

static float sr04_ema_distance = 0.0f;
#define SR04_EMA_ALPHA 0.2f  // lub inna wartość od 0.1 do 0.5

void JCR_sr04_Init() {
    sr04.trig_port = TRIG1_GPIO_Port;
    sr04.trig_pin = TRIG1_Pin;
    sr04.echo_htim = &htim3;
    sr04.echo_channel = TIM_CHANNEL_2;
    sr04_init(&sr04);
}

void SR04_UpdateEma(void) {
    float newValue = (float)sr04.distance;
    sr04_ema_distance =
        SR04_EMA_ALPHA * newValue + (1.0f - SR04_EMA_ALPHA) * sr04_ema_distance;
}

uint32_t JCR_sr04_GetDistance() { return (uint32_t)sr04_ema_distance; }

void JCR_sr04_Process(void) {
    switch (sr04_state) {
        case SR04_IDLE:
            sr04_trigger(&sr04);
            sr04_measurement_start = HAL_GetTick();
            sr04_state = SR04_WAITING;
            break;

        case SR04_WAITING:
            if (HAL_GetTick() - sr04_measurement_start >= 100) {
                sr04_state = SR04_READY;
            }
            break;

        case SR04_READY:
            SR04_UpdateEma();
            sr04_state = SR04_IDLE;
            break;
    }
}
