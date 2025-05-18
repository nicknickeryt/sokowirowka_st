#define WATER_CONTAINER_DIAMETER_METERS 0.099f  // 9.9 cm
#define WATER_CONTAINER_RADIUS_METERS  WATER_CONTAINER_DIAMETER_METERS / 2.0f       // very important, right :)
#define WATER_CONTAINER_EMPTY_HEIGHT_METERS 0 // if we measure height of empty container then we can use this value to check if there is enough water

#define WATER_CONTAINER_RADIUS_CENTIMETERS  (WATER_CONTAINER_RADIUS_METERS * 100.0f) // cm

#define PI 3.14

void JCR_Containers_Init();
void JCR_Containers_Process();

void JCR_Containers_StartVolumeMeasurement();
float JCR_Containers_GetVolumeDeltaCcm();

float JCR_Containers_GetCurrentHeightMm();
float JCR_Containers_GetStartHeightMm();