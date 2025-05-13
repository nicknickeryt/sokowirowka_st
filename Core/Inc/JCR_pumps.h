#ifndef JCR_PUMPS_H
#define JCR_PUMPS_H

#include "main.h"

#define JCR_PUMP_PWM_MAX 1000

void JCR_Pumps_Init();

void JCR_PumpJuice_On();
void JCR_PumpWater_On();

void JCR_PumpJuice_OnWith(uint16_t duty);
void JCR_PumpWater_OnWith(uint16_t duty);

void JCR_PumpJuice_Off();
void JCR_PumpWater_Off();

#endif // JCR_PUMPS_H
