#ifndef INC_JCR_KEY_H_
#define INC_JCR_KEY_H_

#include <stdbool.h>
#include "main.h"  

void JCR_Key_Init();
void JCR_Key_EXTI_Callback(uint16_t GPIO_Pin);

bool JCR_Key_IsPressed(); 

#endif /* INC_JCR_KEY_H_ */
