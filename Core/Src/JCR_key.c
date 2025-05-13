#include "JCR_key.h"

#include "stm32f4xx_hal.h"  // lub inna wersja

static volatile bool keyPressed = false;

void JCR_Key_Init() {}

void JCR_Key_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == KEY_Pin) {
        if (!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin))
            keyPressed = true;
        else
            keyPressed = false;
    }
}

bool JCR_Key_IsPressed() { return keyPressed; }
