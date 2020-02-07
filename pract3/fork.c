#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
/*Hacen falta mas includes*/


int main(int argc, char* argv[]){
	int pidHijo;

	/*1. Inicializamos la variable número de hijos*/
	int nhijos = 10;
	/*2. Mientras tengamos que crear más hijos*/
	int n = 0;
	while(n < nhijos){
		if ((pidHijo = fork()) == -1){
			perror("No hay recursos");
			return -1;
		} 
		else if (pidHijo == 0){ /*Codigo del hijo*/
			/*Imprimir 10 veces el mensaje siguiente*/
			printf("Soy el hijo %d. Mi padre ha creado: %d hijos\n", getpid(), n);
			exit(0);
		}
		else{ /*Código del padre*/
			/*3. Actualizar número de hijos*/
			n++;
		}
	}
	/*hago un bucle para esperar a todos mis hijos*/
	int i;
	for(i=0;i<10;i++){
		wait(0);
		/*4. espera a un hijo*/
	}
	return 0;
}
