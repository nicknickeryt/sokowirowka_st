#include "JCR_uart.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "JCR_app.h"
#include "JCR_containers.h"
#include "JCR_mug.h"
#include "JCR_pumps.h"
#include "JCR_sensor.h"
#include "usart.h"

static uint32_t lastTick = 0;
uint8_t rxBuffer[16];

bool startPressed = false;

void JCR_uart_init() {
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, rxBuffer, sizeof(rxBuffer));
  HAL_UART_Transmit_IT(&huart1, (uint8_t*)"Miksosok <3\r\n", 26);
}

void JCR_uart_Process() {
  uint32_t now = HAL_GetTick();
  if (now - lastTick >= 500) {
    lastTick = now;

    static char msg[64];
    snprintf(msg, sizeof(msg),
             "WMM%4lu.JMM%4lu.MUG%u.WTG%4u.JTG%4u.WCV%4u.JCV%4u\r\n",
             JCR_sensor_GetDistanceWater(), JCR_sensor_GetDistanceJuice(),
             JCR_Mug_IsPresent(), JCR_Containers_GetTargetMlWater(),
             JCR_Containers_GetTargetMlJuice(),
             (uint8_t)JCR_Containers_GetVolumeDeltaCcmWater(),
             (uint8_t)JCR_Containers_GetVolumeDeltaCcmJuice());
    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));
  }
}

void JCR_uart_SetStartPressed(bool pressed) { startPressed = pressed; }

bool JCR_uart_IsStartPressed() { return startPressed; }

void JCR_uart_Callback(UART_HandleTypeDef* huart, uint16_t size) {
  if (huart != &huart1 || size == 0) {
    HAL_UARTEx_ReceiveToIdle_IT(huart, rxBuffer, sizeof(rxBuffer));
    return;
  }

  static char msg[64];
  snprintf(msg, sizeof(msg), "\r\nOK\r\n");

  char* startCommand = "jcr_start";
  char* stopCommand = "jcr_stop";
  char* juicePlus = "jcr_jplus";
  char* juiceMinus = "jcr_jminus";
  char* waterPlus = "jcr_wplus";
  char* waterMinus = "jcr_wminus";

  if (strncmp((char*)rxBuffer, startCommand, strlen(startCommand)) == 0) {
    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));
    startPressed = true;
  } else if (strncmp((char*)rxBuffer, stopCommand, strlen(stopCommand)) == 0) {
    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));
    startPressed = false;
    bool pumpActive = JCR_PumpJuice_IsOn() || JCR_PumpWater_IsOn();

    if (pumpActive) JCR_App_SetState(APP_STATE_DONE);
  } else if (strncmp((char*)rxBuffer, juicePlus, strlen(juicePlus)) == 0) {
    JCR_Contaners_SetTargetMlJuice(JCR_Containers_GetTargetMlJuice() + 1);

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));

  } else if (strncmp((char*)rxBuffer, juiceMinus, strlen(juiceMinus)) == 0) {
    JCR_Contaners_SetTargetMlJuice(JCR_Containers_GetTargetMlJuice() - 1);

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));

  } else if (strncmp((char*)rxBuffer, waterPlus, strlen(waterPlus)) == 0) {
    JCR_Contaners_SetTargetMlWater(JCR_Containers_GetTargetMlWater() + 1);

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));

  } else if (strncmp((char*)rxBuffer, waterMinus, strlen(waterMinus)) == 0) {
    JCR_Contaners_SetTargetMlWater(JCR_Containers_GetTargetMlWater() - 1);

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)msg, strlen(msg));
  }

  HAL_UARTEx_ReceiveToIdle_IT(huart, rxBuffer, sizeof(rxBuffer));
}