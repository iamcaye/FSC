/*
Programa que lee un entero positivo como argumento, que indica el numero de segundos que sera inmune a Ctrl-C
*/
#include<stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>

//Hacen falta más includes

//Aquí hace falta una función manejadora-----------------------
void manejador(int s){
	write(1	,"Manejador\n",10);
	signal(SIGINT,SIG_DFL);
}




// --- Función principal --------------------------------------
int main(int argc, char **argv) {
  
		//1. ¿Número de argumentos adecuados?
		if(argc < 2){
			perror("Minimo necesito 2 argumentos\n");
			return -1;
		}
  
		//2. Leer número de segundos como argumento del programa y convertirlo a entero
		int seg = atoi(argv[1] );

		//3. Registro de las señales
    	signal(SIGALRM,manejador);
    	signal(SIGINT, SIG_IGN);

		//4. Establecer los segundos de alarma
    	alarm(seg);
		//5. Esperamos indefinidamente
		while(1){
			pause();
		}
    
    return 0;
}

