/*
Ejercicio de pipes donde el padre le dice al hijo que imprima por pantalla lo que el usuario esta introduciendo por
teclado

*/


#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

/*Falta includes*/

void manejador(int r){
		wait(0);
		signal(SIGCLD, manejador);
	}

int main(int argc, char *argv[]){

	if(argc < 2){
		perror("Minimo debe introducir dos argumentos");
		exit(1);
	}
	
	int fd_pipe[2];
	int fd_pipe2[2];
	char mensaje[512];
	char cond[2];

	/*1. ignora la señal SIGCLD o manejala para hacer wait*/
	signal(SIGCLD,manejador);
	/*2. crea una pipe y almacena los descriptores de lectura y escritura en fd_pipe*/
	if((pipe(fd_pipe) < 0) || (pipe(fd_pipe2) < 0)){
		perror("Fallo en pipe()");
		exit(1);
	}
	/*3. Clona el proceso*/
	pid_t p = fork(); 
	if(p > 0){ /*quien va a ejecutar este codigo?*/
		/*4. Cierra el descriptor de pipe que no se va a usar*/
		close(fd_pipe[0]);
		close(fd_pipe2[1]);
		do{
			/*5. lee de teclado un mensaje de usuario (chequea el valor de retorno de read!)*/
			int leidos = read(0,mensaje,511);
			/*6. escribe en la tuberia el mensaje (chequea el valor de retorno de write!)*/
			write(fd_pipe[1],mensaje,leidos);
			int r = read(fd_pipe2[0], cond, 2);
			if(r < 0){
				write(1,"qpa",4);
			}
			
		}while(strcmp(cond,"n")!= 0);
		/*7. cierra el descriptor de pipe que falta*/
		close(fd_pipe[1]);
		close(fd_pipe2[0]);

	
	}else if (p == 0){ /*quien va a ejecutar este codigo?*/
		/*8. Cierra el descriptor de pipe que no se va a usar*/
		close(fd_pipe[1]);
		close(fd_pipe2[0]);
		int cont = 0;
		while(1){
			/*9. lee por la pipe el mensaje (chequea el valor de retorno de read!)*/
			int leidos = read(fd_pipe[0],mensaje,511);
			cont += leidos;
			write(1, &leidos, sizeof(leidos));
			if(cont > atoi(argv[1])){
			
				write(fd_pipe2[1], "n",2);
				break; /*sale del while*/
			}else{
				write(fd_pipe2[1], "y",2);
			}

			/*10. escribe en pantalla el mensaje (chequea el valor de retorno de write!) */
			write(1,mensaje,leidos);
		}

		/*11. cierra el descriptor de pipe que falta*/
		close(fd_pipe[0]);
		close(fd_pipe2[1]);
	
	}else if(p == -1){
		perror("Ha fallado el fork");
		exit(1);
	}
	return 0;

}
