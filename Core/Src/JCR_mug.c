#include "JCR_mug.h"

static volatile uint32_t mugStartTime = 0;
static volatile bool mugWaitFlag = false;
static volatile bool mugDetected = false;

void JCR_Mug_Init() {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // LED off
}

void JCR_Mug_Process() {
    if (mugWaitFlag) {
        if (HAL_GPIO_ReadPin(MUG_DET_GPIO_Port, MUG_DET_Pin)) {
            if (HAL_GetTick() - mugStartTime >= MUG_DET_THRESHOLD_MS) {
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);  // LED on
                mugDetected = true;
                mugWaitFlag = false;
            }
        } else {
            mugWaitFlag = false;
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // LED off
        }
    }
}

bool JCR_Mug_IsDetected() {
    return mugDetected;
}

void JCR_Mug_ClearDetected() {
    mugDetected = false;
}

void JCR_Mug_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == MUG_DET_Pin) {
        bool state = HAL_GPIO_ReadPin(MUG_DET_GPIO_Port, MUG_DET_Pin);

        if (state == 1) {
            mugStartTime = HAL_GetTick();
            mugWaitFlag = true;
        } else {
            mugWaitFlag = false;
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // LED off
        }
    }
}

bool JCR_Mug_IsPresent() {
    return HAL_GPIO_ReadPin(MUG_DET_GPIO_Port, MUG_DET_Pin);
}