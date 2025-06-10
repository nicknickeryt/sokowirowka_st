#ifndef JCR_PUMPS_H
#define JCR_PUMPS_H

#include <stdbool.h>

#include "main.h"

#define JCR_PUMP_PWM_MAX 1000

void JCR_Pumps_Init();

bool JCR_PumpJuice_IsOn();
bool JCR_PumpWater_IsOn();

void JCR_PumpJuice_On();
void JCR_PumpWater_On();

void JCR_PumpJuice_Off();
void JCR_PumpWater_Off();

#endif  // JCR_PUMPS_H
