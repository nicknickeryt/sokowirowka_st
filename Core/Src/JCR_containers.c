#include "JCR_containers.h"

#include <stdlib.h>

#include "JCR_app.h"
#include "JCR_pumps.h"
#include "JCR_sr04.h"

float startHeightMmWater = 0.0f;
float currentHeightMmWater = 0.0f;
float volumeDeltaCcmWater = 0.0f;  // ml = cm³

float startHeightMmJuice = 0.0f;
float currentHeightMmJuice = 0.0f;
float volumeDeltaCcmJuice = 0.0f;  // ml = cm³

void JCR_Containers_Init() {
    startHeightMmWater = 0.0f;
    currentHeightMmWater = 0.0f;

    startHeightMmJuice = 0.0f;
    currentHeightMmJuice = 0.0f;
}
void JCR_Containers_StartVolumeMeasurementWater() {
    startHeightMmWater = JCR_sr04_GetDistanceWater();

    currentHeightMmWater = startHeightMmWater;
}

void JCR_Containers_StartVolumeMeasurementJuice() {
    startHeightMmJuice = JCR_sr04_GetDistanceJuice();

    currentHeightMmJuice = startHeightMmJuice;
}

float JCR_Containers_GetVolumeDeltaCcmWater() { return volumeDeltaCcmWater; }

float JCR_Containers_GetCurrentHeightMmWater() { return currentHeightMmWater; }

float JCR_Containers_GetStartHeightMmWater() { return startHeightMmWater; }

float JCR_Containers_GetVolumeDeltaCcmJuice() { return volumeDeltaCcmJuice; }

float JCR_Containers_GetCurrentHeightMmJuice() { return currentHeightMmJuice; }

float JCR_Containers_GetStartHeightMmJuice() { return startHeightMmJuice; }

void JCR_Containers_Process() {
    if (JCR_App_GetState() == APP_STATE_JUICE) {
        // JUICE
        currentHeightMmJuice = JCR_sr04_GetDistanceJuice();

        float startHeightCmJuice = startHeightMmJuice / 10.0f;
        float currentHeightCmJuice = currentHeightMmJuice / 10.0f;

        volumeDeltaCcmJuice =
            PI *
            (WATER_CONTAINER_RADIUS_CENTIMETERS *
             WATER_CONTAINER_RADIUS_CENTIMETERS) *
            (currentHeightCmJuice - startHeightCmJuice);  // cm³

        if (volumeDeltaCcmJuice >= 20.0f && JCR_PumpJuice_IsOn())
            JCR_App_SetState(APP_STATE_WATER);

    }

    else if (JCR_App_GetState() == APP_STATE_WATER) {
        currentHeightMmWater = JCR_sr04_GetDistanceWater();

        float startHeightCmWater = startHeightMmWater / 10.0f;
        float currentHeightCmWater = currentHeightMmWater / 10.0f;

        volumeDeltaCcmWater =
            PI *
            (WATER_CONTAINER_RADIUS_CENTIMETERS *
             WATER_CONTAINER_RADIUS_CENTIMETERS) *
            (currentHeightCmWater - startHeightCmWater);  // cm³

        if (volumeDeltaCcmWater >= 200.0f && JCR_PumpWater_IsOn())
            JCR_App_SetState(APP_STATE_DONE);
    }
}