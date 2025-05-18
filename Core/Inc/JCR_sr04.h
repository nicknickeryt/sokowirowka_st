typedef enum
{
  SR04_IDLE,
  SR04_WAITING,
  SR04_READY
} SR04_State;

void JCR_sr04_Init();
void JCR_sr04_Process();