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
int listo;

void manejador(int r){
	write(1,"Listo para recibir info",24);
	listo=1;
	//signal(SIGUSR1, manejador);
}

int main(int argc, char *argv[]){
	fd_set cjto_desc, cjto_copia;
	char buffer[TAMA];
	listo = 0;
	
	
	int pfifo = open("/tmp/fsc_chat", O_RDONLY);
	if(pfifo < 0){
		perror("Fallo en abriendo la fifo");
	}
	
	//mandamos el pid
	int r = open("pidreceptor", O_WRONLY || O_CREAT);
	
	int pid = getpid();
	write(r, &pid, sizeof(int));
	
	
	//Bloqueamos el proceso hasta recibir la señal del otro proceso para recibir datos por la fifo
	signal(SIGUSR1, manejador);

	//Inicializamos el cjto de descriptores
	FD_ZERO(&cjto_desc);
	
	//Aniadimos los descriptores de escribir por ptlla y la fifo
	FD_SET(1,&cjto_desc);
	FD_SET(pfifo, &cjto_desc);
	
	while(1){
		//Copiamos el cjto descriptores ya que al usar el select sera modificado
		memcpy(&cjto_copia, &cjto_desc, sizeof(fd_set) );
	
		int result = select(pfifo+1, &cjto_copia, NULL, NULL, NULL);
		//CHEQUEO ERRORES
		if(result < 0 && listo == 1){
			perror("Fallo en select");
			exit(-1);
		}else if(result == 0) {
			listo = 0;
		}
		
		if(FD_ISSET(pfifo, &cjto_copia)){
			int leidos = read(pfifo, buffer, TAMA);
			
			if(leidos < 0){
				perror("Fallo en read pfifo");
				exit(-1);
			}			
		}else{
			perror("pfifo not set");
			exit(-1);
		}
		
		if(FD_ISSET(0,&cjto_copia)){
			int escrito = write(1,buffer,TAMA);
			
			if(escrito < 0){
				perror("Fallo en write1");
				exit(-1);
			}
		}
	
	}
	
	close(pfifo);
	
	
	return 0;
}