#include <stdint.h>

typedef enum
{
  SENSOR_IDLE,
  SENSOR_WAITING,
  SENSOR_READY
} SENSOR_State;

void JCR_sr04_Process();
uint32_t JCR_sr04_GetDistanceWater();
uint32_t JCR_sr04_GetDistanceJuice();