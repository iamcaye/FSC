#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>    // para atributos de fichero tipo O_RD...
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
//Hacen falta más includes

#define SIZE 512


//Aquí hace falta una función…
void manejador(int r){
	wait(0);
}

int main(int argc, char* argv[])
{
	
	int pidHijo;

	//1. Registramos manejador para señal SIGCHLD
	signal(SIGCHLD, manejador);
	//2. Creamos una tubería
	int fd_pipe[2];
	int r;
	if( r = pipe(fd_pipe) < 0){
		perror("Fallo en la creacion de la tuberia");
		exit(-1);
	}
	
	//3. Creamos un hijo
	if ((pidHijo = fork()) == -1)
	{
		perror("No hay recursos");
		return -1;
	} 
	else if (pidHijo == 0) /*Codigo del hijo*/
	{
		//4. Cerramos lo que haga falta…
		close(fd_pipe[1]);
		char buffer[SIZE];

		//5. Leemos nombre de fichero destino
		int bytesRead;
		if ((bytesRead = read(fd_pipe[0], buffer, SIZE)) == -1) 
		{	
			perror("Problema en read en hijo\n");
			exit(-1);
		}

		//6. Abrimos el fichero destino (y comprobamos que se haya abierto bien
		int fd;
		if (fd = open(buffer, O_WRONLY | O_TRUNC | O_CREAT) < 0)
		{
			perror("Problema abriendo archivo destino\n");
			exit(-1);
		}
	
		//7. Vamos leyendo lo que nos llega del padre
		while((bytesRead = read(fd_pipe[0], buffer, SIZE)) != 0)
		{
			//8. Convertimos lo que recibimos en minúsculas
			int i;
			for( i = 0; i < bytesRead; i++){
				buffer[i] = (char)tolower((int)buffer[i]);
			}
			//9. Y escribimos en el fichero el resultado (comprobando errores)
			int bytesWritten = write(fd, buffer, SIZE);
			if(bytesWritten < 0)
			{
				perror("Error con write en el hijo\n");
				exit (-1);
			}
		}
		
		//10. Cerramos lo que haga falta
		close(fd_pipe[0]);
		close(fd);
		exit(0);
	}
	else 
	{
		/*Código del padre*/
		//11. Cerramos lo que haga falta
		close(fd_pipe[0]);
		
		char source[SIZE], target[SIZE];
		char buffer[SIZE];

		//12. Pedimos y leemos nombres de fichero origen y destino
		printf("Introduzca nombre de fichero origen y destino: \n");
		
		int leidos1 = read(0, source, SIZE);
		
		int leidos2 = read(0, target, SIZE);
		
		if(leidos1 < 0 || leidos2 < 0){
			perror("Fallo en read de pantalla");
			exit(-1);
		}

		//13. Abrimos el fichero origen comprobando errores
		int p;
		if (p = open(source, O_RDONLY) < 0){
			perror("Problema abriendo origen file\n");
			exit(-1);
		}
		
		//14. Enviamos el nombre al hijo, comprobando errores
		int bytesWritten;
		bytesWritten = write(fd_pipe[1], target, SIZE);
		if (bytesWritten < 0){
			perror("Error con write en el padre\n");
			exit(-1);
		}
		

		//15. Vamos leyendo del fichero… 
		int bytesRead;
		while((bytesRead = read(p, buffer, SIZE)) != 0)
		{
			//16. … y enviándoselo al hijo (comprobando errores)
			bytesWritten = write(fd_pipe[1], buffer, SIZE);
			if (bytesWritten < 0)
			{
				perror("Error in write en el padre\n");
				exit(-1);
			}
			printf("El padre ha mandado %d bytes\n", bytesWritten);
		}

		//17. Cerramos lo que haga falta…
		close(fd_pipe[1]);

	}
	

	return 0;
}
