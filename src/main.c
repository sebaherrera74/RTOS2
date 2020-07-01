
// Includes de FreeRTOS
#include "debouncetecla.h"
#include "FreeRTOSConfig.h"
#include "tareas.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "sapi.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;


/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[Definicon de variables globales]=============================*/

// Cola de mensajes entre tareas
xQueueHandle cola;

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void) {
	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();

	tecla_config[0].tecla = TEC1;
	tecla_config[1].tecla = TEC2;

	// UART for debug messages
	debugPrintConfigUart( UART_USB, 115200 );
	debugPrintlnString("RTOSII-TP1");

	// Led para dar se�al de vida
	gpioToggle(LED3);

	//tarea de endender LED y enviar msje

BaseType_t res =xTaskCreate(tarea_A,  // Funcion de la tarea a ejecutar
				    (const char *) "tarea_A", // Nombre de la tarea como String amigable para el usuario
				    configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
					0,                          	// Parametros de tarea
					tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
					0                         // Puntero a la tarea creada en el sistema
	);

	//tarea de TECLA
	res =xTaskCreate(tarea_B,  // Funcion de la tarea a ejecutar
			(const char *) "tarea_B", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
			tecla_config,                          	// Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
	);

	//Tarea de imprimir msjes
	res =xTaskCreate(tarea_C,  // Funcion de la tarea a ejecutar
			(const char *) "tarea_C", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, 	// Cantidad de stack de la tarea
			0,                          	// Parametros de tarea
			tskIDLE_PRIORITY + 1,         	// Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
	);

	if (res == pdFAIL) {
		ERROR_CREACION_TAREAS;
	}

	// Creación de cola para enviar mensajes

	cola = xQueueCreate(10, sizeof(void*));
	if(cola==NULL){                   //chequeo que se halla creado bien la cola
		ERROR_CREACION_COLA;
	}

	// Iniciar scheduler
	vTaskStartScheduler();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while ( TRUE) {
		// Si cae en este while 1 significa que no pudo iniciar el scheduler
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontroladore y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/



/*==================[fin del archivo]========================================*/
