#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>    // para atributos de fichero tipo O_RD...
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>


#define SIZE 512


void manejador(int s){
	wait(0);
	signal(SIGCHLD, manejador);
}

int main(){
	int pid;
	int fd_pipe[2];
	
	//CONTRATO DEL FORK
	signal(SIGCHLD, manejador);
	
	//Montamos la pipe
	int r = pipe(fd_pipe);
	if(r < 0){
		perror("Fallo en creacion de la pipe");
		exit(-1);
	}
	
	//Creamos la pipe para enviar el nombre del fichero de destino y el contenido de origen
	pid = fork();
	
	if(pid < 0){
		perror("Fallo en la clonacion");
		exit(-1);
		
	}else if(pid > 0){ //CODIGO PAPA////////////////////////////////////////////////////////
	
		close(fd_pipe[0]);
		
		char target[SIZE];
		char source[SIZE];
		char buffer[SIZE];
		
		//Leemos el nombre de los ficheros de origen y destino//////////////////////////////
		int rsource = read(0, source, SIZE);
		int rtarget = read(0, target, SIZE);
		
		if(rsource < 0 || rtarget < 0){
			perror("Fallo en el read de ficheros");
			exit(-1);
		}
		
		//Mandamos el nombre del fichero de destino//////////////////////////////////////////
		int fdtarget = write(fd_pipe[1], target, SIZE);
		
		if(fdtarget < 0){
			perror("Fallo en el envio del nombre del fichero de destino");
			exit(-1);
		}
		
		//Abrimos el fichero de origen////////////////////////////////////////////////////// 
		int fdsource = open(source, O_RDONLY);

		if(fdsource < 0){
			perror("Fallo en el open de fichero de origen");
			exit(-1);
		}
		
		//Mandamos la informacion del fichero de origen por la pipe//////////////////////////
		int envio = -1;
		
		while(envio != 0){
			int leer = read(fdsource, buffer, SIZE);
			if(leer < 0){
				perror("Fallo leyendo de fichero origen");
				exit(-1);
			}
		
			envio = write(fd_pipe[1], buffer, SIZE);
			if(envio < 0){
				perror("Fallo en el envio de informacion desde el fichero de origen");
				exit(-1);
			}
		}
		
		close(fdsource);
		close(fd_pipe[1]);
		
	}else{  //CODIGO HIJO///////////////////////////////////////////////////////////////////
		close(fd_pipe[1]);
		char bufferson[SIZE];
		char mensaje[SIZE];
		
		//Leemos el nombre del fichero de destino desde la pipe
		int targetson = read(fd_pipe[0], bufferson, SIZE);		
		
		if(targetson < 0){
			perror("Fallo al leer el nombre del fichero de destino por la pipe");
			exit(-1);
		}
		
		//Abrimos o creamos el fichero de destino
		int ropen = open(bufferson, O_WRONLY | O_TRUNC | O_CREAT);
		
		if(ropen < o){
			perror("Fallo en el open de targetson");
			exit(-1);
		}
		
		int readson = -1;
		while(readson != 0){
			readson = read(fd_pipe[0], bufferson, SIZE);
			if(readson < 0){
				perror("Fallo leyendo el mensaje de texto");
				exit(-1);
			}
			int i;
			for(i = 0; i < SIZE ; i++){
				mensaje[i] = (char)tolower((int)bufferson[i]);
			}
			
			int writeson = 
		}
		
		
		
		
		//Recibimos por la pipe el nombre del fichero destino
		

	}
	return 0;
}