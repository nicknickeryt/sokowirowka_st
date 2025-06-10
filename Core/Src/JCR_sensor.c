#include "JCR_sensor.h"

#include <stdint.h>
#include <stdio.h>

#include "JCR_lcd.h"
#include "VL53L0X.h"
#include "i2c.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

uint32_t sr04_measurement_start = 0;

static float sensor_water_ema_dist = 0.0f;
static float sensor_juice_ema_dist = 0.0f;

#define SR04_EMA_ALPHA 0.5f

void SR04_UpdateEma() {
  setActiveAddress_VL53L0X(0x60);
  statInfo_t_VL53L0X distanceStr;
  float newValue = readRangeContinuousMillimeters(&distanceStr);
  sensor_water_ema_dist =
      sensor_water_ema_dist == 0.0f
          ? newValue
          : SR04_EMA_ALPHA * newValue +
                (1.0f - SR04_EMA_ALPHA) * sensor_water_ema_dist;

  setActiveAddress_VL53L0X(0x62);
  newValue = readRangeContinuousMillimeters(&distanceStr);
  sensor_juice_ema_dist =
      sensor_juice_ema_dist == 0.0f
          ? newValue
          : SR04_EMA_ALPHA * newValue +
                (1.0f - SR04_EMA_ALPHA) * sensor_juice_ema_dist;
}

uint32_t JCR_sensor_GetDistanceWater() {
  return (uint32_t)sensor_water_ema_dist;
}
uint32_t JCR_sensor_GetDistanceJuice() {
  return (uint32_t)sensor_juice_ema_dist;
}

void JCR_sensor_process() {
  static uint32_t last_update = 0;
  if (HAL_GetTick() - last_update >= 1) {
    last_update = HAL_GetTick();
    SR04_UpdateEma();
  }
}
