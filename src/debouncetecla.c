/*
 * Copyright Sebastian Herrera <sebastianh74@hotmail.com.ar.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 * Version: 0.0.1
 * Fecha de creacion: 2019/10/10*/

/*=====[Inclusion de su propia cabecera]=====================================*/
#include "debouncetecla.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"
#include <string.h>



/*=====[Macros de definicion de constantes privadas]=========================*/
extern xQueueHandle cola;

/*=====[Prototipos de funciones privadas]====================================*/


// accion de el evento de tecla pulsada
void botonPresionado(tTecla* config){
	config->tiempo_down=xTaskGetTickCount();

}

void botonLiberado(tTecla* config ){
	config->tiempo_up=xTaskGetTickCount();
	config->tiempo_diff=config->tiempo_up-config->tiempo_down;  //mido tiempo de pulsacion de tecla


	// Armo el mensaje a mandar donde digo la tecla pulsada y la duracion
	//en el formato especificado
	char msg[] = "TECx Tyyyy\r\n";
	sprintf(msg,"TECx T%04d\r\n",config->tiempo_diff);
	msg[3] = config->tecla == TEC1 ? (const char)'1' : (const char)'2'; //coloco en la posicion la tecla presionada

	taskENTER_CRITICAL ( ); 			// Entrada a seccion critica
	char *p = pvPortMalloc(MSG_SIZE_TECLA);		// "malloc"
	taskEXIT_CRITICAL ( );			// Salida de seccion critica

	//verifico que se haya podido realizar la asignacion de memoria
	if (p != NULL){
		// Inicializo la memoria recién alocada, con el mensaje
		memcpy(p,msg, MSG_SIZE_TECLA);
		// Envío a la cola el puntero al mensaje
		xQueueSend(cola, (void *) &p, portMAX_DELAY);
	}
	else {
		printf("Insuficiente espacio de memoria");
	}


}

void mefbotonInit(tTecla* config) {
	config->contfalling = 0;
	config->contrising = 0;
	config->estadoTecla=estado_up; //inicializo el estado de teclas**/


}

void mefbotonError(tTecla* config){
	config->estadoTecla=estado_up;
}


void actualizacionTecla(tTecla* config){

	switch (config->estadoTecla){

	case estado_up:

		if (!gpioRead(config->tecla)){
			config->estadoTecla=estado_falling;
		}
		break;

	case estado_falling :
		if (config->contfalling <=tiempo_antirebote){
			if (!gpioRead(config->tecla)){
				config->estadoTecla=estado_down;
				/*accion del evento*/

				botonPresionado(config);
			}
			else{
				config->estadoTecla=estado_up;
			}
			config->contfalling=0;
		}
		config->contfalling++;
		break;

	case estado_down :
		if (gpioRead(config->tecla)){
			config->estadoTecla=estado_raising;

		}
		break;

	case estado_raising:
		if (config->contrising<=tiempo_antirebote){
			if (gpioRead(config->tecla)){
				config->estadoTecla=estado_up;
				/*accion del evento*/
				botonLiberado(config);
			}
			else{
				config->estadoTecla=estado_down;
			}
			config->contrising=0;
		}
		config->contrising++;
		break;

	default:
		mefbotonError(config);

	}
}



