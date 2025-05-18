#include "JCR_containers.h"

#include <stdlib.h>

#include "JCR_pumps.h"
#include "JCR_sr04.h"
#include "JCR_app.h"

float startHeightMm = 0.0f;
float currentHeightMm = 0.0f;
float volumeDeltaCcm = 0.0f;  // ml = cm³

void JCR_Containers_Init() {
    startHeightMm = 0.0f;
    currentHeightMm = 0.0f;
}
void JCR_Containers_StartVolumeMeasurement() {
    startHeightMm = JCR_sr04_GetDistance();
    currentHeightMm = startHeightMm;
}

float JCR_Containers_GetVolumeDeltaCcm() { return volumeDeltaCcm; }

float JCR_Containers_GetCurrentHeightMm() { return currentHeightMm; }

float JCR_Containers_GetStartHeightMm() { return startHeightMm; }

void JCR_Containers_Process() {
    currentHeightMm = JCR_sr04_GetDistance();

    float startHeightCm = startHeightMm / 10.0f;
    float currentHeightCm = currentHeightMm / 10.0f;

    volumeDeltaCcm = PI *
                     (WATER_CONTAINER_RADIUS_CENTIMETERS *
                      WATER_CONTAINER_RADIUS_CENTIMETERS) *
                     (currentHeightCm - startHeightCm);  // cm³

    if (volumeDeltaCcm >= 200.0f && JCR_PumpWater_IsOn()) {
        JCR_PumpWater_Off();
        JCR_App_SetState(APP_STATE_DONE);
    }
}