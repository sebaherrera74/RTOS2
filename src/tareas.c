#include "tareas.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "sapi.h"
#include "string.h"
#include "task.h"
#include "debouncetecla.h"

extern xQueueHandle cola;

// Tarea A: Encender periódicamente un LED. Cada vez que el LED encienda, se deberá enviar un mensaje "LED ON"
void tarea_A(void* taskParmPtr) {
	// ---------- CONFIGURACIONES ------------------------------

	portTickType xPeriodicity = 1000/ portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();
	const char mensaje[] = "LED ON"; // Mensaje a enviar, de 7 caracteres
    char *p=NULL;
	// ---------- REPETIR POR SIEMPRE --------------------------

	while (TRUE) {
		gpioToggle( LEDB );

		taskENTER_CRITICAL ( ); 			// Entrada a seccion critica
		p = pvPortMalloc(MSG_SIZE_LED);         // malloc
		taskEXIT_CRITICAL ( );				// Salida de seccion critica

		//verifico que se haya podido realizar la asignacion de memoria
		if (p != NULL){

			memcpy(p,mensaje, MSG_SIZE_LED);       // copio bloque de memoria de mensaje a p
			xQueueSend(cola, (void *) &p, portMAX_DELAY); // Envío en la cola el puntero al nuevo mensaje
		}
		else {
			printf("Insuficiente espacio de memoria");
		}

		vTaskDelayUntil(&xLastWakeTime, xPeriodicity);
	}

	vTaskDelete(NULL);
}



// tarea tecla
void tarea_B(void* taskParmPtr) {
	tTecla* config = (tTecla*) taskParmPtr;

		//inicializacion teclas
		int i = 0;
		for (i = 0; i < 2; i++)
			mefbotonInit(&config[i]);

		// Tarea periodica cada 10 ms
		portTickType xPeriodicity =  40/ portTICK_RATE_MS;
		portTickType xLastWakeTime = xTaskGetTickCount();

		// ---------- REPETIR POR SIEMPRE --------------------------
		while(TRUE) {
			for (i = 0; i < 2; i++)
				actualizacionTecla(&config[i]);  //update de tareas teclas 1 ,2
			vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
		}
		vTaskDelete(NULL);
}


// Recibe los mensajes, los imprime, y libera la memoria
void tarea_C(void* taskParmPtr) {
	// ---------- CONFIGURACIONES ------------------------------

	char * mensaje=NULL;

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {
		xQueueReceive(cola, &mensaje, portMAX_DELAY);

		printf("%s\r\n", mensaje);

		taskENTER_CRITICAL ( );  		// Entrada a seccion critica
		//libero bloque de memoria
		vPortFree(mensaje);
		taskEXIT_CRITICAL ( );				// Salida de seccion critica

   }
	vTaskDelete(NULL);
}

