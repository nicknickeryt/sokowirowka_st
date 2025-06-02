#include "usart.h"

#include <stdbool.h>
#include <stdint.h>

#include "VL53L0X.h"

void JCR_uart_init();
void JCR_uart_Process();
void JCR_uart_Callback(UART_HandleTypeDef* huart, uint16_t size);

void JCR_uart_SetStartPressed(bool pressed);
bool JCR_uart_IsStartPressed();
