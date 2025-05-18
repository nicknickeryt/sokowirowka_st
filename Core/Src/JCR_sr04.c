#include "sr04.h"
#include "JCR_sr04.h"
#include "JCR_lcd.h"

#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "tim.h"

sr04_t sr04;

SR04_State sr04_state = SR04_IDLE;
uint32_t sr04_measurement_start = 0;

void JCR_sr04_Init()
{
  sr04.trig_port = TRIG1_GPIO_Port;
  sr04.trig_pin = TRIG1_Pin;
  sr04.echo_htim = &htim3;
  sr04.echo_channel = TIM_CHANNEL_2;
  sr04_init(&sr04);
}

void JCR_sr04_Process(void)
{
  char buf[16];

  switch (sr04_state)
  {
  case SR04_IDLE:
    sr04_trigger(&sr04); // rozpocznij pomiar
    sr04_measurement_start = HAL_GetTick();
    sr04_state = SR04_WAITING;
    break;

  case SR04_WAITING:
    if (HAL_GetTick() - sr04_measurement_start >= 100)
    { // odczekaj 100 ms
      // gotowy do odczytu
      sr04_state = SR04_READY;
    }
    break;

  case SR04_READY:
    sprintf(buf, "%lu mm", sr04.distance);
    JCR_Lcd_Print(buf, 0, 0);

    sr04_state = SR04_IDLE; // gotowy do nowego pomiaru
    break;
  }
}

