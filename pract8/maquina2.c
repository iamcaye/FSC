//ESTADOS
#define LISTO 0
#define RETRASO_TX 1
#define ESPERA_CTS 2
#define ESPERA_TX_FIN 3
#define RETRASO_RTS_OFF 4
#define ESPERA_CTS_OFF 5
#define ERROR -1

//EVENTOS
#define t1 0
#define t2 1
#define t3 2
#define t4 3
#define t5 4
#define t6 5
#define t7 6


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


int fd_pipe[2];

int espera_evento(){
	char leer[]= "hola";
	int evento = -1;
	int leido = read(fd_pipe[0], leer, 100);
	evento = atoi(leer);
	if(leido < 0){
		perror("Fallo en recepcion de evento");
		return ERROR;
	}else{
		return evento;
	}
}

void manda_mensaje(int r){
	write(fd_pipe[1], (int *)t1, 1);
	signal(SIGINT, manda_mensaje);
}


void cts (int r){
	write(fd_pipe[1], (int *)t3, sizeof(int));
	signal(SIGTERM, cts);
}

void tx_finished (int r){
	write(fd_pipe[1], (int *)t5 ,sizeof(int));
	signal(SIGUSR1, tx_finished);
}

void cts_off (int r){
	write(fd_pipe[1], (int *)t7, sizeof(int));
	signal(SIGUSR2, cts_off);
}

void alarma(int r){
	write(fd_pipe[1], (int *)t2, sizeof(int));
	signal(SIGALRM, alarma);
}


int main(){
	
	int r = pipe(fd_pipe);
	if(r < 0){
		perror("Fallo en la pipe");
		exit(-1);
	}

	int evento;
	int estado = LISTO;
	int fin = 0;
	
	struct timeval time;
	struct itimerval timer;
	
	signal(SIGINT, manda_mensaje);
	signal(SIGTERM, cts);
	signal(SIGUSR1, tx_finished);
	signal(SIGUSR2, cts_off);
	signal(SIGALRM, alarma);
	
	while(estado != ERROR){
		evento = espera_evento();
		
		switch(evento){
			case t1:
				if(estado == LISTO){
					time.tv_sec = 1;
					time.tv_usec = 500;
					timer.it_value = time;
					timer.it_interval = time;
					
					setitimer(ITIMER_REAL, &timer, &timer);
				}else{
					printf("t1 no esperado en este momento\n");
				}
				break;
			case t2:
				if(estado == ESPERA_CTS){
					printf("Error: CTS no activa\n");
					estado = ERROR;
					
				}else if(estado == ESPERA_CTS_OFF){
					printf("Error: CTS no desactivada\n");
					estado = ERROR;
					
				}else if(estado == RETRASO_TX){
					printf("RTS activa\n");
					time.tv_sec = 3;
					time.tv_usec = 300;
					timer.it_value = time;
					timer.it_interval = time;
					setitimer(ITIMER_REAL, &timer, &timer);
					
					estado = ESPERA_CTS;
				}else if(estado = RETRASO_RTS_OFF){
					printf("RTS desactivada\n");
					time.tv_sec = 4;
					time.tv_usec = 700;
					timer.it_value = time;
					timer.it_interval = time;
					setitimer(ITIMER_REAL, &timer, &timer);
					estado = ESPERA_CTS_OFF;
				
				}else{
					printf("timeout no esperado\n");
				}
				break;
			case t3:
				if(estado == ESPERA_CTS){
					setitimer(0, 0, 0);
					printf("CTS activa");
					estado = ESPERA_TX_FIN;
				}else{
					printf("t3 no esperado\n");
				}
				break;
				
			case t5:
				if(estado == ESPERA_TX_FIN){
					time.tv_sec = 1;
					time.tv_usec = 500;
					timer.it_value = time;
					timer.it_interval = time;
					setitimer(ITIMER_REAL, &timer, &timer);
					
					estado =  RETRASO_RTS_OFF;
				}else{
					printf("t5 no esperado\n");
				}
				break;
			
			case t7:
				if(estado == ESPERA_CTS_OFF){
					setitimer(0,0,0);
					printf("CTS desactivada\n");
					estado = LISTO;
				}else{
					printf("t7 no esperado\n");
				}
				break;
			default:
				estado = ERROR;
				printf("Fail\n");
				
				
		};
	}
	
	close(fd_pipe[0]);
	close(fd_pipe[1]);

	return 0;
}