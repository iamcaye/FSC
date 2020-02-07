#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
//Hacen falta mas includes

int main(int argc, char* argv[]){
	
	int pidHijo;
  
	/*1. Inicializar número de hijos*/
	int nhijos = 10;
	/*2. Mientras haya que crear más hijos*/
	int n = 0;
	while(n < nhijos){
		/*3. Creamos un hijo*/
		if ((pidHijo = fork()) == -1){
			perror("No hay recursos");
			return -1;
		} 
		else if (pidHijo == 0){ /*Codigo del hijo*/
			/*4. Si el pid del hijo es par*/
			if ((getpid()%2) == 0){	
				printf("El hijo con pid %d va a ejecutar un date\n", getpid());
				/*5. Llamar a alguna función exec adecuada*/
				/*(date es un comando del sistema operativo que imprime la fecha y hora por pantalla)*/
				int r = execl("/bin/date", "date", NULL);
				perror("fallo en execl()!\n\n");
				exit(1);
			}else{
				printf("El hijo con pid %d va a ejecutar un pwd\n", getpid());

				/*6. Llamar a alguna función exec adecuada*/
				/*(pwd es un comando del sistema operativo que imprime el directorio actual)*/
				int r = execl("/bin/pwd", "pwd", NULL);
				perror("fallo en execl()!\n\n");
				exit(1);
			}
		}else{
			/*7. Actualizar contador de hijos*/n++;
		}
	}
	/*hago un bucle para esperar a todos mis hijos*/
	int i;
	for(i=0;i<10;i++){
		/*8. espera a un hijo*/
		wait(0);
	}
		
	return 0;
}
