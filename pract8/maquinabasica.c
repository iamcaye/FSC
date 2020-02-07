#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

/* Declaracion de constantes */
#define estadoA 0
#define estadoB 1
#define eventoA 0
#define eventoB 1
#define alarma 2

static int global = 0;
/* Funcion auxiliar */
int espera_evento(){
	int res;
	char c = 0;
	
	int r = read(0, &c, 1);/*lee un caracter del teclado*/
	char barraN;
	
	/* 3. Decidir el evento de retorno en funcion del valor de c*/
	if(r < 0 && global == 1 && c != 'A' && c != 'B'){
		res = alarma;
		global = 0;
	}else if(c == 'A'){
		read(0, &barraN,1);
		res = eventoA;
	}else if(c == 'B'){
		res = eventoB;
		read(0, &barraN,1);/*para quitar el retorno de carro*/
	}
	return res;
}

void manejador(int r){

	global = 1;
	signal(SIGALRM, manejador);
	
}

int main(){
	int estado = estadoA; //Estado inicial
	int fin = 0, contador=1;
	
	printf("EstadoA\n");
	signal(SIGALRM, manejador);

	
	while(!fin){
		alarm(5);
		int evento = espera_evento();
		switch(evento){
			case eventoA:
				switch(estado){
					case estadoA:
						if(contador < 3){
							contador++;
						}else{
							printf("EstadoB\n");
							contador = 1;
							estado = estadoB;
						}
						break;
						
					case estadoB:
						printf("EstadoA\n");
						estado = estadoA;
						contador = 1;
						break;
				}
				break;
			case eventoB:
				switch(estado){
					case estadoA:
						printf("EstadoB\n");
						estado = estadoB;
						contador = 1;
						break;
						
					case estadoB:
						if(contador < 4){
							contador++;
						}else{
							fin = 1;
						}
						break;
				}
				break;
				
			case alarma:
				switch(estado){
					case estadoA:
						printf("EstadoB\n");
						estado = estadoB;
						contador = 1;
						break;
						
					case estadoB:
						break;
				}
				break;
			
			default:
				printf("Evento no esperado");
				fin = 1;
		}
	}
	
	return 0;
}


