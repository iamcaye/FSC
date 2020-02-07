#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>


#define TAMA 1024

int main(int argc, char *argv[]){


	fd_set cjto_desc, cjto_copia;
	struct timeval tv;
	char buffer[TAMA];
	//Abrimos la fifo
	int pfifo = open("/tmp/fsc_chat", 0666);
	if(pfifo < 0){
		perror("Fallo en el open fifo");
		exit(-1);
	}

	//Recibimos el pid
	int pid;
	int abrir = open("pidreceptor", O_RDONLY);
	if(abrir < 0){
		perror("Fallo en obtener pid de fichero");
		exit(-1);
	}
	read(abrir, &pid, sizeof(int));
	
	write(1, &pid, sizeof(int));
	
	//Mandamos la señal al otro programa 
	int rsen = kill(pid, SIGUSR1);
	if(rsen < 0){
		perror("Fallo en el envio de la señal");
		exit(-1);
	}
	
	
	//Inicializamos los descriptores a cero
	FD_ZERO(&cjto_desc);
	
	//Aniadimos los decriptores de teclado y la fifo
	FD_SET(0,&cjto_desc);
	FD_SET(pfifo, &cjto_desc);
	
	while(1){
		
		printf("qpa");
		//copia de cjto_desc ya que su valor puede ser modificado por select
		memcpy(&cjto_copia, &cjto_desc, sizeof(fd_set));
		
		tv.tv_usec = 0;
		tv.tv_sec = 3;
		
		int result = select(pfifo+1, &cjto_copia, NULL, NULL, &tv);
		
		if(result < 0){
			perror("Fallo en result");
			exit(-1);
		}else if(result == 0){
			write(1,".\n", 3);
		}
		
		if(FD_ISSET(0,&cjto_copia)){
			int leidos = read(0, buffer, TAMA-1);
			
			if(leidos < 0){
				perror("Fallo en read de teclado");
				exit(-1);
			}else{
				buffer[leidos] = '\0';
			}
		}
		
		if(FD_ISSET(pfifo, &cjto_copia)){			
			int msj = write(pfifo, buffer, TAMA);
			
			if(msj < 0){
				perror("Fallo en envio de msj");
				exit(-1); 	
			}else{
				printf("ENVIADO\n");
			}
		}else{
			perror("Fallo en set pfifo");
			exit(-1);
		}
		
		
	}
		close(pfifo);
		return 0;

}