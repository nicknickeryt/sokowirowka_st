#include "JCR_dispenser.h"

#include "tim.h"

void JCR_dispenser_init() {
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void JCR_dispenser_push() {
  TIM1->CCR3 = 75;  // 60->25
  HAL_Delay(20);
  TIM1->CCR4 = 25;  // 25->70
}

void JCR_dispenser_retract() {
  TIM1->CCR3 = 37;
  HAL_Delay(20);
  TIM1->CCR4 = 68;
}

void JCR_dispenser_dispenseCup() {
  JCR_dispenser_push();
  HAL_Delay(1000);
  JCR_dispenser_retract();
}