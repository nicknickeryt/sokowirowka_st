#include "JCR_pumps.h"
#include "main.h"
#include "tim.h"

void JCR_Pumps_Init() {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); 
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); 

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
}

void JCR_PumpJuice_On() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, JCR_PUMP_PWM_MAX);
}

void JCR_PumpWater_On() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, JCR_PUMP_PWM_MAX);
}

void JCR_PumpJuice_OnWith(uint16_t duty) {
    if (duty > JCR_PUMP_PWM_MAX) duty = JCR_PUMP_PWM_MAX;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, duty);
}

void JCR_PumpWater_OnWith(uint16_t duty) {
    if (duty > JCR_PUMP_PWM_MAX) duty = JCR_PUMP_PWM_MAX;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, duty);
}

void JCR_PumpJuice_Off() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
}

void JCR_PumpWater_Off() {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
}
