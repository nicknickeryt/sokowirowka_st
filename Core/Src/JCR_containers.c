#include "JCR_containers.h"

#include <stdlib.h>

#include "JCR_app.h"
#include "JCR_lcd.h"
#include "JCR_pumps.h"
#include "JCR_sensor.h"

#define START_HEIGHT_CHANGE_ALPHA 0.1f
#define VOLUME_CHANGE_ALPHA 0.1f

static uint32_t lastEncoderValue;

float kJ = 1.0f;
float kW = 1.19f;
uint8_t targetMlJuice = 40;
uint8_t targetMlWater = 150;

float startHeightMmWater = 0.0f;
float currentHeightMmWater = 0.0f;
float volumeDeltaCcmWater = 0.0f;  // ml = cm³

float startHeightMmJuice = 0.0f;
float currentHeightMmJuice = 0.0f;
float volumeDeltaCcmJuice = 0.0f;  // ml = cm³

void JCR_Contaners_SetTargetMlJuice(uint8_t ml) { targetMlJuice = ml; }
void JCR_Contaners_SetTargetMlWater(uint8_t ml) { targetMlWater = ml; }

uint8_t JCR_Containers_GetTargetMlJuice() { return targetMlJuice; }
uint8_t JCR_Containers_GetTargetMlWater() { return targetMlWater; }

void JCR_Containers_Init() {
  startHeightMmWater = 0.0f;
  currentHeightMmWater = 0.0f;

  startHeightMmJuice = 0.0f;
  currentHeightMmJuice = 0.0f;

  lastEncoderValue = TIM2->CNT / 2;
}
void JCR_Containers_StartVolumeMeasurementWater() {
  startHeightMmWater = JCR_sensor_GetDistanceWater();

  currentHeightMmWater = startHeightMmWater;
  volumeDeltaCcmJuice = 0.0f;
}

void JCR_Containers_StartVolumeMeasurementJuice() {
  startHeightMmJuice = JCR_sensor_GetDistanceJuice();

  currentHeightMmJuice = startHeightMmJuice;
  volumeDeltaCcmWater = 0.0f;
}

float JCR_Containers_GetVolumeDeltaCcmWater() { return volumeDeltaCcmWater; }

float JCR_Containers_GetCurrentHeightMmWater() { return currentHeightMmWater; }

float JCR_Containers_GetStartHeightMmWater() { return startHeightMmWater; }

float JCR_Containers_GetVolumeDeltaCcmJuice() { return volumeDeltaCcmJuice; }

float JCR_Containers_GetCurrentHeightMmJuice() { return currentHeightMmJuice; }

float JCR_Containers_GetStartHeightMmJuice() { return startHeightMmJuice; }

void JCR_Containers_Process() {
  if (JCR_App_GetState() == APP_STATE_JUICE) {
    currentHeightMmJuice = JCR_sensor_GetDistanceJuice();

    if (currentHeightMmJuice < startHeightMmJuice) {
      startHeightMmJuice =
          START_HEIGHT_CHANGE_ALPHA * currentHeightMmJuice +
          (1.0f - START_HEIGHT_CHANGE_ALPHA) * startHeightMmJuice;
    }

    float startHeightCmJuice = startHeightMmJuice / 10.0f;
    float currentHeightCmJuice = currentHeightMmJuice / 10.0f;

    float volumeRawJuice = PI *
                           (WATER_CONTAINER_RADIUS_CENTIMETERS *
                            WATER_CONTAINER_RADIUS_CENTIMETERS) *
                           (currentHeightCmJuice - startHeightCmJuice);

    volumeDeltaCcmJuice = VOLUME_CHANGE_ALPHA * volumeRawJuice +
                          (1.0f - VOLUME_CHANGE_ALPHA) * volumeDeltaCcmJuice;

    if (volumeDeltaCcmJuice >= ((float)targetMlJuice * kJ) &&
        JCR_PumpJuice_IsOn())
      JCR_App_SetState(APP_STATE_WATER);
  }

  else if (JCR_App_GetState() == APP_STATE_WATER) {
    currentHeightMmWater = JCR_sensor_GetDistanceWater();

    if (currentHeightMmWater < startHeightMmWater) {
      startHeightMmWater =
          START_HEIGHT_CHANGE_ALPHA * currentHeightMmWater +
          (1.0f - START_HEIGHT_CHANGE_ALPHA) * startHeightMmWater;
    }

    float startHeightCmWater = startHeightMmWater / 10.0f;
    float currentHeightCmWater = currentHeightMmWater / 10.0f;

    float volumeRawWater = PI *
                           (WATER_CONTAINER_RADIUS_CENTIMETERS *
                            WATER_CONTAINER_RADIUS_CENTIMETERS) *
                           (currentHeightCmWater - startHeightCmWater);

    volumeDeltaCcmWater = VOLUME_CHANGE_ALPHA * volumeRawWater +
                          (1.0f - VOLUME_CHANGE_ALPHA) * volumeDeltaCcmWater;

    if (volumeDeltaCcmWater >= ((float)targetMlWater * kW) &&
        JCR_PumpWater_IsOn())
      JCR_App_SetState(APP_STATE_DONE);
  }
}

void JCR_Containers_EncoderCallback() {
  uint32_t newEncoderValue = TIM2->CNT / 2;
  int32_t deltaEncoderValue = lastEncoderValue - newEncoderValue;

  if (deltaEncoderValue != 0) {
    lastEncoderValue = newEncoderValue;

    bool pumpActive = JCR_PumpJuice_IsOn() || JCR_PumpWater_IsOn();
    if (pumpActive) return;

    switch (JCR_Lcd_GetSettingMode()) {
      case MODE_JUICE: {
        int juice = JCR_Containers_GetTargetMlJuice();
        int water = JCR_Containers_GetTargetMlWater();
        int newJuice = juice + (deltaEncoderValue > 0 ? 1 : -1);

        if (deltaEncoderValue > 0 && juice < 255 && (newJuice + water) <= 200)
          JCR_Contaners_SetTargetMlJuice((uint8_t)newJuice);
        else if (deltaEncoderValue < 0 && juice > 0 &&
                 (newJuice + water) <= 200)
          JCR_Contaners_SetTargetMlJuice((uint8_t)newJuice);

        break;
      }
      case MODE_WATER: {
        int water = JCR_Containers_GetTargetMlWater();
        int juice = JCR_Containers_GetTargetMlJuice();
        int newWater = water + (deltaEncoderValue > 0 ? 1 : -1);

        if (deltaEncoderValue > 0 && water < 255 && (juice + newWater) <= 200)
          JCR_Contaners_SetTargetMlWater((uint8_t)newWater);
        else if (deltaEncoderValue < 0 && water > 0 &&
                 (juice + newWater) <= 200)
          JCR_Contaners_SetTargetMlWater((uint8_t)newWater);

        break;
      }
    }
  }
}