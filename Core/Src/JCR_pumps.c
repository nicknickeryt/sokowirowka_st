#include "JCR_pumps.h"

#include "main.h"
#include "tim.h"

void JCR_Pumps_Init() {
  HAL_GPIO_WritePin(PUMP1_GPIO_Port, PUMP1_Pin, 0);
  HAL_GPIO_WritePin(PUMP2_GPIO_Port, PUMP2_Pin, 0);
}

bool JCR_PumpJuice_IsOn() {
  return HAL_GPIO_ReadPin(PUMP1_GPIO_Port, PUMP1_Pin);
}

bool JCR_PumpWater_IsOn() {
  return HAL_GPIO_ReadPin(PUMP2_GPIO_Port, PUMP2_Pin);
}

void JCR_PumpJuice_On() { HAL_GPIO_WritePin(PUMP1_GPIO_Port, PUMP1_Pin, 1); }

void JCR_PumpWater_On() { HAL_GPIO_WritePin(PUMP2_GPIO_Port, PUMP2_Pin, 1); }

void JCR_PumpJuice_Off() { HAL_GPIO_WritePin(PUMP1_GPIO_Port, PUMP1_Pin, 0); }

void JCR_PumpWater_Off() { HAL_GPIO_WritePin(PUMP2_GPIO_Port, PUMP2_Pin, 0); }
