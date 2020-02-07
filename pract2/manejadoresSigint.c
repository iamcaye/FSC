/*
Ejercicio para practicar con manejadores y punteros

Compila con -ansi y sin este flag y observa el comportamiento de los procesos!
NOTA: cuando se compila en ansi, los comentarios // no los acepta, hay que ponerlos con barra-asterisco-asterisco-barra
*/

#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/*Definicion de tipo puntero a funcion que devuelve void y recibe como argumento un entero*/
typedef void (*sighandler_t)(int);
/*Defino una variable global de ese tipo*/
sighandler_t puntero_a_funcion;

void manejador2(int s); /*prototipo para que no se queje el compilador*/


void manejador1(int s){
  write(1,"manejador1\n",11);
  /*2. Registra el manejador2 para que atienda a SIGINT (Ctrl-C),
  guarda en la variable puntero_a_funcion lo que devuelve signal */
  puntero_a_funcion = signal(SIGINT,manejador2);
}

void manejador2(int s){
	write(1,"manejador2\n",11);
	/*3. Registra puntero_a_funcion para que atienda a SIGINT (Ctrl-C)*/
	signal(SIGINT,puntero_a_funcion);
}

int main(){
	
	/*1. Registra el manejador1 para que atienda a SIGINT (Ctrl-C)*/
  	signal(SIGINT,manejador1);
	while(1){
		pause();
	}; /*quiza sea mas solidario poner un pause() dentro del cuerpo del while!*/
  
	return 0;
}