#ifndef JCR_MUG_H
#define JCR_MUG_H

#include <stdbool.h>
#include <stdint.h>
#include "main.h"

#define MUG_DET_THRESHOLD_MS 1500

void JCR_Mug_Init();

void JCR_Mug_Process();

bool JCR_Mug_IsDetected();
bool JCR_Mug_IsPresent(); 
void JCR_Mug_ClearDetected();

void JCR_Mug_EXTI_Callback(uint16_t GPIO_Pin);


#endif // JCR_MUG_H
