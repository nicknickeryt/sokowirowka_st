#include "JCR_key.h"
#include "JCR_pumps.h"
#include "JCR_app.h"

#include "stm32f4xx_hal.h"  // lub inna wersja

static volatile bool keyPressed = false;

void JCR_Key_Init() {}

void JCR_Key_EXTI_Callback(uint16_t GPIO_Pin) {
    
    if (GPIO_Pin == BUTTON_Pin) {
        if (!HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)) {
            keyPressed = true;

            bool pumpActive = JCR_PumpJuice_IsOn() || JCR_PumpWater_IsOn();

            if (pumpActive) {
                JCR_App_SetState(APP_STATE_DONE);
            }
        }
        else
            keyPressed = false;
    }
}

bool JCR_Key_IsPressed() { return keyPressed; }
