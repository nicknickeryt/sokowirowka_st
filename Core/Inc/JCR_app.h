#ifndef JCR_APP_H
#define JCR_APP_H

#define JUICE_PUMP_MAX_TIME_MS 100000
#define WATER_PUMP_MAX_TIME_MS 100000

typedef enum {
    APP_STATE_IDLE = 0,
    APP_STATE_JUICE,
    APP_STATE_WATER,
    APP_STATE_DONE
} JCR_AppState_t;

void JCR_App_Init();
void JCR_App_Process();
void JCR_App_SetState(JCR_AppState_t state);
JCR_AppState_t JCR_App_GetState();

#endif // JCR_APP_H
