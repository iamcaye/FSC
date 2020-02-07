#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* ESTADOS */
#define ESPERA_F1 0
#define ESPERA_F2 1

/* EVENTOS */
#define ev_fin 0
#define ev_nofin 1
#define ev_timeout 2

static int global = 0;

int espera_evento(){
	char buffer[512]= {""};
	int res=-1;

	int fd = open("/tmp/fsc_fifo", O_RDONLY);	
	
	if(fd < 0){
		perror("Fallo en open la fifo");
		exit(-1);
	}

	int leido = read(fd, buffer, 511);
	buffer[leido-1] = '\0';
	
	if(leido < 0 && global == 1 ){
		res = ev_timeout;
		
	}else if(strcmp("fin", buffer)== 0){
		res = ev_fin;
	
	}else{
		res = ev_nofin;
	}
	
	close(fd);
	return res;

}

void manejador(int r){
	global = 1;
	printf("Alaaaarma\n");
	signal(SIGALRM, manejador);
}

int main(int args, char *argc[]){
	
	int estado = ESPERA_F1;
	int evento = 0;
	int fin = 0;
	
		signal(SIGALRM, manejador);
	while(!fin){
	
		alarm(5);		
		evento = espera_evento();
		
		switch(evento){
			case ev_fin:
				if(estado == ESPERA_F1){
					printf("Estado Espera_F1 -> Espera_F2\n");
					estado = ESPERA_F2;
				}else{
					printf("Fin de la maquina\n");
					fin = 1;
				}
				break;
				
			case ev_nofin:
				printf("nofin\n");
				break;
				
			case ev_timeout:
				printf("timeout\n");
				global = 0;
				estado = ESPERA_F1;
				break;
				
			default:
				printf("Evento no esperado");
				break;				
		}
	}
	
	
	return 0;
}
