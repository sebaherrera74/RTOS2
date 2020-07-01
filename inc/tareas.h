#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "sapi.h"
#include "semphr.h"


#define MSG_SIZE_LED    7
#define ERROR_CREACION_TAREAS    printf("ERROR AL CREAR TAREA")
#define ERROR_CREACION_COLA    printf("ERROR AL CREAR COLA")

void tarea_A(void* taskParmPtr);
void tarea_B(void* taskParmPtr);
void tarea_C(void* taskParmPtr);

