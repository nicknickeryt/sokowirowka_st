#include "JCR_containers.h"

#include <stdlib.h>

#include "JCR_app.h"
#include "JCR_pumps.h"
#include "JCR_sr04.h"

#define START_HEIGHT_CHANGE_ALPHA 0.05f
#define VOLUME_CHANGE_ALPHA 0.005f

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
        currentHeightMmJuice = JCR_sr04_GetDistanceJuice();

        if(currentHeightMmJuice < startHeightMmJuice) {
            startHeightMmJuice = START_HEIGHT_CHANGE_ALPHA * currentHeightMmJuice + (1.0f - START_HEIGHT_CHANGE_ALPHA) * startHeightMmJuice;
        }

        float startHeightCmJuice = startHeightMmJuice / 10.0f;
        float currentHeightCmJuice = currentHeightMmJuice / 10.0f;

        float volumeRawJuice =
            PI *
            (WATER_CONTAINER_RADIUS_CENTIMETERS *
             WATER_CONTAINER_RADIUS_CENTIMETERS) *
            (currentHeightCmJuice - startHeightCmJuice + 0.07f);

        volumeDeltaCcmJuice = VOLUME_CHANGE_ALPHA * volumeRawJuice + (1.0f - VOLUME_CHANGE_ALPHA) * volumeDeltaCcmJuice;

        if (volumeDeltaCcmJuice >= 0.0f && JCR_PumpJuice_IsOn())    // TEMP!!!
            JCR_App_SetState(APP_STATE_WATER);
    }

    else if (JCR_App_GetState() == APP_STATE_WATER) {
        currentHeightMmWater = JCR_sr04_GetDistanceWater();

        if(currentHeightMmWater < startHeightMmWater) {
            startHeightMmWater = START_HEIGHT_CHANGE_ALPHA * currentHeightMmWater + (1.0f - START_HEIGHT_CHANGE_ALPHA) * startHeightMmWater;
        }

        float startHeightCmWater = startHeightMmWater / 10.0f;
        float currentHeightCmWater = currentHeightMmWater / 10.0f;

        float volumeRawWater =
            PI *
            (WATER_CONTAINER_RADIUS_CENTIMETERS *
             WATER_CONTAINER_RADIUS_CENTIMETERS) *
            (currentHeightCmWater - startHeightCmWater);

        volumeDeltaCcmWater = VOLUME_CHANGE_ALPHA * volumeRawWater + (1.0f - VOLUME_CHANGE_ALPHA) * volumeDeltaCcmWater;

        if (volumeDeltaCcmWater >= 150.0f && JCR_PumpWater_IsOn())
            JCR_App_SetState(APP_STATE_DONE);
    }
}
